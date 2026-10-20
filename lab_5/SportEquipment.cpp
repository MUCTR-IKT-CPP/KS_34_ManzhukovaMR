#include "Constants.h"
#include "SportEquipment.h"

#include <algorithm>
#include <iomanip>
#include <ostream>
#include <stdexcept>


SportEquipment::SportEquipment(int id, const std::string& type, int wear_resistance,
                               double cost, double base_wear_rate)
    : _id(id), _type(type), _wear_resistance(wear_resistance), _cost(cost),
      _wear(constants::initial_wear), _broken(false), _usage_count(0),_base_wear_rate(base_wear_rate) {

    if (_id <= 0) {throw std::invalid_argument("Equipment ID must be positive.");}
    if (_type.empty()) {throw std::invalid_argument("Equipment type cannot be empty.");}
    if (_wear_resistance < constants::min_wear_resistance || _wear_resistance > constants::max_wear_resistance) {
        throw std::invalid_argument("Wear resistance must be between 1 and 100.");
    }
    if (_cost < 0.0) {throw std::invalid_argument("Cost cannot be negative.");}
    if (_base_wear_rate <= 0.0) {throw std::invalid_argument("Base wear rate must be positive.");}
}


bool SportEquipment::use(int intensity, int duration_minutes, double random_coefficient) {
    if (isBroken()) {
        return false;
    }
    ++_usage_count;

    const double resistance_factor = constants::percent / static_cast<double>(_wear_resistance);
    const double added_wear = getWearRate() * intensity * duration_minutes
                           * random_coefficient / resistance_factor;

    _wear = std::min(constants::max_wear, _wear + added_wear);

    if (_wear >= constants::max_wear) {
        _broken = true;
    }
    return true;
}

double SportEquipment::repair(double restored_wear_percent) {
    // if (restored_wear_percent < constants::min_repair_restore_percent || restored_wear_percent > constants::max_repair_restore_percent) {
    //     throw std::invalid_argument("Repair must restore 50 to 80 percent of accumulated wear.");
    // }
    if (restored_wear_percent < 0 || restored_wear_percent > 100) {
        throw std::invalid_argument("Repair must restore 50 to 80 percent of accumulated wear.");
    }

    const double repair_cost = calculateRepairCost();
    _wear *= (1.0 - restored_wear_percent / constants::percent);
    _broken = (_wear >= constants::max_wear);
    return repair_cost;
}


void SportEquipment::print(std::ostream& out) const {
    out << "ID: " << _id
        << ", type: " << _type
        << ", wear resistance: " << _wear_resistance
        << ", cost: " << std::fixed << std::setprecision(2) << _cost
        << ", wear: " << std::setprecision(1) << _wear << "%"
        << ", condition: " << getCondition() << "%"
        << ", uses: " << _usage_count
        << ", status: " << (_broken ? "BROKEN" : "available");

    const std::string specific_info = getSpecificInfo();
    if (!specific_info.empty()) {
        out << ", " << specific_info;
    }
}
 
