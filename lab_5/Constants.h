#pragma once

namespace constants {

/** @name Identifier and wear limits */
constexpr int min_id = 1;
constexpr int min_wear_resistance = 1;
constexpr int max_wear_resistance = 100;
constexpr double initial_wear = 0.0;
constexpr double max_wear = 100.0;
constexpr double replacement_wear_threshold = 70.0;
constexpr double sudden_break_wear_threshold = 80.0;

/** @name Exercise load limits */
constexpr int min_intensity = 1;
constexpr int max_intensity = 5;
constexpr int min_duration_minutes = 1;
constexpr int max_duration_minutes = 10;
constexpr double min_random_coefficient = 0.5;
constexpr double max_random_coefficient = 2.0;

/** @name Repair parameters */
constexpr double min_repair_restore_percent = 50.0;
constexpr double max_repair_restore_percent = 80.0;
constexpr double repair_cost_factor = 0.5;
constexpr double broken_item_repair_cost_factor = 0.2;
constexpr double default_repair_restore_percent = 65.0;

/** @name Random event parameters */
constexpr double sudden_break_probability_percent = 1.0;
constexpr int probability_scale = 100;

/** @name Simulation parameters */
constexpr int min_exercises = 5;
constexpr int max_exercises = 15;
constexpr int simulation_training_count = 100;

/** @name Per-type base wear rates */
constexpr double ball_wear_rate = 0.35;
constexpr double jump_rope_wear_rate = 0.55;
constexpr double dumbbells_wear_rate = 0.20;

/** @brief Percentage scale factor (100.0). */
constexpr double percent = 100.0;
}

