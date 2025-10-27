#ifndef CASCADE_GRID_CELLSTATE_HPP
#define CASCADE_GRID_CELLSTATE_HPP

#include <string>

namespace cascade
{
    enum class CellState
    {
        Unburned,    ///< Cell has not been affected by fire
        Igniting,    ///< Cell is heating up, about to catch fire
        Burning,     ///< Cell is actively burning
        Smouldering, ///< cell is burning with reduced intensity
        Burned,      ///< Cell has complemetly burnt out
        Wet,         ///< Cell has been treated with water (fire resistanct)
        Supressed,   ///< Cell has been completely extinguished
    };

    /**
     * @brief Convert CellState to string
     * @param state The cell state to convert
     * @return String representation of state
     */
    inline std::string cellStateToString(CellState state)
    {
        switch (state)
        {
        case CellState::Unburned:
            return "Unburned";
        case CellState::Igniting:
            return "Igniting";
        case CellState::Burning:
            return "Burning";
        case CellState::Smouldering:
            return "Smouldering";
        case CellState::Burned:
            return "Burned";
        case CellState::Wet:
            return "Wet";
        case CellState::Supressed:
            return "Supressed";
        default:
            return "Unknown";
        }
    }

    inline char cellStateToChar(CellState state)
    {
        switch (state)
        {
        case CellState::Unburned:
            return '.';
        case CellState::Igniting:
            return 'i';
        case CellState::Burning:
            return 'F';
        case CellState::Smouldering:
            return 's';
        case CellState::Burned:
            return 'X';
        case CellState::Wet:
            return '~';
        case CellState::Supressed:
            return '-';
        default:
            return '?';
        }
    }

    /**
     * @brief Check if a cell state represents an active file
     * @param state The cell state to check
     * @return true if the cell is actively on 🔥🔥fire🔥🔥
     */
    inline bool isBurning(CellState state)
    {
        return state == CellState::Igniting || state == CellState::Burning || state == CellState::Smouldering;
    }

    /**
     * @brief Check if a cell can ignite
     * @param state The cell state to check
     * @return true if cell can catch fire
     */
    inline bool canIgnite(CellState state) {
        return state == CellState::Unburned;
    }

    /**
     * @brief Check if a cell is fire-resistant
     * @param state The cell state to check
     * @return true if cell resists ignition, false otherwise
     */
    inline bool isFireResistance(CellState state) {
        return state == CellState::Wet || state == CellState::Supressed || state == CellState::Burned;
    }
}

#endif