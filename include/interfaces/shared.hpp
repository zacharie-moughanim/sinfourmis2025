#pragma once

#include "interface.hpp"

/// Generic interface for interfacing with shared objects
class SharedInterface : public Interface {
  public:
    SharedInterface() = default;
    ~SharedInterface() override = default;

    void load(std::string_view path) override;
    reine_retour reine_activation(fourmi_etat fourmis[], const size_t nb_fourmis,
                                  const reine_etat *etat, const salle *salle) override;
    fourmi_retour fourmi_activation(fourmi_etat *fourmi, const salle *salle) override;

  private:
    reine_retour (*reine_fn)(fourmi_etat *, const size_t, const reine_etat *, const salle *);

    fourmi_retour (*fourmi_fn)(fourmi_etat *, const salle *);
};
