#include "Constants.h"
#include "SportHall.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

SportHall::SportHall() : _total_repair_cost(0.0) {}

void SportHall::addEquipment(std::unique_ptr<SportEquipment> equipment_item) {
    if (!equipment_item) {
        throw std::invalid_argument("Cannot add a null equipment pointer.");
    }

    for (const auto& existing_item : _equipment) {
        if (existing_item->getId() == equipment_item->getId()) {
            throw std::invalid_argument("Equipment with this ID already exists.");
        }
    }
    _equipment.push_back(std::move(equipment_item));
}

std::vector<SportEquipment*> SportHall::getAvailable() {
    std::vector<SportEquipment*> result;
    for (auto& item : _equipment) {
        if (!item->isBroken()) {
            result.push_back(item.get());
        }
    }
    return result;
}

std::vector<SportEquipment*> SportHall::getAllEquipment() {
    std::vector<SportEquipment*> result;
    for (auto& item : _equipment) {
        result.push_back(item.get());
    }
    return result;
}

void SportHall::logUsage(const UsageRecord& record) {
    _usage_history.push_back(record);
}

void SportHall::showStatus() const {
    std::cout << "\n=== Equipment status ===\n";
    for (const auto& item : _equipment) {
        std::cout << *item << '\n';
        if (item->checkWear() && !item->isBroken()) {
            std::cout << "  Recommendation: schedule replacement (wear exceeds 70%).\n";
        }
    }
}

void SportHall::showUsageHistory() const {
    std::cout << "\n=== Usage history ===\n";
    if (_usage_history.empty()) {
        std::cout << "Usage history is empty.\n";
        return;
    }

    for (const auto& record : _usage_history) {
        std::cout << "Training " << record.training_number
                  << ", exercise " << record.exercise_number
                  << ", item #" << record.equipment_id
                  << " (" << record.equipment_type << "), intensity "
                  << record.intensity << ", duration "
                  << record.duration_minutes << " min, wear after exercise "
                  << std::fixed << std::setprecision(2)
                  << record.wear_after_exercise << "%\n";
    }
}

double SportHall::repairEquipment(int equipment_id, double restored_wear_percent) {
    for (auto& item : _equipment) {
        if (item->getId() == equipment_id) {
            const double cost = item->repair(restored_wear_percent);
            _total_repair_cost += cost;
            return cost;
        }
    }
    throw std::invalid_argument("Equipment with the specified ID was not found.");
}

double SportHall::repairAll() {
    double cost = 0.0;
    for (auto& item : _equipment) {
        if (item->getWear() > 0.0 || item->isBroken()) {
            
            cost += item->repair(constants::default_repair_restore_percent);
        }
    }
    _total_repair_cost += cost;
    return cost;
}

void SportHall::printReplacementPlan() const {
    std::cout << "\n=== Optimal Replacement Plan ===\n";
    
    for (const auto& item : _equipment) {
        double repair_cost = item->calculateRepairCost();
        double new_cost = item->getCost();
        double wear = item->getWear();

    
        if (wear > 80.0 || repair_cost > new_cost * 0.6) {
            std::cout << "Item #" << item->getId() << " (" << item->getType() << "): "
                      << "REPLACE. (Wear: " << std::fixed << std::setprecision(1) << wear << "%, "
                      << "Repair: " << std::setprecision(2) << repair_cost 
                      << " vs New: " << new_cost << ")\n";
        } else {
            std::cout << "Item #" << item->getId() << " (" << item->getType() << "): "
                      << "KEEP & REPAIR. (Wear: " << std::setprecision(1) << wear << "%, "
                      << "Repair cost: " << std::setprecision(2) << repair_cost << ")\n";
        }
    }
}

void SportHall::smartMaintenance() {
    std::cout << "\n=== Smart Repair & Replace ===\n";
    double total_cost = 0.0;

    for (auto& item : _equipment) {
        double wear = item->getWear();
        double new_cost = item->getCost();
        double repair_cost = item->calculateRepairCost();

        if (wear > 80.0 || repair_cost > new_cost * 0.6) {
            total_cost += new_cost;
            item->repair(100.0);
            std::cout << "Item #" << item->getId() << " (" << item->getType() 
                      << "): REPLACED. Cost: " << new_cost << "\n";
        } else if (wear > 0.0 || item->isBroken()) {
            total_cost += repair_cost;
            item->repair(constants::default_repair_restore_percent);
            std::cout << "Item #" << item->getId() << " (" << item->getType() 
                      << "): REPAIRED. Cost: " << repair_cost << "\n";
        } else {
             std::cout << "Item #" << item->getId() << " (" << item->getType() 
                      << "): OK. No action needed.\n";
        }
    }
    
    _total_repair_cost += total_cost;
    std::cout << "Total maintenance cost: " << total_cost << "\n";
}