#ifndef BATTLE_H
#define BATTLE_H

#include <tuple>
#include <algorithm>
#include "imperialfleet.h"
#include "rebelfleet.h"

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

public:

    explicit SpaceBattle(S... ship) : ships(ship...) {
        static_assert(std::is_integral<T>::value, "Time must be of integral type");
        static_assert(t0 >= 0, "Time cannot be negative");
        static_assert(t0 <= t1, "Start must be before end");
        countFleet(ships);
    }

    size_t countImperialFleet() {
        return imperialFleet;
    }

    size_t countRebelFleet() {
        return rebelFleet;
    }

    void tick(T timeStep) {

        static constexpr auto squares = genSquares();

        if (rebelFleet == 0 && imperialFleet == 0) std::cout << "DRAW\n";
        else if (rebelFleet == 0) std::cout << "IMPERIUM WON\n";
        else if (imperialFleet == 0) std::cout << "REBELLION WON\n";
        else {
            if (std::binary_search(squares.begin(), squares.end(), time)) {
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

    static constexpr auto getMaxN() {
        auto i = 0;
        for (; i * i < t1; i++) {}
        return i;
    }

    static constexpr auto power(uint64_t n) {
        return n * n;
    }

    static constexpr auto genSquares() {

        const auto N = getMaxN();

        std::array<uint64_t, N> arr{};

        for (uint64_t i = 0; i < N; i++) {
            arr[i] = power(i);
        }

        return arr;
    }

    template<size_t n = 0, typename ...S1>
    constexpr void countFleet(std::tuple<S1...> &t) {
        if constexpr(n < sizeof...(S1)) {

            if constexpr (std::tuple_element_t<n, std::tuple<S1...>>::isImperial()) {
                imperialFleet++;
            } else {
                rebelFleet++;
            }

            countFleet<n + 1>(t);
        }
    }

    template<size_t n = 0, typename I, typename...S1>
    constexpr void iterateRebel(I &imperialStarship, std::tuple<S1...> &t) {
        if constexpr (n < sizeof...(S1)) {

            auto &s = std::get<n>(t);

            if constexpr (!std::tuple_element_t<n, std::tuple<S1...>>::isImperial()) {

                if (imperialStarship.getShield() > 0 && s.getShield() > 0) {
                    attack(imperialStarship, s);
                    if (imperialStarship.getShield() == 0) {
                        imperialFleet--;
                    }
                    if (s.getShield() == 0) {
                        rebelFleet--;
                    }
                }
            }
            iterateRebel<n + 1>(imperialStarship, t);
        }
    }

    template<size_t n = 0, typename ...S1>
    constexpr void iterateImperial(std::tuple<S1...> &t) {
        if constexpr (n < sizeof...(S1)) {

            auto &s = std::get<n>(t);

            if constexpr (std::tuple_element_t<n, std::tuple<S1...>>::isImperial()) {
                iterateRebel(s, t);
            }
            iterateImperial<n + 1>(t);
        }
    }
};

#endif