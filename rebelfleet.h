#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <iostream>
#include <type_traits>
#include <cassert>

enum class RebelShip { EXPLORER, STARCRUISER, XWING };

template <typename U, RebelShip Type,
        int Lo = 99999, int Hi = 299795,
        int Lo1 = 299796, int Hi1 = 2997960>
class RebelStarship {
public:

//    https://stackoverflow.com/questions/1TwoArgObject964447/why-compile-error-with-enable-if/26678178
    template <typename T = U>
    RebelStarship(typename std::enable_if<Type == RebelShip::EXPLORER, T>::type shield, T speed)
    : shield(shield), speed(speed) {
        assert(speed >= Lo1 && speed <= Hi1);
        std::cout << "2 arg ctor\n";
    }

    template <typename T = U>
    RebelStarship(typename std::enable_if<Type == RebelShip::STARCRUISER || Type == RebelShip::XWING, T>::type shield, T speed, T attackPower)
    : shield(shield), speed(speed), attackPower(attackPower) {
        if (Type == RebelShip::STARCRUISER)  {
            assert(speed >= Lo && speed <= Hi);
        } else {
            assert(speed >= Lo1 && speed <= Hi1);
        }
        std::cout << "3 arg ctor\n";
    }

    U getShield() { return shield; }
    U getSpeed() { return speed; }

    void takeDamage(U damage) {
        if (shield >= damage) shield -= damage;
        else shield = 0;
    }

    template <typename = typename std::enable_if<(Type == RebelShip::STARCRUISER || Type == RebelShip::XWING)> >
    U getAttackPower() {
        return attackPower;
    }

    RebelStarship getShipType() { return Type; }

    bool isImperial() { return false; }


private:
    U shield;
    const U speed;
    U attackPower;
};

template <typename U>
using Explorer = RebelStarship<U, RebelShip::EXPLORER>;

template <typename U>
using StarCruiser = RebelStarship<U, RebelShip::STARCRUISER>;

template <typename U>
using XWing = RebelStarship<U, RebelShip::XWING>;

#endif
