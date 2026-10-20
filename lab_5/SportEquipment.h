#pragma once

#include "Constants.h"
#include <iosfwd>
#include <string>

/**
 * @brief Base class for a piece of sports equipment.
 *
 * Stores identity, cost and wear state, and defines the wear/repair model.
 * Derived classes may override getWearRate() and getSpecificInfo() to add
 * type-specific behavior.
 */
class SportEquipment {
private:
    friend class Training;
    int _id;
    std::string _type;
    int _wear_resistance;
    double _cost;
    double _wear;
    bool _broken;
    int _usage_count;
    double _base_wear_rate;

protected:
    /**
     * @brief Constructs an equipment item.
     * @param id Unique positive identifier.
     * @param type Non-empty type name.
     * @param wear_resistance Resistance in [min_wear_resistance, max_wear_resistance].
     * @param cost Non-negative original price.
     * @param base_wear_rate Positive base wear rate.
     * @throws std::invalid_argument If any argument is outside its allowed range.
     */
    SportEquipment(int id, const std::string& type, int wear_resistance,
                   double cost, double base_wear_rate);

public:
    virtual ~SportEquipment() = default;
    SportEquipment(const SportEquipment&) = delete;
    SportEquipment& operator=(const SportEquipment&) = delete;

    int getId() const { return _id; }
    const std::string& getType() const { return _type; }
    int getWearResistance() const { return _wear_resistance; }
    double getCost() const { return _cost; }
    double getWear() const { return _wear; }
    double getCondition() const { return 100.0 - _wear; }
    bool isBroken() const { return _broken; }
    int getUsageCount() const { return _usage_count; }
    virtual double getWearRate() const {return _base_wear_rate;};

    /**
     * @brief Forces the equipment into a worn/broken state.
     * @param _wear Wear value to set (defaults to max wear).
     * @param _broken Broken flag to set.
     */
    void breakEquipment(double new_wear = constants::max_wear, bool new_broken = true) {
        _wear = new_wear;
        _broken = new_broken;
    }

    /**
     * @brief Applies exercise load and updates wear.
     * @param intensity Exercise intensity.
     * @param duration_minutes Duration in minutes.
     * @param random_coefficient Random multiplier for wear.
     * @return true if the item was used, false if it was already broken.
     * @details The item becomes broken when wear reaches the maximum.
     */
    bool use(int intensity, int duration_minutes, double random_coefficient);

    /** @return true if the item is broken or its wear exceeds the replacement threshold. */
    bool checkWear() const {return _broken || _wear > constants::replacement_wear_threshold;};

    /**
     * @brief Removes the requested percentage of accumulated wear.
     * @param restored_wear_percent Percentage of current wear to restore.
     * @return The repair cost.
     * @throws std::invalid_argument If the percentage is outside the allowed range.
     */
    double repair(double restored_wear_percent);

    /** @brief Calculates repair cost from the original price and current wear. */
    double calculateRepairCost() const {return _cost * (_wear / constants::percent) * constants::repair_cost_factor;};

    /** @brief Prints the item description to the given stream. */
    virtual void print(std::ostream& out) const;

    /** @return Type-specific extra info appended to print(); empty by default. */
    virtual std::string getSpecificInfo() const {return {};};
};

/**
 * @brief Stream insertion operator; delegates to SportEquipment::print().
 */
inline std::ostream& operator<<(std::ostream& out, const SportEquipment& equipment) {
    equipment.print(out);
    return out;
}