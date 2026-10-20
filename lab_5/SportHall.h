#pragma once

#include "Constants.h"
#include "SportEquipment.h"

#include <memory>
#include <string>
#include <vector>

/**
 * @brief A single record of equipment usage during one exercise.
 *
 * Captures the training/exercise context, load parameters (intensity and
 * duration) and the resulting wear of the equipment after the exercise.
 * Used by SportHall::showUsageHistory().
 */
struct UsageRecord {
    int training_number;
    int exercise_number;
    int equipment_id;
    std::string equipment_type;
    int intensity;
    int duration_minutes;
    double wear_after_exercise;
};

/**
 * @brief A sports hall that owns equipment and tracks its usage and costs.
 *
 * Owns all SportEquipment instances through std::unique_ptr, keeps a usage
 * log and accumulates the total repair/replacement cost. Non-copyable and
 * non-movable.
 */
class SportHall {
private:
    std::vector<std::unique_ptr<SportEquipment>> _equipment;
    std::vector<UsageRecord> _usage_history;
    double _total_repair_cost;

public:
    SportHall();
    ~SportHall() = default;
    SportHall(const SportHall&) = delete;
    SportHall& operator=(const SportHall&) = delete;

    /**
     * @brief Adds an equipment item to the hall.
     * @param equipment_item Unique pointer to the item to add.
     * @throws std::invalid_argument If the pointer is null or the ID is already used.
     */
    void addEquipment(std::unique_ptr<SportEquipment> equipment_item);

    /**
     * @brief Returns all non-broken equipment items.
     * @return Raw pointers to available items; ownership is not transferred.
     */
    std::vector<SportEquipment*> getAvailable();

    /**
     * @brief Returns all equipment items in the hall.
     * @return Raw pointers to items; ownership is not transferred.
     */
    std::vector<SportEquipment*> getAllEquipment();

    /**
     * @brief Appends a record to the usage history.
     * @param record The record to store.
     */
    void logUsage(const UsageRecord& record);

    /** @brief Prints the current state of all equipment and replacement hints. */
    void showStatus() const;

    /** @brief Prints the recorded equipment usage history. */
    void showUsageHistory() const;

    /**
     * @brief Repairs a single equipment item by its ID.
     * @param equipment_id ID of the item to repair.
     * @param restored_wear_percent Percentage of accumulated wear to restore.
     * @return The repair cost.
     * @throws std::invalid_argument If no item with the given ID exists or the
     *         restore percentage is outside the allowed range.
     */
    double repairEquipment(int equipment_id, double restored_wear_percent);

    /**
     * @brief Repairs every worn or broken item in the hall.
     * @return The total repair cost.
     * @details Uses constants::default_repair_restore_percent as the restore
     *          percentage for each item.
     */
    double repairAll();

    /**
     * @brief Prints a replacement plan for all equipment.
     * @details For each item decides REPLACE or KEEP & REPAIR using the
     *          wear (>80%) and relative repair cost (>60% of price) criteria.
     */
    void printReplacementPlan() const;

    /**
     * @brief Performs smart maintenance: repairs or replaces equipment.
     * @details For each item:
     *          - if wear > 80% or repair_cost > 60% of price, the item is
     *            "replaced" (fully restored via repair(100.0), new price charged);
     *          - otherwise, if the item is worn or broken, it is repaired using
     *            constants::default_repair_restore_percent;
     *          - otherwise no action is taken.
     *          Total cost is added to _total_repair_cost.
     */
    void smartMaintenance();

    /** @return Total repair/replacement cost accumulated by the hall. */
    double getTotalRepairCost() const {return _total_repair_cost;};

    /** @return The equipment usage history (read-only). */
    const std::vector<UsageRecord>& getUsageHistory() const {return _usage_history;};

    /** @return The hall's equipment container (read-only; ownership stays with the hall). */
    const std::vector<std::unique_ptr<SportEquipment>>& getEquipment() const {return _equipment;};
};