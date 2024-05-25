#pragma once

#include <iostream>
#include <unordered_set>
#include <span>
#include <optional>

#include "state.h"

class AStar
{
    std::unordered_set<State> m_opened;
    std::unordered_set<State> m_closed;
    field_bits m_restricted_cells;
    State m_start_state;
    State m_target_state;
    State m_last_state;

public:
    explicit AStar( std::span<std::string> init_list, const State &start_state, const State &target_state );

    auto getRestrictedCells() const -> const field_bits&;
    auto testRestricted(size_t x, size_t y) const -> bool;

    auto solve() -> void;
    std::vector<State> getStatesSequence();

    friend auto operator<<( std::ostream& os, const AStar& state ) -> std::ostream&;

private:
    auto closeState(const State &state) -> void;
    auto openStatesNearState(const State &state) -> void;
    auto openStatesForCell(const State &state, size_t x, size_t y) -> void;
    auto openState(const State &parent_state, size_t old_x, size_t old_y, size_t new_x, size_t new_y) -> void;
    auto getBestOpenState() -> const State&;
};
