#include <iostream>

#include "battle.h"
#include "rebelfleet.h"

using namespace std;

int main() {

    StarCruiser<int> starCruiser(1, 1, 1);
    cout << starCruiser.getSpeed() << " " << starCruiser.getShield() << " " << starCruiser.getAttackPower() << "\n";


    return 0;
}