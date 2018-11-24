#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <iostream>
#include <type_traits>

enum class RebelShip { EXPLORER, STARCRUISER, XWING };

static const int TwoArgObject = 2;
static const int ThreeArgObject = 3;

template <typename U, int Size, RebelShip Type>
class RebelStarship {
public:

//    https://stackoverflow.com/questions/1TwoArgObject964447/why-compile-error-with-enable-if/26678178
    template <typename T = U>
    RebelStarship(typename std::enable_if<Size == TwoArgObject, T>::type shield, T speed)
    : shield(shield), speed(speed) {
        std::cout << "2 arg ctor\n";
    }

    template <typename T = U>
    RebelStarship(typename std::enable_if<Size == ThreeArgObject, T>::type shield, T speed, T attackPower)
    : shield(shield), speed(speed), attackPower(attackPower) {
        std::cout << "3 arg ctor\n";
    }

    U getShield() { return shield; }
    U getSpeed() { return speed; }

    void takeDamage(U damage) { shield -= damage; }

    template <typename = typename std::enable_if<(Type == RebelShip::STARCRUISER || Type == RebelShip::XWING)>::type >
    U getAttackPower() {
        return attackPower;
    }

    RebelStarship getShipType() { return Type; }


private:
    U shield;
    U speed;
    U attackPower;
};

template <typename U>
using Explorer = RebelStarship<U, TwoArgObject, RebelShip::EXPLORER>;

template <typename U>
using StarCruiser = RebelStarship<U, ThreeArgObject, RebelShip::STARCRUISER>;

template <typename U>
using XWing = RebelStarship<U, ThreeArgObject, RebelShip::XWING>;

#endif
