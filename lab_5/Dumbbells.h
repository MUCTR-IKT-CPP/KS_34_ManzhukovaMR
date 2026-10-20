#pragma once

#include "Constants.h"
#include "SportEquipment.h"
#include <stdexcept>

/**
 * @brief A dumbbell piece of equipment.
 *
 * Adds a physical weight to the base equipment model and fixes the type
 * name to "Dumbbells" and the base wear rate to constants::dumbbells_wear_rate.
 */
class Dumbbells final : public SportEquipment {
private:
    double _weight; 
public:
    Dumbbells(int id, int wear_resistance, double cost, double weight): 
        SportEquipment(id, "Dumbbells", wear_resistance, cost, constants::dumbbells_wear_rate),
        _weight(weight) {
    if (_weight <= 0.0) {
        throw std::invalid_argument("Dumbbell weight must be positive.");
    }
}
    double getWeight() const {return _weight;};
    double getWearRate() const override {return SportEquipment::getWearRate();};
    std::string getSpecificInfo() const override {return "weight: " + std::to_string(_weight) + " kg";};
};
