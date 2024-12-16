#pragma once

#include <string_view>
#include "sinfourmis.h"

class Interface {
  public:
	Interface() = default;
	// Move-only, should not be copied
	Interface(Interface&&);
	Interface& operator=(Interface&&) noexcept;

	virtual ~Interface() = default;

	virtual void load(std::string_view path) = 0;
    virtual reine_action reine_activation(const fourmi_etat fourmis[], const salle *salle) = 0;
	virtual fourmi_action fourmi_activation(const fourmi_etat *fourmi, const salle *salle) = 0;

  private:
	// Non-copyable
	Interface(const Interface&) = delete;
	Interface& operator=(const Interface&) = delete;	
};
