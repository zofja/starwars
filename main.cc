//
// Created by skonrad on 24/11/18.
//


/**
 * add this
auto debug_get_attack_moments() {
        return genSquares();
    }
    in battle if you want this to work
 */

#define BOOST_TEST_MODULE StarwarsTests

#include <boost/test/included/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "rebelfleet.h"
#include "imperialfleet.h"
#include "battle.h"
#include <cassert>

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
            : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

struct StarCruiserInfo {
    static constexpr int minSpeed = 99999;
    static constexpr int maxSpeed = 299795;
};

struct ExplorerInfo {
    static constexpr int minSpeed = 299796;
    static constexpr int maxSpeed = 2997960;
};

struct XWingInfo : public ExplorerInfo {
};


template<typename U, typename S>
bool testRebelConstruction(U shield, U speed) {
    S ship(shield, speed);

    return ship.getShield() == shield
           && ship.getSpeed() == speed;

}

template<typename U, typename S>
bool testRebelConstruction(U shield, U speed, U attackPower) {
    S ship(shield, speed, attackPower);

    return ship.getShield() == shield
           && ship.getSpeed() == speed
           && ship.getAttackPower() == attackPower;

}

template<typename U, typename S>
bool testEmpireConstruction(U shield, U attackPower) {
    S ship(shield, attackPower);

    return ship.getShield() == shield
           && ship.getAttackPower() == attackPower;
}

template<typename U, typename T>
bool checkDamageTake(T &ship, U dmg, U result) {
    ship.takeDamage(dmg);
    return ship.getShield() == result;
}

template<typename U, typename T>
bool checkDamageTake(T &&ship, U dmg, U result) {
    ship.takeDamage(dmg);
    return ship.getShield() == result;
}



BOOST_AUTO_TEST_SUITE(ProvidedTests);

    BOOST_AUTO_TEST_CASE(example) {

        XWing<float> xwing(100.0f, 300000.0f, 50.0f);
        Explorer<int> explorer(150, 400000);
        StarCruiser<unsigned> cruiser(1234, 100000, 11);
        DeathStar<long> deathStar(10000, 75);
        TIEFighter<unsigned> fighter(50, 9);
        ImperialDestroyer<int> destroyer(150, 20);
        auto battle = SpaceBattle < short,
                1, 23,
                DeathStar < long >,
                Explorer<int>,
                TIEFighter < unsigned >,
                XWing < float >> (deathStar,
                                  explorer,
                                  fighter,
                                  xwing);

        BOOST_CHECK_EQUAL(battle.countRebelFleet(), 2);
        BOOST_CHECK_EQUAL(battle.countImperialFleet(), 2);

        battle.tick(2);
        BOOST_CHECK_EQUAL(battle.countRebelFleet(), 2);
        BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);

        battle.tick(1);
        BOOST_CHECK_EQUAL(battle.countRebelFleet(), 2);
        BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);

        battle.tick(4);
        BOOST_CHECK_EQUAL(battle.countRebelFleet(), 0);
        BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);

        {
            boost::test_tools::output_test_stream output;
            cout_redirect redir(output.rdbuf());

            for (size_t i =0; i < 5; ++i) {
                battle.tick(1);
            }
            BOOST_CHECK( output.is_equal( ("IMPERIUM WON\n"
                                           "IMPERIUM WON\n"
                                           "IMPERIUM WON\n"
                                           "IMPERIUM WON\n"
                                           "IMPERIUM WON\n") ) );
        }

    }

BOOST_AUTO_TEST_SUITE_END();


BOOST_AUTO_TEST_SUITE(Rebels);

    BOOST_AUTO_TEST_CASE(construction) {
        BOOST_CHECK((testRebelConstruction<unsigned, StarCruiser<unsigned> >(100, StarCruiserInfo::minSpeed, 12)));
        BOOST_CHECK((testRebelConstruction<unsigned, StarCruiser<unsigned> >(100, StarCruiserInfo::maxSpeed, 12)));
        //todo is this required?
        BOOST_CHECK((testRebelConstruction<long long, StarCruiser<long long> >(100, StarCruiserInfo::maxSpeed, 12)));

        BOOST_CHECK((testRebelConstruction<float, Explorer<float> >(77, ExplorerInfo::minSpeed)));
        BOOST_CHECK((testRebelConstruction<float, Explorer<float> >(24, ExplorerInfo::maxSpeed)));
    }

    BOOST_AUTO_TEST_CASE(takeDamage) {
        BOOST_CHECK((checkDamageTake<unsigned>(StarCruiser<unsigned>(0, StarCruiserInfo::minSpeed, 12), 100, 0)));
        BOOST_CHECK((checkDamageTake<unsigned>(StarCruiser<unsigned>(1, StarCruiserInfo::minSpeed, 12), 100, 0)));
        BOOST_CHECK((checkDamageTake<unsigned>(StarCruiser<unsigned>(100, StarCruiserInfo::minSpeed, 12), 100, 0)));

        {
            StarCruiser<unsigned> ship(100, StarCruiserInfo::minSpeed, 42);
            BOOST_CHECK((checkDamageTake<unsigned>(ship, 8, 92)));
            BOOST_CHECK((checkDamageTake<unsigned>(ship, 42, 50)));
            BOOST_CHECK((checkDamageTake<unsigned>(ship, 999999, 0)));

        }

        {
            Explorer<float> ship(12.34, ExplorerInfo::minSpeed);
            BOOST_CHECK((checkDamageTake<float>(ship, 0.12, 12.22)));
            BOOST_CHECK((checkDamageTake<float>(ship, 1.10, 11.12)));
            BOOST_CHECK((checkDamageTake<float>(ship, 999999.999999, 0.0)));
        }


    }

BOOST_AUTO_TEST_SUITE_END();


BOOST_AUTO_TEST_SUITE(Empire);

    BOOST_AUTO_TEST_CASE(construction) {
        BOOST_CHECK((testEmpireConstruction<unsigned, DeathStar<unsigned> >(100, 12)));
        BOOST_CHECK((testEmpireConstruction<double, TIEFighter<double> >(100, 12)));
        //todo is this required?
        BOOST_CHECK((testEmpireConstruction<long long, ImperialDestroyer<long long> >(100, 12)));

    }

    BOOST_AUTO_TEST_CASE(takeDamage) {

        BOOST_CHECK((checkDamageTake<unsigned>(DeathStar<unsigned>(0, 12), 100, 0)));
        BOOST_CHECK((checkDamageTake<unsigned>(DeathStar<unsigned>(1, 12), 100, 0)));
        BOOST_CHECK((checkDamageTake<unsigned>(DeathStar<unsigned>(100, 12), 9, 91)));
        //todo is this required?
        BOOST_CHECK((checkDamageTake<long long>(DeathStar<long long>(100, 12), 9, 91)));


        {
            DeathStar<unsigned> ship(100, 42);
            BOOST_CHECK((checkDamageTake<unsigned>(ship, 8, 92)));
            BOOST_CHECK((checkDamageTake<unsigned>(ship, 42, 50)));
            BOOST_CHECK((checkDamageTake<unsigned>(ship, 999999, 0)));

        }
    }

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(Battle);

    BOOST_AUTO_TEST_CASE(time) {
        {
            XWing<float> xwing(100.0f, 300000.0f, 50.0f);
            auto battle = SpaceBattle<char, 0, 127, XWing<float>>(xwing);

            std::vector<short> res{0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121};


            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), res.size());

            for (size_t i = 0; i < res.size(); ++i) {
                BOOST_CHECK_EQUAL(battle.debug_get_attack_moments()[i], res[i]);
            }


            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);

            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(1);
                }
                BOOST_CHECK( (output.is_equal( "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n" )) );

            }

            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);


        }

        {
            XWing<float> xwing(100.0f, 300000.0f, 50.0f);
            auto battle = SpaceBattle<short, 0, std::numeric_limits<short>::max(), XWing<float>>(xwing);
            //std::cerr << std::numeric_limits<int>::max() << std::endl;
            std::vector<long long> res;
            for (long long i = 0; i * i < std::numeric_limits<short>::max(); ++i) {
                res.push_back(i * i);
            }

            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), res.size());

            for (size_t i = 0; i < res.size(); ++i) {
                BOOST_CHECK_EQUAL(battle.debug_get_attack_moments()[i], res[i]);
            }


            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);

            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(1);
                }
                BOOST_CHECK( (output.is_equal( "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n" )) );

            }

            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);


        }

        {
            XWing<float> xwing(10000000000.0f, 300000.0f, 0.0f);
            TIEFighter<float> TIE(10000000000.0f, 0.0f);
            auto battle = SpaceBattle<char, 0, std::numeric_limits<char>::max(), XWing<float>, TIEFighter<float>>(xwing, TIE);
            //std::cerr << std::numeric_limits<int>::max() << std::endl;

            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);

            int t = 0;
            for (size_t i = 0; i < 1000; ++i) {
                int jump = rand();
                battle.tick(jump);
                t += jump;
                t %= std::numeric_limits<char>::max();
                //todo czy ten oveflow trzeba osblozyc
                // BOOST_CHECK_EQUAL(battle.debug_get_current_time(), t);
            }

            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);


        }
    }


    BOOST_AUTO_TEST_CASE(construction) {
        {
            auto battle = SpaceBattle<short, 1, 4>();
            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), 3);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);

            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(i);
                }
                BOOST_CHECK( (output.is_equal( "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n" )) );

            }

            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), 3);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
        }


        {
            XWing<float> xwing(100.0f, 300000.0f, 50.0f);
            Explorer<int> explorer(150, 400000);
            StarCruiser<unsigned> cruiser(1234, 100000, 11);
            DeathStar<long> deathStar(10000, 75);
            TIEFighter<unsigned> fighter(50, 9);
            //ImperialDestroyer<int> destroyer(150, 20);
            auto battle = SpaceBattle<short,
                    1, 23,
                    DeathStar<long>,
                    Explorer<int>,
                    TIEFighter<unsigned>,
                    XWing<float >>(deathStar,
                                   explorer,
                                   fighter,
                                   xwing);
            {
                std::vector<short> res{0, 1, 4, 9, 16};

                BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), res.size());

                for (size_t i = 0; i < res.size(); ++i) {
                    BOOST_CHECK_EQUAL(battle.debug_get_attack_moments()[i], res[i]);
                }


                BOOST_CHECK_EQUAL(battle.countRebelFleet(), 2);
                BOOST_CHECK_EQUAL(battle.countImperialFleet(), 2);
            }
        }

        {
            XWing<float> xwing(100.0f, 300000.0f, 50.0f);
            XWing<float> xwing2(0.0f, 300000.0f, 50.0f);
            Explorer<int> explorer(150, 400000);
            Explorer<float> explorer2(0.0, 400000.0);
            StarCruiser<unsigned> cruiser(1234, 100000, 11);

            StarCruiser<unsigned> cruiser2(0, 100000, 11);
            StarCruiser<float> cruiser3(1234, 100000, 11);
            DeathStar<long> deathStar(10000, 75);
            TIEFighter<unsigned> fighter(50, 9);
            TIEFighter<unsigned> fighter2(0, 9);
            ImperialDestroyer<int> destroyer(150, 20);
            ImperialDestroyer<long long> destroyer2(0, 20);//todo is this required?

            auto battle = SpaceBattle<int,
                    20, 50,
                    StarCruiser<unsigned>,
                    StarCruiser<unsigned>,
                    StarCruiser<float>,
                    ImperialDestroyer<int>,
                    ImperialDestroyer<long long>,//todo is this required?
                    DeathStar<long>,
                    Explorer<int>,
                    Explorer<float>,
                    TIEFighter<unsigned>,
                    TIEFighter<unsigned>,
                    XWing<float >,
                    XWing<float >>(
                    cruiser,
                    cruiser2,
                    cruiser3,
                    destroyer,
                    destroyer2,
                    deathStar,
                    explorer,
                    explorer2,
                    fighter,
                    fighter2,
                    xwing,
                    xwing2
            );

            std::vector<int> res{0, 1, 4, 9, 16, 25, 36, 49};

            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), res.size());

            for (size_t i = 0; i < res.size(); ++i) {
                BOOST_CHECK_EQUAL(battle.debug_get_attack_moments()[i], res[i]);
            }

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 3);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 4);

        }



    }

    BOOST_AUTO_TEST_CASE(battles) {

        {
            XWing<char32_t> xwing(100, 300000, 50);
            XWing<float> xwing2(0.0f, 300000.0f, 50.0f);
            Explorer<int> explorer(150, 400000);
            Explorer<float> explorer2(0.0, 400000.0);
            StarCruiser<unsigned> cruiser(123, 100000, 11);

            StarCruiser<unsigned> cruiser2(0, 100000, 11);
            StarCruiser<float> cruiser3(1234, 100000, 11);
            DeathStar<long> deathStar(100, 75);
            TIEFighter<unsigned> fighter(50, 9);
            TIEFighter<unsigned> fighter2(0, 9);
            ImperialDestroyer<int> destroyer(150, 20);
            ImperialDestroyer<int> destroyer2(0, 20);

            auto battle = SpaceBattle<char,
                    0, 0,
                    XWing<float>,
                    TIEFighter<unsigned>,
                    Explorer<float>,
                    StarCruiser<unsigned>,
                    ImperialDestroyer<int>,


                    XWing<char32_t>,
                    Explorer<int>,
                    StarCruiser<unsigned>,
                    StarCruiser<float>,
                    DeathStar<long>,
                    TIEFighter<unsigned>,
                    ImperialDestroyer<int>>(
                    xwing2,
                    fighter2,
                    explorer2,
                    cruiser2,
                    destroyer2,

                    xwing,
                    explorer,
                    cruiser,
                    cruiser3,
                    deathStar,
                    fighter,
                    destroyer);


            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), 1);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 3);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 4);

            battle.tick(12);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 2);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 3);

            //todo co jak battle.tick(0)
            battle.tick(1);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 2);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);


            battle.tick(1);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);


            battle.tick(1);
            battle.tick(1);
            battle.tick(1);
            battle.tick(1);
            battle.tick(1);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);

            battle.tick(1);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);

            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(1);
                }
                BOOST_CHECK( (output.is_equal( "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n" )) );
            }






            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);


        }

        {
            XWing<char32_t> xwing(100, 300000, 50);
            XWing<float> xwing2(0.0f, 300000.0f, 50.0f);
            Explorer<int> explorer(150, 400000);
            Explorer<float> explorer2(0.0, 400000.0);
            StarCruiser<unsigned> cruiser(123, 100000, 11);

            StarCruiser<unsigned> cruiser2(0, 100000, 11);
            StarCruiser<float> cruiser3(1234, 100000, 11);
            DeathStar<long> deathStar(100, 75);
            TIEFighter<unsigned> fighter(50, 9);
            TIEFighter<unsigned> fighter2(0, 9);
            ImperialDestroyer<int> destroyer(150, 20);
            ImperialDestroyer<int> destroyer2(0, 20);

            auto battle = SpaceBattle<char,
                    0, 100,
                    XWing<float>,
                    TIEFighter<unsigned>,
                    Explorer<float>,
                    StarCruiser<unsigned>,
                    ImperialDestroyer<int>,


                    XWing<char32_t>,
                    Explorer<int>,
                    StarCruiser<unsigned>,
                    StarCruiser<float>,
                    DeathStar<long>,
                    TIEFighter<unsigned>,
                    ImperialDestroyer<int>>(
                    xwing2,
                    fighter2,
                    explorer2,
                    cruiser2,
                    destroyer2,

                    xwing,
                    explorer,
                    cruiser,
                    cruiser3,
                    deathStar,
                    fighter,
                    destroyer);


            BOOST_REQUIRE_EQUAL(battle.debug_get_attack_moments().size(), 11);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 3);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 4);

            battle.tick(0);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 2);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 3);

            //todo co jak battle.tick(0)
            battle.tick(0);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 2);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);


            battle.tick(0);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);


            battle.tick(0);
            battle.tick(0);
            battle.tick(0);
            battle.tick(0);
            battle.tick(0);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);

            battle.tick(0);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);

            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(12);
                }
                BOOST_CHECK( (output.is_equal( "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n"
                                               "REBELLION WON\n" )) );
            }

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
        }

        {
            XWing<float> xwing(100.0f, 300000.0f, 150.0f);
            ImperialDestroyer<int> destroyer(150, 100);

            auto battle = SpaceBattle<short, 0, 5, XWing<float>, ImperialDestroyer<int>>(xwing, destroyer);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            battle.tick(1);

            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(1);
                }
                BOOST_CHECK( (output.is_equal( "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n" )) );

            }

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 0);

        }

        {
            XWing<float> xwing(300.0f, 300000.0f, 100.0f);
            ImperialDestroyer<int> destroyer(300, 100);

            auto battle = SpaceBattle<short, 0, 5, XWing<float>, ImperialDestroyer<int>>(xwing, destroyer);

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            battle.tick(1);
            battle.tick(4);
            battle.tick(1);
            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 1);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 1);
            battle.tick(1);


            {
                boost::test_tools::output_test_stream output;
                cout_redirect redir(output.rdbuf());

                for (size_t i =0; i < 5; ++i) {
                    battle.tick(1);
                }
                BOOST_CHECK( (output.is_equal( "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n"
                                               "DRAW\n" )) );

            }

            BOOST_CHECK_EQUAL(battle.countImperialFleet(), 0);
            BOOST_CHECK_EQUAL(battle.countRebelFleet(), 0);

        }
    }

    BOOST_AUTO_TEST_CASE(pair) {
        {
            XWing<float> xwing(42, XWingInfo::minSpeed, 2);
            DeathStar<float> ds(42, 3);
            attack(ds, xwing);
            BOOST_CHECK_EQUAL(ds.getShield(), 40);
            BOOST_CHECK_EQUAL(xwing.getShield(), 39);
        }

        {
            XWing<float> xwing(42, XWingInfo::minSpeed, 2000);
            DeathStar<float> ds(42, 2000);
            attack(ds, xwing);
            /* todo czy tak ma byc */
            BOOST_CHECK_EQUAL(ds.getShield(), 0);
            BOOST_CHECK_EQUAL(xwing.getShield(), 0);
        }

        {
            XWing<double> xw(100, XWingInfo::minSpeed, 1);
            StarCruiser<double> sc(101, StarCruiserInfo::minSpeed, 2);
            Explorer<double> ex(102, ExplorerInfo::minSpeed);

            DeathStar<float> ds(103, 3);
            ImperialDestroyer<float> id(104, 4);
            TIEFighter<float> tf(105, 5);

            attack(ds, xw);
            attack(ds, sc);
            attack(ds, ex);

            attack(id, xw);
            attack(id, sc);
            attack(id, ex);

            attack(tf, xw);
            attack(tf, sc);
            attack(tf, ex);

            BOOST_CHECK_EQUAL(xw.getShield(), 88);
            BOOST_CHECK_EQUAL(sc.getShield(), 89);
            BOOST_CHECK_EQUAL(ex.getShield(), 90);

            BOOST_CHECK_EQUAL(ds.getShield(), 100);
            BOOST_CHECK_EQUAL(id.getShield(), 101);
            BOOST_CHECK_EQUAL(tf.getShield(), 102);
        }


        {
            Explorer<float> exp(42, ExplorerInfo::minSpeed);
            DeathStar<float> ds(42, 2000);
            attack(ds, exp);
            /* todo czy tak ma byc */
            BOOST_CHECK_EQUAL(ds.getShield(), 42);
            BOOST_CHECK_EQUAL(exp.getShield(), 0);
        }

        {
            //todo is this required?
            Explorer<long long> exp(42, ExplorerInfo::minSpeed);
            DeathStar<long long> ds(42, 2000);
            attack(ds, exp);
            /* todo czy tak ma byc */
            BOOST_CHECK_EQUAL(ds.getShield(), 42);
            BOOST_CHECK_EQUAL(exp.getShield(), 0);
        }


    }

BOOST_AUTO_TEST_SUITE_END();