#include "rebelfleet.h"
#include "imperialfleet.h"
#include "battle.h"

int main() {

    XWing<float> xwing(100.0f, 300000.0f, 50.0f);
    Explorer<int> explorer(150, 400000);
    StarCruiser<unsigned> cruiser(1234, 100000, 11);
    DeathStar<long> deathStar(10000, 75);
    TIEFighter<unsigned> fighter(50, 9);
    ImperialDestroyer<int> destroyer(150, 20);
    auto battle = SpaceBattle<short, 1, 23,
            DeathStar<long>,
            Explorer<int>,
            TIEFighter<unsigned>,
            XWing<float>>(deathStar,
                          explorer,
                          fighter,
                          xwing);


    std::cout << "testy, teściki: \n\n";

    DeathStar<int> ds(1, 1);
    XWing<float> xw(1.1f, 300000.0f, 1.0f);
    auto battle1 = SpaceBattle<short, 1, 10, DeathStar<int>, XWing<float>>(ds, xw);

    battle1.tick(3);
    assert(battle1.countImperialFleet() == 0);
    battle1.tick(1); // wypisuje REBELLION WON

    attack<DeathStar<long>, Explorer<int>>(deathStar, explorer);
    attack<DeathStar<long>, XWing<float>>(deathStar, xwing);

    std::cout << "\n\ntesty Peczary: \n\n";

    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 2);

    battle.tick(2);
    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1);
    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(4);
    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1); // Wypisuje "IMPERIUM WON\n".
}
