#ifndef BATTLE_H
#define BATTLE_H

#include <tuple>
#include <algorithm>
#include "imperialfleet.h"
#include "rebelfleet.h"

/** Variadic class template managing space battles
 * @tparam T – type of time parameters @param t0 and @param t1
 * @tparam t0 – start time
 * @tparam t1 – end time
 * @tparam S – parameter pack representing starships
 */
template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

public:

    /** Constructor
     * Unpacks parameter pack @param Ships into private @param ships
     * Counts starships with countFleet function
     */
    explicit SpaceBattle(S... Ships) : ships(Ships...) {

        static_assert(std::is_integral<T>::value, "Time must be of integral type");
        static_assert(t0 >= 0, "Time cannot be negative");
        static_assert(t0 <= t1, "Start must be before end");
        countFleet(ships);
    }

    /**
     * @return @param imperialFleet
     */
    size_t countImperialFleet() { return imperialFleet; }

    /**
     * @return @param imperialFleet
     */
    size_t countRebelFleet() { return rebelFleet; }

    /**
     * Checks @param time
     * If @param time is time of the attack then calls iterateImperial function
     * Increases @param time by @param timeStep
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

    /**
     * Determines maximum size of the array @param squares at compile-time
     * @return @param i
     */
    static constexpr auto getMaxN() {

        auto i = 0;
        while (i * i <= t1) i++;
        return i;
    }

    /**
     * Generates static array of squares of integers at compile-time
     * @return @param squares
     */
    static constexpr auto genSquares() {

        const auto N = getMaxN();

        std::array<T, N> squares{};

        for (T i = 0; i < N; i++) {
            squares[i] = i * i;
        }

        return squares;
    }

    /** Variadic function template
     * Iterates over @param t and counts ships of type ImperialStarhip and RebelStarship
     * @tparam n – index of current element in tuple
     * @tparam S1 – parameter pack representing starships' types
     * @param t – tuple of starships
     */
    template<size_t n = 0, typename ...S1>
    constexpr void countFleet(std::tuple<S1...> &t) {

        if constexpr(n < sizeof...(S1)) {

            if constexpr (std::tuple_element_t<n, std::tuple<S1...>>::isImperial()) {
                if (std::get<n>(t).getShield() > 0) {
                    imperialFleet++;
                }
            } else {
                if (std::get<n>(t).getShield() > 0) {
                    rebelFleet++;
                }
            }

            countFleet<n + 1>(t);
        }
    }

    /** Variadic function template
     * Iterates over tuple @param t and attacks starships of type RebelStarship
     * @tparam n – index of current element in tuple
     * @tparam I – type of @param imperialStarship
     * @tparam S1 – parameter pack representing starships
     * @param imperialStarship – starship of type ImperialStarship which attack RebelStarships
     * @param t – tuple of starships
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

    /** Variadic function template
     * Iterates over tuple @param t and commences attack if the starship is of type ImperialStarship
     * @tparam n – index of current element in tuple
     * @tparam S1 – parameter pack representing starships' types
     * @param t – tuple of starships
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