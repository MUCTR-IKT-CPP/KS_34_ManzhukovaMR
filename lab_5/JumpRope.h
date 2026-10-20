#pragma once

#include "Constants.h"
#include "SportEquipment.h"
#include <stdexcept>

/**
 * @brief A jump rope piece of equipment.
 *
 * Adds a physical length to the base equipment model and fixes the type
 * name to "Jump rope" and the base wear rate to constants::jump_rope_wear_rate.
 */
class JumpRope final : public SportEquipment {
private:
    double _length; 
public:
    JumpRope(int id, int wear_resistance, double cost, double length): 
        SportEquipment(id, "Jump rope", wear_resistance, cost, constants::jump_rope_wear_rate),
        _length(length) {
    if (_length <= 0.0) {
        throw std::invalid_argument("Jump rope length must be positive.");
    }
}
    double getLength() const {return _length;};
    double getWearRate() const override {return SportEquipment::getWearRate();};
    std::string getSpecificInfo() const override {return "length: " + std::to_string(_length) + " m";};
};

