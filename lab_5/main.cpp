#include "Constants.h"
#include "Ball.h"
#include "Dumbbells.h"
#include "JumpRope.h"
#include "SportHall.h"
#include "Training.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>


int readInt(const char* prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }

        std::cout << "Invalid input. Enter an integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        SportHall hall;

        hall.addEquipment(std::make_unique<Ball>(1, 80, 1500.0, 22.0));
        hall.addEquipment(std::make_unique<Ball>(2, 75, 1200.0, 20.0));
        hall.addEquipment(std::make_unique<Ball>(3, 85, 1800.0, 24.0));

  
        hall.addEquipment(std::make_unique<JumpRope>(4, 65, 700.0, 2.8));
        hall.addEquipment(std::make_unique<JumpRope>(5, 70, 900.0, 3.0));

        hall.addEquipment(std::make_unique<Dumbbells>(6, 95, 3500.0, 10.0));
        hall.addEquipment(std::make_unique<Dumbbells>(7, 90, 2800.0, 5.0)); 

        int next_training_number = 1;
        bool running = true;

        while (running) {
            std::cout << "\n========== SPORTS HALL ==========\n"
                      << "1. Show equipment\n"
                      << "2. Run a training session\n"
                      << "3. Show usage history\n"
                      << "4. Repair all equipment\n"
                      << "5. Smart Repair & Replace (Use Recommendations)\n"
                      << "6. Simulate n training sessions\n"
                      << "0. Exit\n";

            int choice = readInt("Choose an option: ");

            try {
                switch (choice) {
                case 1:
                    hall.showStatus();
                    break;

                case 2: {
                    int count = readInt("Number of exercises (5..15): ");
                    Training training(hall, next_training_number++);
                    training.conduct(count);
                    break;
                }

                case 3:
                    hall.showUsageHistory();
                    break;

                case 4: {
                    const double cost = hall.repairAll();
                    std::cout << "Repair complete. Cost: " << cost
                              << "\nTotal repair cost: "
                              << hall.getTotalRepairCost() << '\n';
                    break;
                }

                case 6: {

                    int num_trainings = readInt("Enter number of training sessions to simulate: ");

                    if (num_trainings <= 0) { 
                         std::cout << "Number of trainings must be positive.\n"; 
                        break;
                    }

                    for (int i = 0; i < num_trainings; ++i) {
                        auto available_items = hall.getAvailable();
                        if (available_items.empty()) {
                            std::cout << "All equipment is broken. Repairing it to continue.\n";
                            hall.repairAll();
                        }

                        Training training(hall, next_training_number++);
                        int range = constants::max_exercises - constants::min_exercises + 1;
                        int count = constants::min_exercises + (std::rand() % range);
                        training.conduct(count);
                    }

                    std::cout << "\n=== Usage statistics (Most Used First) ===\n";
                    

                    auto equipment_stats = hall.getAllEquipment(); 
                    

                    std::sort(equipment_stats.begin(), equipment_stats.end(), 
                        [](SportEquipment* a, SportEquipment* b) {
                        return a->getUsageCount() > b->getUsageCount();});

                    for (const auto& item : equipment_stats) {
                        std::cout << "Equipment #" << item->getId() << " (" << item->getType()
                                  << "): uses " << item->getUsageCount()
                                  << ", wear " << std::fixed << std::setprecision(1) << item->getWear() << "%\n";
                    }
                    
                    std::cout << "Total exercise records: "
                              << hall.getUsageHistory().size() << '\n'
                              << "Total repair cost: "
                              << hall.getTotalRepairCost() << '\n';
                              
                    hall.printReplacementPlan();
                    break;
                }

                case 5:
                    hall.smartMaintenance();
                    break;

                case 0:
                    running = false;
                    break;

                default:
                    std::cout << "Invalid menu option.\n";
                    break;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
