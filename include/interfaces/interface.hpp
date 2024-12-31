#pragma once

#include "sinfourmis.h"
#include <string_view>

class Interface {
  public:
    Interface() = default;
    // Move-only, should not be copied
    Interface(Interface &&);
    Interface &operator=(Interface &&) noexcept;

    virtual ~Interface() = default;

    virtual void load(std::string_view path) = 0;
    virtual reine_retour reine_activation(const fourmi_etat fourmis[],
                                          const size_t nb_fourmis, const salle *salle) = 0;
    virtual fourmi_retour fourmi_activation(const fourmi_etat *fourmi, const salle *salle) = 0;

  private:
    // Non-copyable
    Interface(const Interface &) = delete;
    Interface &operator=(const Interface &) = delete;
};
