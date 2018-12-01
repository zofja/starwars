#include <tuple>
#include <algorithm>
#include "imperialfleet.h"
#include "rebelfleet.h"

#ifndef BATTLE_H
#define BATTLE_H

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

    static_assert(t0 >= 0, "Time cannot be negative");
    static_assert(t0 <= t1, "Start must be before end");
    static_assert(t1 >= 0, "Time cannot be negative");

public:

    explicit SpaceBattle(S... ship) : ships(ship...) {
        countFleet(ships);
    }

    size_t countImperialFleet() {
        return imperialFleet;
    }

    size_t countRebelFleet() {
        return rebelFleet;
    }

    void tick(T timeStep) {

        if (rebelFleet == 0 && imperialFleet == 0) std::cout << "DRAW\n";
        else if (rebelFleet == 0) std::cout << "IMPERIUM WON\n";
        else if (imperialFleet == 0) std::cout << "REBELLION WON\n";
        else {
            if (std::binary_search(powers.begin(), powers.end(), time)) {
                iterateImperial(ships);
            }
        }

        time = (time + timeStep) % (t1 + 1);
    }

private:

    T time = t0;

    std::tuple<S...> ships;

    size_t rebelFleet = 0;

    size_t imperialFleet = 0;

//    TODO umądrzyć albo zostawić
    static const uint64_t N = 20;

    static constexpr auto power(uint64_t n) {
        return n * n;
    }

    static constexpr auto genPowers() {

        std::array<uint64_t, N> arr{};

        for (uint64_t i = 0; i < N; i++) {
            arr[i] = power(i);
        }

        return arr;
    }


// TODO nwm co z tym warningiem o static storage duration
    static constexpr auto powers = genPowers();


    template<size_t n = 0, typename ...S1>
    constexpr void countFleet(std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            if (std::get<n>(t).isImperial()) {
                imperialFleet++;
            } else {
                rebelFleet++;
            }

            countFleet<n + 1>(t);
        }
    }

    template<size_t n = 0, typename I, typename...S1>
    constexpr void iterateRebel(I &imperialStarship, std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            auto &s = std::get<n>(t);

            if (!s.isImperial()) {

                if (imperialStarship.getShield() > 0 && s.getShield() > 0) {
                    attack(imperialStarship, s);
                    if (imperialStarship.getShield() == 0) {
                        std::cout << "Imperial killed\n";
                        imperialFleet--;
                    }
                    if (s.getShield() == 0) {
                        std::cout << "Rebel killed\n";
                        rebelFleet--;
                    }
                }
            }
            iterateRebel<n + 1>(imperialStarship, t);
        }
    }

    template<size_t n = 0, typename ...S1>
    constexpr void iterateImperial(std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            auto &s = std::get<n>(t);

            if (s.isImperial()) {
                iterateRebel(s, t);
            }
            iterateImperial<n + 1>(t);
        }
    }
};

#endif