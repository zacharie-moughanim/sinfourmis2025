#pragma once

#include "interface.hpp"
#include <unordered_map>

class Dummy : public Interface {
public:
	Dummy() = default;
	~Dummy() override = default;
	void load(std::string_view path) override;
    reine_retour reine_activation(fourmi_etat fourmis[], const size_t nb_fourmis,
                                  const reine_etat *etat, const salle *salle) override;
    fourmi_retour fourmi_activation(fourmi_etat *fourmi, const salle *salle) override;
private:
	uint32_t fourmis_count = 0;
	std::unordered_map<int, int> levels;
	char hold_state = 0;
};
