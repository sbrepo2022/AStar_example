#include <iostream>
#include <array>
#include <thread>
#include <chrono>

#include "state.h"
#include "helpers.h"
#include "a_star.h"

auto showAnimation(const AStar &a_star, const std::vector<State> &states_sequence, size_t cell_width = 4, size_t cell_height = 3, size_t frame_duration_in_ms = 100) -> void;

int main(int argc, char *argv[])
{
    std::array start_state_init = {
        std::string("11111000"),
        std::string("11100000"),
        std::string("11000000"),
        std::string("10000000"),
        std::string("10000000"),
        std::string("00000000"),
        std::string("00000000"),
        std::string("00000000")
    };
    State start_state(start_state_init);
    std::cout << start_state << std::endl;

    std::array target_state_init = {
        std::string("00000000"),
        std::string("00000000"),
        std::string("00000000"),
        std::string("00000001"),
        std::string("00000001"),
        std::string("00000011"),
        std::string("00000111"),
        std::string("00011111")
    };
    State target_state(target_state_init);
    std::cout << target_state << std::endl;

    std::array restricted_cells_init = {
        std::string("00000000"),
        std::string("00000000"),
        std::string("00000100"),
        std::string("00000000"),
        std::string("00010000"),
        std::string("00001000"),
        std::string("01000000"),
        std::string("00000000")
    };
    AStar a_star(restricted_cells_init, start_state, target_state);

    std::cout << "Distance between start and end states (before start): " << start_state.distance(target_state) << std::endl;

    std::cout << std::endl << "Calculation A-star starts..." << std::endl;
    auto start_1 = std::chrono::high_resolution_clock::now();
    a_star.solve();
    auto end_1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_1 = end_1 - start_1;
    std::cout << "Calculation A-star ends in " << duration_1.count() << " seconds!" << std::endl;
    std::cout << a_star << std::endl;

    std::cout << std::endl << "Calculating states sequence starts..." << std::endl;
    auto start_2 = std::chrono::high_resolution_clock::now();
    std::vector<State> states_sequence = a_star.getStatesSequence();
    auto end_2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_2 = end_2 - start_2;
    std::cout << "Calculation states sequence ends in " << duration_2.count() << " seconds!" << std::endl;
    std::cout << "States sequence length: " <<  states_sequence.size() << std::endl << std::endl;
    std::cout << "Press any button to show animation ...";
    std::cin.get();

    showAnimation(a_star, states_sequence, 4, 3, 100);

    return 0;
}

auto showAnimation(const AStar &a_star, const std::vector<State> &states_sequence, size_t cell_width, size_t cell_height, size_t frame_duration_in_ms) -> void
{
    const field_bits& restricted_cells = a_star.getRestrictedCells();
    for (auto& state : states_sequence)
    {
        system("clear");
        const field_bits& field = state.getField();

        for (size_t y = field_height; y > 0; y--)
        {
            for (size_t cell_y = 0; cell_y < cell_height; cell_y++)
            {
                for (size_t x = field_width; x > 0; x--)
                {
                    for (size_t cell_x = 0; cell_x < cell_width; cell_x++)
                    {
                        /* Клетка закрыта */
                        if (restricted_cells.test((x - 1) + (y - 1) * field_width))
                        {
                            std::cout << "\033[31;42m╬\033[0m";
                        }
                        /* Клетка открыта */
                        else
                        {
                            /* Есть фишка */
                            if (field.test((x - 1) + (y - 1) * field_width))
                            {
                                std::cout << "\033[31m█\033[0m";
                            }
                            /* Нет фишки */
                            else
                            {
                                std::cout << "\033[31;42m \033[0m";
                            }
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration_in_ms));
    }
}