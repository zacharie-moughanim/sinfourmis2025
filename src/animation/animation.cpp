#include "animation/animation.hpp"

void Animation::start_frame() {
	if (file.is_open()) {
		file.close();
		throw std::runtime_error("Start / end frame must be called in pairs");
	}
	turn++;
	current_path = path / (std::to_string(turn) + ".json");
	file.open(current_path);

}

void Animation::end_frame() {

	file.close();	
}