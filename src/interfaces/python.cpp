#include "interfaces/python.hpp"
#include "abstract.h"
#include "dictobject.h"
#include "listobject.h"
#include "longobject.h"
#include "object.h"
#include "pytypedefs.h"
#include "sinfourmis.h"
#include <iostream>
#include <sys/types.h>

/// Converts a salle struct to a PyObject
PyObject *salle_to_po(const salle *s) {
    PyObject *py_salle = PyDict_New();
    PyDict_SetItemString(py_salle, "type", PyLong_FromLong(s->type));
    PyDict_SetItemString(py_salle, "pheromone", PyLong_FromLong(s->pheromone));
    PyDict_SetItemString(py_salle, "degre", PyLong_FromLong(s->degre));

    return py_salle;
}

/// Converts a reinre_etat struct to a PyObject
PyObject *reine_etat_to_po(const reine_etat *etat) {
    PyObject *py_etat = PyDict_New();
    PyDict_SetItemString(py_etat, "nourriture", PyLong_FromLong(etat->nourriture));
    PyDict_SetItemString(py_etat, "result", PyLong_FromLong(etat->result));
    PyDict_SetItemString(py_etat, "max_nourriture", PyLong_FromLong(etat->max_nourriture));
    PyDict_SetItemString(py_etat, "max_eau", PyLong_FromLong(etat->max_eau));
    PyDict_SetItemString(py_etat, "max_vie", PyLong_FromLong(etat->max_vie));
    PyDict_SetItemString(py_etat, "max_degats", PyLong_FromLong(etat->max_degats));
    PyDict_SetItemString(py_etat, "duree_amelioration", PyLong_FromLong(etat->duree_amelioration));
    PyDict_SetItemString(py_etat, "max_stockage", PyLong_FromLong(etat->max_stockage));
    PyDict_SetItemString(py_etat, "max_production", PyLong_FromLong(etat->max_production));
    PyDict_SetItemString(py_etat, "max_envoi", PyLong_FromLong(etat->max_envoi));

    return py_etat;
}

/// Converts a fourmi_etat struct to a PyObject
PyObject *fourmi_etat_to_po(const fourmi_etat *etat) {
    PyObject *py_etat = PyDict_New();
    PyDict_SetItemString(py_etat, "vie", PyLong_FromLong(etat->vie));
    PyDict_SetItemString(py_etat, "eau", PyLong_FromLong(etat->eau));
    PyDict_SetItemString(py_etat, "result", PyLong_FromLong(etat->result));
    PyDict_SetItemString(py_etat, "nourriture", PyLong_FromLong(etat->nourriture));

    PyObject *py_mem = PyList_New(256);
    for (int i = 0; i < 256; i++) {
        PyList_SetItem(py_mem, i, PyLong_FromLong(etat->memoire[i]));
    }
    PyDict_SetItemString(py_etat, "memoire", py_mem);

    return py_etat;
}

/// Converts a PyObject to a reine_retour struct
reine_retour po_to_reine_retour(PyObject *po) {
    PyObject *py_action = PyDict_GetItemString(po, "action");
    PyObject *py_arg = PyDict_GetItemString(po, "arg");

    return {
        .action = (reine_action)PyLong_AsLong(py_action),
        .arg = (int32_t)PyLong_AsLong(py_arg),
    };
}

/// Converts a PyObject to a fourmi_retour struct
fourmi_retour po_to_fourmi_retour(PyObject *po) {
    PyObject *py_action = PyDict_GetItemString(po, "action");
    PyObject *py_arg = PyDict_GetItemString(po, "arg");

    // TODO: Complete api
    return {.action = (fourmi_action)PyLong_AsLong(py_action),
            .arg = (int32_t)PyLong_AsLong(py_arg),
            .depose_pheromone = false,
            .pheromone = 0};
}

void PythonInterface::load(std::string_view path) {
    PyObject *pName;

    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(path.data());
    if (pName == NULL) {
        PyErr_Print();
        std::cerr << "Failed to decode path" << std::endl;
        Py_XDECREF(pName);
        return;
    }

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFourmiActivation = PyObject_GetAttrString(pModule, "fourmi_activation");
        pReineActivation = PyObject_GetAttrString(pModule, "reine_activation");

        if (pFourmiActivation && pReineActivation && PyCallable_Check(pFourmiActivation) &&
            PyCallable_Check(pReineActivation)) {
            // load successful
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            std::cerr << "Cannot find functions fourmi_activation and reine_activation"
                      << std::endl;
        }
    } else {
        PyErr_Print();
        std::cerr << "Failed to load " << path << std::endl;
    }
}

PythonInterface::~PythonInterface() {
    Py_XDECREF(pFourmiActivation);
    Py_XDECREF(pReineActivation);
    Py_XDECREF(pModule);

    Py_Finalize();
}

reine_retour PythonInterface::reine_activation(fourmi_etat fourmis[], const size_t nb_fourmis,
                                               const reine_etat *etat, const salle *salle) {
    // Convert arguments to python objects
    PyObject *pArgs = PyTuple_New(3);
    PyObject *pFourmis = PyList_New(nb_fourmis);
    for (size_t i = 0; i < nb_fourmis; i++) {
        PyList_SetItem(pFourmis, i, fourmi_etat_to_po(&fourmis[i]));
    }
    PyTuple_SetItem(pArgs, 0, pFourmis);

    PyObject *pEtat = reine_etat_to_po(etat);
    PyTuple_SetItem(pArgs, 1, pEtat);

    PyObject *pSalle = salle_to_po(salle);
    PyTuple_SetItem(pArgs, 2, pSalle);

    // Calls reine_activation and converts result back
    PyObject *pResult = PyObject_CallObject(pReineActivation, pArgs);
    reine_retour retour = po_to_reine_retour(pResult);

    Py_XDECREF(pArgs);
    Py_XDECREF(pResult);
    return retour;
}

fourmi_retour PythonInterface::fourmi_activation(fourmi_etat *etat, const salle *salle) {
    // Convert arguments to python objects
    PyObject *pArgs = PyTuple_New(2);
    PyObject *pEtat = fourmi_etat_to_po(etat);
    PyTuple_SetItem(pArgs, 0, pEtat);

    PyObject *pSalle = salle_to_po(salle);
    PyTuple_SetItem(pArgs, 1, pSalle);

    // Calls fourmi_activation and converts result back
    PyObject *pResult = PyObject_CallObject(pFourmiActivation, pArgs);
    fourmi_retour result = po_to_fourmi_retour(pResult);

    Py_XDECREF(pArgs);
    Py_XDECREF(pResult);
    return result;
}