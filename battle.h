#include <vector>
#include <tuple>
#include "imperialfleet.h"
#include "rebelfleet.h"

#ifndef BATTLE_H
#define BATTLE_H

#endif

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

public:

    explicit SpaceBattle(S... ship) : ships(ship...) {}

private:

    std::tuple<S...> ships;

};

template<typename I, size_t n = 0, typename ...S>
constexpr void iterateRebel(I &ship, std::tuple<S...> &t) {
    if constexpr(n < sizeof...(S)) {

        if (!std::get<n>(t).isImperial()) {
            attack(ship, std::get<n>(t));
        }
        iterateRebel<n + 1, S...>(t);
    }
}

template<size_t n = 0, typename ...S>
constexpr void iterateImperial(std::tuple<S...> &t) {
    if constexpr(n < sizeof...(S)) {

        if (std::get<n>(t).isImperial()) {
            ImperialStarship ship = std::get<n>(t);
            iterateRebel<0, S...>(ship, t);
        }
        iterateImperial<n + 1, S...>(t);
    }
}