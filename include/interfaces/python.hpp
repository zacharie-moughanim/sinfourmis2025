#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "interfaces/interface.hpp"

class PythonInterface : public Interface {
  public:
    PythonInterface() = default;
    PythonInterface(PythonInterface &&);
    PythonInterface &operator=(PythonInterface &&) noexcept;
    ~PythonInterface();

    bool load(std::string_view path) override;
    reine_retour reine_activation(fourmi_etat fourmis[], const size_t nb_fourmis,
                                  const reine_etat *etat, const salle *salle) override;
    fourmi_retour fourmi_activation(fourmi_etat *fourmi, const salle *salle) override;

  private:
    PyObject *pModule, *pReineActivation;
    PyThreadState *gpy; // global python interpreter

    PythonInterface(const PythonInterface &) = delete;
    PythonInterface &operator=(const PythonInterface &) = delete;
};
