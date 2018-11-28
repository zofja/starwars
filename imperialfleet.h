#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include <iostream>
#include <type_traits>
#include <tgmath.h>
#include "rebelfleet.h"

enum class ImperialShip { DEATHSTAR, IMPERIALDESTROYER, TIEFIGHTER };


template <typename U, ImperialShip Type>
class ImperialStarship {
public:

    using typeValue = U; // po co to komu

    ImperialStarship(U shield, U attackPower)
            : shield(shield), attackPower(attackPower) {
        std::cout << "2 arg ctor\n";
    }

    U getShield() { return shield; }

    void takeDamage(U damage) { shield -= damage; }

    U getAttackPower() { return attackPower; }

    ImperialShip getShipType() { return Type; }



private:
    U shield;
    U attackPower;
};

template <typename I, typename U>
void attackPower(I &imperialShip, Explorer<U> &rebelShip) {
    rebelShip.takeDamage(imperialShip.getAttackPower());
}


template <typename I, typename R>
void attack(I &imperialShip, R &rebelShip) {
    rebelShip.takeDamage(imperialShip.getAttackPower());
    imperialShip.takeDamage(rebelShip.getAttackPower());
}


template <typename U>
using DeathStar = ImperialStarship<U, ImperialShip::DEATHSTAR>;

template <typename U>
using ImperialDestroyer = ImperialStarship<U, ImperialShip::IMPERIALDESTROYER>;

template <typename U>
using TIEFighter = ImperialStarship<U, ImperialShip::TIEFIGHTER>;


#endif