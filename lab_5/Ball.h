#pragma once

#include "Constants.h"
#include "SportEquipment.h"
#include <stdexcept>


/**
 * @brief A ball piece of equipment.
 *
 * Adds a physical diameter to the base equipment model and fixes the
 * type name to "Ball" and the base wear rate to constants::ball_wear_rate.
 */
class Ball final : public SportEquipment {
private:
    double _diameter; 
public:
    Ball(int id, int wear_resistance, double cost, double diameter): 
        SportEquipment(id, "Ball", wear_resistance, cost, constants::ball_wear_rate),
        _diameter(diameter) {
    if (_diameter <= 0.0) {
        throw std::invalid_argument("Ball diameter must be positive.");
    }
}
    double getDiameter() const {return _diameter;};
    double getWearRate() const override {return SportEquipment::getWearRate();};
    std::string getSpecificInfo() const override {return "diameter: " + std::to_string(_diameter) + " cm";};
};


