#include "a_star.h"

#include <numeric>
#include <functional>
#include <stdexcept>


AStar::AStar( std::span<std::string> restricted_list, const State &start_state, const State &target_state  )
    : m_opened()
    , m_closed()
    , m_restricted_cells(std::accumulate(restricted_list.begin(), restricted_list.end(), std::string()))
    , m_start_state(start_state)
    , m_target_state(target_state)
{
    m_opened.insert(start_state);
}


auto AStar::getRestrictedCells() const -> const field_bits&
{
    return m_restricted_cells;
}


auto AStar::testRestricted(size_t x, size_t y) const -> bool
{
    return m_restricted_cells.test(x + y * field_width);
}


auto AStar::solve() -> void
{
    if (m_opened.size() == 0)
    {
        return;
    }

    while(true)
    {
        const State& best_state = getBestOpenState();
        if (best_state == m_target_state)
        {
            m_last_state = best_state;
            return;
        }

        openStatesNearState(best_state);
        closeState(best_state);
    }
}


std::vector<State> AStar::getStatesSequence()
{
    std::unordered_set<State> final_set;
    final_set.insert(m_opened.begin(), m_opened.end());
    final_set.insert(m_closed.begin(), m_closed.end());

    std::vector<State> state_sequence;

    State cur_state = m_last_state;
    while (cur_state.getParentUID() != 0)
    {
        state_sequence.push_back(cur_state);

        size_t uid = cur_state.getParentUID();
        auto it = std::find_if(
            final_set.begin(),
            final_set.end(),
            [ uid ]( const State& state )
            {
                return state.getUID() == uid;
            }
        );

        if (it != final_set.end()) {
            cur_state = *it;
        } else {
            throw std::domain_error("Cannot create state sequence: state with uid not found in closed states set!");
        }
    }

    std::reverse(state_sequence.begin(), state_sequence.end());

    return state_sequence;
}


auto AStar::closeState(const State &state) -> void
{
    auto it = m_opened.find(state);
    if (it != m_opened.end())
    {
        m_closed.insert(*it);
        m_opened.erase(it);
    }
}


auto AStar::openStatesNearState(const State &state) -> void
{
    for (size_t y = 0; y < field_height; y++)
    {
        for (size_t x = 0; x < field_width; x++)
        {
            if (state.testValue(x, y))
            {
                openStatesForCell(state, x, y);
            }
        }
    }
}


auto AStar::openStatesForCell(const State &state, size_t x, size_t y) -> void
{
    if (x > 0 && !state.testValue(x - 1, y) && !testRestricted(x - 1, y))
    {
        openState(state, x, y, x - 1, y);
    }

    if (x > 1 && state.testValue(x - 1, y) && !state.testValue(x - 2, y) && !testRestricted(x - 2, y))
    {
        openState(state, x, y, x - 2, y);
    }

    if (x < field_width - 1 && !state.testValue(x + 1, y) && !testRestricted(x + 1, y))
    {
        openState(state, x, y, x + 1, y);
    }

    if (x < field_width - 2 && state.testValue(x + 1, y) && !state.testValue(x + 2, y) && !testRestricted(x + 2, y))
    {
        openState(state, x, y, x + 2, y);
    }

    if (y > 0 && !state.testValue(x, y - 1) && !testRestricted(x, y - 1))
    {
        openState(state, x, y, x, y - 1);
    }

    if (y > 1 && state.testValue(x, y - 1) && !state.testValue(x, y - 2) && !testRestricted(x, y - 2))
    {
        openState(state, x, y, x, y - 2);
    }

    if (y < field_height - 1 && !state.testValue(x, y + 1) && !testRestricted(x, y + 1))
    {
        openState(state, x, y, x, y + 1);
    }

    if (y < field_height - 2 && state.testValue(x, y + 1) && !state.testValue(x, y + 2) && !testRestricted(x, y + 2))
    {
        openState(state, x, y, x, y + 2);
    }
}


auto AStar::openState(const State &parent_state, size_t old_x, size_t old_y, size_t new_x, size_t new_y) -> void
{
    State new_state(parent_state.getField());
    new_state.resetValue(old_x, old_y);
    new_state.setValue(new_x, new_y);
    new_state.setCost(parent_state.getCost() + 1);
    new_state.setParentUID(parent_state.getUID());

    if (m_closed.find(new_state) == m_closed.end())
    {
        m_opened.insert(new_state);
    }
}


auto AStar::getBestOpenState() -> const State&
{
    auto best_state = std::ref(*m_opened.begin());
    size_t best_value = best_state.get().getCost() + best_state.get().distance(m_target_state);

    for (const auto& state : m_opened)
    {
        size_t state_value = state.getCost() + state.distance(m_target_state);
        if (state_value < best_value)
        {
            best_state = std::ref(state);
            best_value = state_value;
        }
    }

    return best_state.get();
}



auto operator<<( std::ostream& os, const AStar& a_star ) -> std::ostream&
{
    os << "\033[1;4m<< AStar run stats >>\033[22;24m" << std::endl;
    os << "Number of opened states: " << a_star.m_opened.size() << std::endl;
    os << "Number of closed states: " << a_star.m_closed.size() << std::endl;
    return os;
}
