#include "Training.h"

#include "Constants.h"
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>


Training::Training(SportHall& hall, int training_number)
    : _sport_hall(hall), _training_number(training_number) {
    if (_training_number <= 0) {
        throw std::invalid_argument("Training number must be positive.");
    }
}

Exercise Training::_generateExercise(int number, int equipment_id) const {
    return Exercise{number, constants::min_intensity + std::rand() % constants::max_intensity,
                    constants::min_duration_minutes + std::rand() % constants::max_duration_minutes, equipment_id};
}

double Training::_randomReal(double min_value, double max_value) const {
    
    const double unit = static_cast<double>(std::rand()) / RAND_MAX;
    return min_value + unit * (max_value - min_value);
}

TrainingReport Training::conduct(int exercise_count) {
    if (exercise_count < constants::min_exercises || exercise_count > constants::max_exercises) {
        throw std::invalid_argument("A training session must contain 5 to 15 exercises.");
    }

    TrainingReport report;
    report.training_number = _training_number;

    std::cout << "\nTraining session #" << _training_number
              << " (" << exercise_count << " exercises)\n";

    for (int i = 1; i <= exercise_count; ++i) {
        auto available_items = _sport_hall.getAvailable();
        if (available_items.empty()) {
            std::cout << "No equipment is available. The training session has ended early.\n";
            break;
        }

        SportEquipment* equipment_item = available_items[std::rand() % available_items.size()];
        const Exercise exercise = _generateExercise(i, equipment_item->getId());
        const double coefficient = _randomReal(constants::min_random_coefficient, constants::max_random_coefficient);

        equipment_item->use(exercise.intensity, exercise.duration_minutes, coefficient);
        ++report.exercises_completed;

        _sport_hall.logUsage(UsageRecord{
            _training_number, exercise.number, equipment_item->getId(), equipment_item->getType(),
            exercise.intensity, exercise.duration_minutes, equipment_item->getWear()
        });

        std::cout << "Exercise " << exercise.number
                  << ": " << equipment_item->getType()
                  << ", intensity " << exercise.intensity
                  << ", duration " << exercise.duration_minutes
                  << " min, coefficient " << std::fixed << std::setprecision(2)
                  << coefficient << ", wear " << equipment_item->getWear() << "%\n";

        if (!equipment_item->isBroken() && equipment_item->getWear() > constants::sudden_break_wear_threshold
            && std::rand() % constants::probability_scale < constants::sudden_break_probability_percent) {
            
            equipment_item->breakEquipment();
            report.broken_inventory_ids.push_back(equipment_item->getId());
            std::cout << "Unexpected equipment failure #" << equipment_item->getId() << "!\n";
        }

        if (equipment_item->isBroken()
            && std::find(report.broken_inventory_ids.begin(),
                         report.broken_inventory_ids.end(), equipment_item->getId())
               == report.broken_inventory_ids.end()) {
            report.broken_inventory_ids.push_back(equipment_item->getId());
        }
    }

    for (int id : report.broken_inventory_ids) {
        for (const auto& item : _sport_hall.getEquipment()) {
            if (item->getId() == id) {
                report.repair_cost += item->getCost() * constants::broken_item_repair_cost_factor;
                break;
            }
        }
    }

    printReport(report);
    return report;
}

void Training::printReport(const TrainingReport& report) const {
    std::cout << "\n=== Training report #" << report.training_number << " ===\n"
              << "Exercises completed: " << report.exercises_completed << '\n';

    if (report.broken_inventory_ids.empty()) {
        std::cout << "No equipment was broken.\n";
    } else {
        std::cout << "Broken equipment (IDs): ";
        for (int id : report.broken_inventory_ids) {
            std::cout << id << ' ';
        }
        std::cout << "\nEstimated repair cost: " << std::fixed
                  << std::setprecision(2) << report.repair_cost << '\n';
    }

    // _sport_hall.showStatus();
}

