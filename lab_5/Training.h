#pragma once

#include "SportHall.h"
#include <vector>

/** @brief A single exercise within a training session. */
struct Exercise {
    int number;
    int intensity;
    int duration_minutes;
    int equipment_id;
};

/** @brief Aggregated outcome of one training session. */
struct TrainingReport {
    int training_number = 0;
    int exercises_completed = 0;
    std::vector<int> broken_inventory_ids;
    double repair_cost = 0.0;
};

/**
 * @brief Runs training sessions on a SportHall and produces reports.
 *
 * Holds a reference to the hall (no ownership) and the training number.
 * Non-copyable implicitly due to the reference member.
 */
class Training {
private:
    SportHall& _sport_hall;
    int _training_number;

    /** @brief Generates an exercise with random intensity/duration for the given equipment. */
    Exercise _generateExercise(int number, int equipment_id) const;

    /** @brief Returns a uniform random real in [min_value, max_value]. */
    double _randomReal(double min_value, double max_value) const;

public:
    /**
     * @brief Constructs a training bound to the given hall.
     * @param hall Hall to use; must outlive this Training.
     * @param training_number Training sequence number.
     */
    Training(SportHall& hall, int training_number);

    /**
     * @brief Runs a session, selects available equipment randomly, and collects a report.
     * @param exercise_count Number of exercises to perform.
     * @return Aggregated session report.
     */
    TrainingReport conduct(int exercise_count);

    /** @brief Prints the given report to stdout. */
    void printReport(const TrainingReport& report) const;
};