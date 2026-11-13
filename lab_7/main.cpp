#include "entities.hpp"
#include <iostream>

int main() {
    World world;

    world.add(std::make_unique<NPC<SimpleAI, SimplePhysics>>("Guard"));
    world.add(std::make_unique<NPC<AggressiveAI, SimplePhysics>>("Berserker"));
    world.add(std::make_unique<NPC<PassiveAI, SimplePhysics>>("Healer"));

    std::cout << "=== World Tick ===\n";
    world.tick(0.5);

    std::cout << "=== Second Tick ===\n";
    world.tick(1.0);

    return 0;
}