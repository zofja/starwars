#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <iostream>
#include <type_traits>
#include <cassert>

enum class RebelShip {
    EXPLORER, STARCRUISER, XWING
};

template <typename U, RebelShip Type,
        int LO = 99999, int HI = 299795,
        int LO1 = 299796, int HI1 = 2997960>
class RebelStarship {
public:

    using typeValue = U;

    template <typename T = U>
    RebelStarship(typename std::enable_if<Type == RebelShip::EXPLORER, T>::type shield, T speed)
    : shield(shield), speed(speed) {
        assert(speed >= LO1 && speed <= HI1);
    }

    template <typename T = U>
    RebelStarship(typename std::enable_if<Type != RebelShip::EXPLORER, T>::type shield, T speed, T attackPower)
    : shield(shield), speed(speed), attackPower(attackPower) {
        if (Type == RebelShip::STARCRUISER)  {
            assert(speed >= LO && speed <= HI);
        } else {
            assert(speed >= LO1 && speed <= HI1);
        }
    }

    U getShield() { return shield; }
    U getSpeed() { return speed; }

    void takeDamage(U damage) {
        if (shield >= damage) shield -= damage;
        else shield = 0;
    }

    template <typename = typename std::enable_if<(Type != RebelShip::EXPLORER)>>
    U getAttackPower() {
        return attackPower;
    }

    constexpr bool isImperial() { return false; }


private:

    U shield;
    U speed;
    U attackPower;
};

template <typename U>
using Explorer = RebelStarship<U, RebelShip::EXPLORER>;

template <typename U>
using StarCruiser = RebelStarship<U, RebelShip::STARCRUISER>;

template <typename U>
using XWing = RebelStarship<U, RebelShip::XWING>;

#endif
