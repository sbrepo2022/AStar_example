#include "state.h"
#include "helpers.h"

#include <limits>
#include <numeric>
#include <sstream>


size_t State::current_uid = 1;


State::State( const field_bits &field )
    : m_field(field)
{
    m_uid = current_uid++;
}


State::State( std::span<std::string> init_list )
    : m_field(std::accumulate(init_list.begin(), init_list.end(), std::string()))
{
    m_uid = current_uid++;
}


auto State::operator==( const State &other_state ) const -> bool
{
    return m_field == other_state.m_field;
}


auto State::setValue( size_t x, size_t y, bool value ) -> State&
{
    m_field.set(x + field_width * y, value);
    return *this;
}


auto State::resetValue( size_t x, size_t y ) -> State&
{
    m_field.reset(x + field_width * y);
    return *this;
}


auto State::testValue( size_t x, size_t y ) const -> bool
{
    return m_field.test(x + field_width * y);
}


auto State::getField() const -> const field_bits&
{
    return m_field;
}


auto operator<<( std::ostream& os, const State& state ) -> std::ostream&
{
    os << "\033[1;4m<< State >>\033[22;24m" << std::endl;
    os << "UID: " << state.m_uid << std::endl;
    os << "Parent UID: " << state.m_parent_uid << std::endl;
    os << "Cost: " << state.m_cost << std::endl;
    os << "Field: ▼" << std::endl;
    for (size_t y = field_height; y > 0; y--)
    {
        for (size_t x = field_width; x > 0; x--)
        {
            os << (state.testValue(x - 1, y - 1) ? "\033[31;42m☻\033[0m" : "\033[31;42m·\033[0m");
        }
        os << std::endl;
    }
    return os;
}


auto State::getUID() const noexcept -> size_t
{
    return m_uid;
}


auto State::setCost(size_t cost) noexcept -> void
{
    m_cost = cost;
}


auto State::getCost() const noexcept -> size_t
{
    return m_cost;
}


auto State::setParentUID(size_t parent_uid) noexcept -> void
{
    m_parent_uid = parent_uid;
}


auto State::getParentUID() const noexcept -> size_t
{
    return m_parent_uid;
}


[[nodiscard]] auto State::distance( const State &other_state ) const noexcept -> size_t
{
    size_t result_dist = 0;

    State this_state_copy = *this;
    State other_state_copy = other_state;

    for (size_t this_y = 0; this_y < field_height; this_y++)
    {
        for (size_t this_x = 0; this_x < field_width; this_x++)
        {
            if (this_state_copy.testValue(this_x, this_y))
            {
                size_t other_x, other_y;
                std::tie(other_x, other_y) = *other_state_copy.nearestTo(this_x, this_y);

                this_state_copy.resetValue(this_x, this_y);
                other_state_copy.resetValue(other_x, other_y);

                std::array this_p = {this_x, this_y};
                std::array other_p = {other_x, other_y};
                result_dist += manhattan_dist(std::span(this_p), std::span(other_p));
            }
        }
    }
    return result_dist;
}


[[nodiscard]] auto State::nearestTo( size_t x, size_t y ) const noexcept -> std::optional<std::tuple<size_t, size_t>>
{
    size_t lowest_dist = std::numeric_limits<size_t>::max();
    size_t lowest_x = 0;
    size_t lowest_y = 0;

    std::optional<std::tuple<size_t, size_t>> result = std::nullopt;

    for (size_t p_y = 0; p_y < field_height; p_y++)
    {
        for (size_t p_x = 0; p_x < field_width; p_x++)
        {
            if (! testValue(p_x, p_y)) continue;

            std::array p1 = {p_x, p_y};
            std::array p2 = {x, y};
            if (size_t new_dist = manhattan_dist(std::span(p1), std::span(p2)); new_dist < lowest_dist)
            {
                lowest_dist = new_dist;
                lowest_x = p_x;
                lowest_y = p_y;
                result = {lowest_x, lowest_y};
            }
        }
    }

    return result;
}
