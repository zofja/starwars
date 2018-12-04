#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <iostream>
#include <type_traits>
#include <cassert>

namespace {
    enum class RebelShip {
        EXPLORER, STARCRUISER, XWING
    };
}

/** Class template representing rebel starships
 * @tparam U – type of @param shield, @param speed and @param attackPower
 * @tparam Type – starship type
 * @tparam LO – lowest speed of StarCruiser
 * @tparam HI – highest speed of StarCruiser
 * @tparam LO1 – lowest speed of XWing and Explorer
 * @tparam HI1 – highest speed ofXWing and Explorer
 */
template<typename U, RebelShip Type,
        int LO = 99999, int HI = 299795,
        int LO1 = 299796, int HI1 = 2997960>
class RebelStarship {
public:

    using valueType = U;

    /**
     * Constructor for Explorer<U>
     */
    template<typename = typename std::enable_if<(Type == RebelShip::EXPLORER)>>
    RebelStarship(U shield, U speed)
            : shield(shield), speed(speed) {
        assert(speed >= LO1 && speed <= HI1);
    }

    /**
     * Constructor for XWing<U> and StarCruiser<U>
     */
    template<typename = typename std::enable_if<(Type != RebelShip::EXPLORER)>>
    RebelStarship(U shield, U speed, U attackPower)
            : shield(shield), speed(speed), attackPower(attackPower) {
        if (Type == RebelShip::STARCRUISER) {
            assert(speed >= LO && speed <= HI);
        } else {
            assert(speed >= LO1 && speed <= HI1);
        }
    }

    /*
     * GETTERS
     */
    U getShield() { return shield; }

    U getSpeed() { return speed; }

    template<typename = typename std::enable_if<(Type != RebelShip::EXPLORER)>>
    U getAttackPower() {
        return attackPower;
    }

    /**
     * Decreases shield by @param damage if possible,
     * otherwise sets to 0
     */
    void takeDamage(U damage) {
        if (shield >= damage) shield -= damage;
        else shield = 0;
    }

     /**
      * Determines that the ship is not of type ImperialStarship
      * at compile-time
      * @return false;
      */
    static constexpr bool isImperial() { return false; }

private:

    U shield;
    U speed;
    U attackPower;
};

/**
 * Specializations of class template RebelStarship
 */
template<typename U>
using Explorer = RebelStarship<U, RebelShip::EXPLORER>;

template<typename U>
using StarCruiser = RebelStarship<U, RebelShip::STARCRUISER>;

template<typename U>
using XWing = RebelStarship<U, RebelShip::XWING>;

#endif