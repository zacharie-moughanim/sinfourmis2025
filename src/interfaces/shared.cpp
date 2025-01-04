#include "interfaces/shared.hpp"
#include "sinfourmis.h"

#include <cstddef>
#include <dlfcn.h>
#include <iostream>

void SharedInterface::load(std::string_view path) {
    void *handle = dlopen(path.data(), RTLD_LAZY);

    reine_fn = (reine_retour(*)(fourmi_etat *, const size_t, const reine_etat *,
                                const salle *))dlsym(handle, "reine_activation");
    if (!reine_fn) {
        std::cerr << "Error loading reine_activation: " << dlerror() << std::endl;
    }

    fourmi_fn = (fourmi_retour(*)(fourmi_etat *, const salle *))dlsym(handle, "fourmi_activation");
    if (!fourmi_fn) {
        std::cerr << "Error loading fourmi_activation: " << dlerror() << std::endl;
    }
}

reine_retour SharedInterface::reine_activation(fourmi_etat fourmis[], const size_t nb_fourmis,
                                               const reine_etat *etat, const salle *salle) {
    return reine_fn(fourmis, nb_fourmis, etat, salle);
}

fourmi_retour SharedInterface::fourmi_activation(fourmi_etat *fourmi, const salle *salle) {
    return fourmi_fn(fourmi, salle);
}
