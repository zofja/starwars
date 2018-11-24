#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include <iostream>
#include <type_traits>

enum class ImperialShip { DEATHSTAR, IMPERIALDESTROYER, TIEFIGHTER };

static const int TwoArgObject = 2;
static const int ThreeArgObject = 3;

template <typename U, int Size, ImperialShip Type>
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

template <typename U>
using DeathStar = ImperialStarship<U, TwoArgObject, ImperialShip::DEATHSTAR>;

template <typename U>
using ImperialDestroyer = ImperialStarship<U, ThreeArgObject, ImperialShip::IMPERIALDESTROYER>;

template <typename U>
using TIEFighter = ImperialStarship<U, ThreeArgObject, ImperialShip::TIEFIGHTER>;

#endif