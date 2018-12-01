#include <vector>
#include <tuple>
#include <algorithm>
#include "imperialfleet.h"
#include "rebelfleet.h"

#ifndef BATTLE_H
#define BATTLE_H

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

//    TODO co zrobić static

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

    void print() {
        for (int i = 0; i < N; i++) {
            std::cout << powers[i] << "\n";
        }
    }

private:

    T time = t0;

    std::tuple<S...> ships;

    size_t rebelFleet = 0;

    size_t imperialFleet = 0;

    static const uint64_t N = (1UL << 10); // TODO umądrzyć

    static constexpr auto power(uint64_t n) {
        return n * n;
    }

    static constexpr auto gen_power() {

        std::array<uint64_t, N> arr{};

        for (uint64_t i = 0; i < N; i++) {
            arr[i] = power(i);
        }

        return arr;
    }

    static constexpr auto powers = gen_power();

    template<size_t n = 0, typename ...S1>
    constexpr void countFleet(std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            if (std::get<n>(t).isImperial()) {
                imperialFleet++;
            }
            else {
                rebelFleet++;
            }

            countFleet<n + 1, S1...>(t);
        }
    }

    template<typename I, size_t n = 0, typename...S1>
    constexpr void iterateRebel(I &imperialStarship, std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            if (!std::get<n>(t).isImperial()) {

                if (imperialStarship.getShield() > 0 && std::get<n>(t).getShield() > 0) {
                    attack(imperialStarship, std::get<n>(t));
                    if (imperialStarship.getShield() == 0) {
                        std::cout << "Imperial killed\n";
                        imperialFleet--;
                    }
                    if (std::get<n>(t).getShield() == 0) {
                        std::cout << "Rebel killed\n";
                        rebelFleet--;
                    }
                }

            }
            iterateRebel<I, n + 1, S1...>(imperialStarship, t);
        }
    }

    template<size_t n = 0, typename ...S1>
    constexpr void iterateImperial(std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            if (std::get<n>(t).isImperial()) {
                iterateRebel(std::get<n>(t), t);
            }
            iterateImperial<n + 1, S1...>(t);
        }
    }
};

#endif