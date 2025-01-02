#pragma once

#include "interface.hpp"

class Dummy : public Interface {
public:
	Dummy() = default;
	~Dummy() override = default;
	void load(std::string_view path) override;
	reine_retour reine_activation(const fourmi_etat fourmis[], const size_t nb_fourmis,
	                              const reine_etat *etat, const salle *salle) override;
	fourmi_retour fourmi_activation(const fourmi_etat *fourmi, const salle *salle) override;

};
