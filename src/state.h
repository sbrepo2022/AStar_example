#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <tuple>
#include <optional>
#include <span>

constexpr size_t field_width = 8;
constexpr size_t field_height = 8;
constexpr size_t field_size = field_width * field_height;
using field_bits = std::bitset<field_size>;

class State
{
    static size_t current_uid;

    size_t m_uid = 0;
    size_t m_parent_uid = 0;
    size_t m_cost = 0;

    field_bits m_field;

public:
    /* Общие функции */

    State() = default;
    explicit State( const field_bits &field );
    explicit State( std::span<std::string> init_list );
    State( const State &state ) = default;
    State( State &&state ) = default;
    auto operator= ( const State &state ) -> State& = default;
    auto operator= ( State &&state ) -> State& = default;

    auto operator==( const State &other_state ) const -> bool;

    auto setValue( size_t x, size_t y, bool value = true ) -> State&;
    auto resetValue( size_t x, size_t y ) -> State&;
    auto testValue( size_t x, size_t y ) const -> bool;

    auto getField() const -> const field_bits&;

    friend auto operator<<( std::ostream& os, const State& state ) -> std::ostream&;


    /* Функции для решения задачи */

    auto getUID() const noexcept -> size_t;
    auto setCost(size_t cost) noexcept -> void;
    auto getCost() const noexcept -> size_t;
    auto setParentUID(size_t parent_uid) noexcept -> void;
    auto getParentUID() const noexcept -> size_t;

    [[nodiscard]] auto distance( const State &other_state ) const noexcept -> size_t;
    [[nodiscard]] auto nearestTo( size_t x, size_t y ) const noexcept -> std::optional<std::tuple<size_t, size_t>>;
};


namespace std {
    template <>
    struct hash<State> {
        std::size_t operator()(const State& s) const noexcept {
            return std::hash<unsigned long long>{}(s.getField().to_ullong());
        }
    };
}
