#pragma once

#include <concepts>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class IEntity {
public:
    virtual ~IEntity() = default;
    virtual void update(double dt) = 0;
    virtual std::string name() const = 0;
};

/**
 * @brief Compile-time contract for AI policies.
 *
 * Any type @c T satisfying this concept must expose @c decide(IEntity&).
 * Enforced at template instantiation time when used as an @c NPC parameter.
 */
template <typename T>
concept AIModel = requires(T ai, IEntity& entity) {
    { ai.decide(entity) } -> std::same_as<void>;
};


/**
 * @brief Compile-time contract for physics policies.
 *
 * Any type @c T satisfying this concept must expose
 * @c apply(IEntity&, double\ dt). Used to validate @c NPC template arguments.
 */
template <typename T>
concept PhysicsModel = requires(T physics, IEntity& entity, double dt) {
    { physics.apply(entity, dt) } -> std::same_as<void>;
};


/**
 * @brief Concrete AI policy that prints a generic "thinking" message.
 *
 * Stateless and cheap to copy; swapped in at compile time via the @c NPC
 * template parameter.
 */
class SimpleAI {
public:
    void decide(IEntity& entity) {
        std::cout << entity.name() << " is thinking...\n";
    }
};

/**
 * @brief Concrete AI policy that prints an attack message.
 *
 * Shares the same interface as @c SimpleAI, demonstrating interchangeable
 * behavior without inheritance.
 */
class AggressiveAI {
public:
    void decide(IEntity& entity) {
        std::cout << entity.name() << ": The character attacks the enemy!\n";
    }
};

/**
 * @brief Concrete AI policy that prints an avoidance/rest message.
 *
 * Another drop-in replacement that satisfies the @c AIModel concept.
 */
class PassiveAI {
public:
    void decide(IEntity& entity) {
        std::cout << entity.name()
        << ": The character avoids combat and restores strength.\n";
    }
};


/**
 * @brief Concrete physics policy that moves the entity by @c dt units.
 *
 * Satisfies @c PhysicsModel and logs the movement. Would be replaced by,
 * for example, gravity or collision-resolution code.
 */
class SimplePhysics {
public:
    void apply(IEntity& entity, double dt) {
        std::cout << entity.name() << " moves by " << dt
                  << " units.\n";
    }
};


/**
 * @brief Concrete entity parameterized by an AI policy @c A and a physics
 *        policy @c P.
 *
 * Inherits @c IEntity so it can live in the @c World's heterogeneous
 * container, while its behavior is selected statically (no virtual dispatch
 * for policies). The constructor forwards a name and optional policy
 * instances; @c update() first asks the AI to decide, then lets physics move
 * the entity.
 *
 * @tparam A AI policy satisfying @c AIModel.
 * @tparam P Physics policy satisfying @c PhysicsModel.
 */
template <AIModel A, PhysicsModel P>
class NPC : public IEntity {
private:
    std::string name_;
    A ai_;
    P physics_;

public:
    NPC(std::string entity_name, A ai = {}, P physics = {})
        : name_{std::move(entity_name)},
        ai_{std::move(ai)},
        physics_{std::move(physics)} {}

    void update(double dt) override {
        ai_.decide(*this);
        physics_.apply(*this, dt);
    }

    std::string name() const override { return name_; }
};


/**
 * @brief Owns a heterogeneous collection of entities and advances them.
 *
 * Entities are stored as @c unique_ptr<IEntity>; @c add() transfers
 * ownership, and @c tick(dt) calls @c update(dt) on every entity
 * polymorphically.
 */
class World {
private:
    std::vector<std::unique_ptr<IEntity>> entities_;

public:
    void add(std::unique_ptr<IEntity> entity) {
        entities_.push_back(std::move(entity));
    }

    void tick(double dt) {
        for (auto& entity : entities_) {
            entity->update(dt);
        }
    }
};

template <typename T>
T Summ(T a, T b){
    return a + b;
}
