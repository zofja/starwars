#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include <iostream>
#include <type_traits>
#include "rebelfleet.h"

/** Class template representing imperial starships
 * @tparam U – type of @param shield and @param attackPower
 */
template<typename U>
class ImperialStarship {
public:

    using valueType = U;

    /**
     * Constructor
     */
    ImperialStarship(U shield, U attackPower) : shield(shield), attackPower(attackPower) {
    }

    /**
     * GETTERS
     */
    U getShield() { return shield; }

    U getAttackPower() { return attackPower; }

    /**
     * Decreases shield by @param damage if possible,
     * otherwise sets to 0
     */
    void takeDamage(U damage) {

        if (shield >= damage) {
            shield -= damage;
        } else {
            shield = 0;
        }
    }

    /**
     * Determines that the ship is not of type ImperialStarship
     * at compile-time
     * @return true;
     */
    static constexpr bool isImperial() { return true; }

private:

    U shield;
    U attackPower;
};

/**
 * Specialization of attack function for @param rebelShip of type Explorer
 * @tparam I – type of @param imperialShip
 * @tparam R – type of @param rebelShip
 */
template<typename I, typename R, typename = typename std::enable_if<(std::is_same<R, Explorer<typename R::valueType>>::value)>>
void attack(I &imperialShip, Explorer<typename R::valueType> &rebelShip) {

    static_assert(I::isImperial(), "Imperial ship must be imperial");

    rebelShip.takeDamage(imperialShip.getAttackPower());
}

/**
 * Attack function for @param rebelShip of type different than Explorer
 * @tparam I – type of @param imperialShip
 * @tparam R – type of @param rebelShip
 */

template<typename I, typename R, typename = typename std::enable_if<(!std::is_same<R, Explorer<typename R::valueType>>::value)>>
void attack(I &imperialShip, R &rebelShip) {

    static_assert(I::isImperial() && !R::isImperial(),
                  "Imperial ship must be imperial or rebel ship must be rebel");

    rebelShip.takeDamage(imperialShip.getAttackPower());
    imperialShip.takeDamage(rebelShip.getAttackPower());
}

/**
 * Specializations of class template ImperialStarship
 */
template<typename U>
using DeathStar = ImperialStarship<U>;

template<typename U>
using ImperialDestroyer = ImperialStarship<U>;

template<typename U>
using TIEFighter = ImperialStarship<U>;

#endif