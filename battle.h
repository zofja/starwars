#ifndef BATTLE_H
#define BATTLE_H

#include <tuple>
#include <algorithm>
#include "imperialfleet.h"
#include "rebelfleet.h"

/*
 * Class managing space battle on ships
 * @param T – type of given time parameters
 * @param t0 – start time
 * @param t1 – end time
 * @param S... parameter pack representing ships
 */
template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

public:

    /*
     * Constructor unpacking parameter pack @p Ships into private std::tuple<S...> @param ships
     * Calls countFleet to count Imperial starships and Rebel starhips
     */
    explicit SpaceBattle(S... Ships) : ships(Ships...) {

        static_assert(std::is_integral<T>::value, "Time must be of integral type");
        static_assert(t0 >= 0, "Time cannot be negative");
        static_assert(t0 <= t1, "Start must be before end");
        countFleet(ships);
    }

    /*
     * GETTERS
     */
    size_t countImperialFleet() { return imperialFleet; }

    size_t countRebelFleet() { return rebelFleet; }

    /*
     * Starts battle in at a time @time
     * Changes @time by @timeStep
     */
    void tick(T timeStep) {

        static constexpr auto squares = genSquares();

        if (rebelFleet == 0 && imperialFleet == 0) {
            std::cout << "DRAW\n";
        } else if (rebelFleet == 0) {
            std::cout << "IMPERIUM WON\n";
        } else if (imperialFleet == 0) {
            std::cout << "REBELLION WON\n";
        } else {
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

    /*
     * Counts maximum size of the array of squares
     * in compile time
     */
    static constexpr auto getMaxN() {

        auto i = 0;
        while (i * i < t1) i++;
        return i;
    }

    /*
     * Generates the array of squares
     * in compile time
     */
    static constexpr auto genSquares() {

        const auto N = getMaxN();

        std::array<T, N> squares{};

        for (T i = 0; i < N; i++) {
            squares[i] = i * i;
        }

        return squares;
    }

    /*
     * Counts the number of ships of type ImperialStarhip and RebelStarship
     */
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

    /*
     * Iterates over Rebel starships and attack if the ships is RebelStarship
     */
    template<size_t n = 0, typename I, typename...S1>
    constexpr void iterateRebel(I &imperialStarship, std::tuple<S1...> &t) {

        if constexpr (n < sizeof...(S1)) {

            if constexpr (!std::tuple_element_t<n, std::tuple<S1...>>::isImperial()) {

                auto &rebelStarship = std::get<n>(t);

                if (imperialStarship.getShield() > 0 && rebelStarship.getShield() > 0) {

                    attack(imperialStarship, rebelStarship);

                    if (imperialStarship.getShield() == 0) {
                        imperialFleet--;
                    }
                    if (rebelStarship.getShield() == 0) {
                        rebelFleet--;
                    }
                }
            }

            iterateRebel<n + 1>(imperialStarship, t);
        }
    }

    /*
     * Iterates over Imperial starships
     * Calls iterateRebel when the ship is Imperial
     */
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