#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include <iostream>
#include <type_traits>
#include "rebelfleet.h"

template<typename U>
class ImperialStarship {
public:

    using typeValue = U;

    ImperialStarship(U shield, U attackPower) : shield(shield), attackPower(attackPower) {
    }

    U getShield() { return shield; }

    void takeDamage(U damage) {
        if (shield >= damage) shield -= damage;
        else shield = 0;
    }

    U getAttackPower() { return attackPower; }

    constexpr bool isImperial() { return true; }


private:

    U shield;
    U attackPower;
};

template<typename I, typename R, typename U>
typename std::enable_if<(std::is_same<R, Explorer<U>>::value)>::type attack(I &imperialShip, Explorer<U> &rebelShip) {
//    static_assert(imperialShip.isImperial(), "Imperial ship must be imperial");
    rebelShip.takeDamage(imperialShip.getAttackPower());
}

template<typename I, typename U>
void attack(I &imperialShip, Explorer<U> &rebelShip) {
//    static_assert(imperialShip.isImperial(), "Imperial ship must be imperial");
    rebelShip.takeDamage(imperialShip.getAttackPower());
}

template<typename I, typename R>
void attack(I &imperialShip, R &rebelShip) {
//    static_assert(imperialShip.isImperial() && !rebelShip.isImperial(), "Imperial must be imperial or rebel must be rebel");
    rebelShip.takeDamage(imperialShip.getAttackPower());
    imperialShip.takeDamage(rebelShip.getAttackPower());
}

template<typename U>
using DeathStar = ImperialStarship<U>;

template<typename U>
using ImperialDestroyer = ImperialStarship<U>;

template<typename U>
using TIEFighter = ImperialStarship<U>;

#endif