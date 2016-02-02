#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <cassert>
#include <Windows.h>

// This presents a hard coded problem
// rather than requesting input
//#define SOLVE_GHAWAR_16

using Board = std::vector<std::string>;
using Position = std::pair<size_t, size_t>;

#define check(expression, error_str) do { if (!(expression)) { std::cerr << error_str << std::endl; terminate(); } } while (false)

#ifdef SOLVE_GHAWAR_16

// First problem from the game that I got really stuck on.
// It's was from section "Ghawar" and was puzzle 16 in that section.
//
// The solution is GYCYBYBGMCR
//
// Not sure why I couldn't find this after days of looking :)

void ReadBoard(Board &board)
{
    board = {
        "   MMRRGGC",
        "  BMBBYRCC",
        "RRBCCYYRGM",
        "CMRYGGYGGY",
        "MMYY MMBBY",
        "GGBBB YYCC",
        "BYYMBMMGCC",
        "BBMMYYRGGC",
        "BCCGYGRM  ",
        "CGGGYGR   ",
    };
}

void ReadPosition(const Board &board, Position &pos)
{
    pos = Position(2, 9);
}

void ReadMaxMoves(size_t &max_moves)
{
    max_moves = 11;
}

#else // Read input from keyboard

void ReadBoard(Board &board)
{
    check(board.empty(), "The board must be empty before it can be read");

    std::cout << "Enter the horizontal rows of the board, one per line" << std::endl;
    std::cout << "   <space>=blank, b=blue, r=red, g=green, y=yellow, c=cyan, m=magenta" << std::endl;
    std::cout << "   Enter a blank line to finish" << std::endl;

    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        if (line.empty())
            break;

        // Verify the line only contains correct colors,
        // and convert to upper case

        bool valid = true;

        for (char &ch : line)
        {
            ch = toupper(ch);

            switch (ch)
            {
            case ' ':
            case 'B':
            case 'R':
            case 'G':
            case 'Y':
            case 'C':
            case 'M':
                break;

            default:
                MessageBeep(MB_ICONASTERISK);
                std::cout << "Invalid character in input" << std::endl;
                valid = false;
            }

            if (!valid)
                break;
        }

        // Verify it's the same length as previous lines

        if (!board.empty()
            && (line.size() != board[0].size()))
        {
            MessageBeep(MB_ICONASTERISK);
            std::cout << "All lines must be the same length" << std::endl;
            valid = false;
        }

        if (valid)
            board.emplace_back(std::move(line));
    }

    check(!board.empty(), "You must enter at least one row");
}

void ReadPosition(const Board &board, Position &pos)
{
    std::cout << "Enter the starting position (row, column), where (0, 0) is the top left corner" << std::endl;
    std::cin >> pos.first >> pos.second;
    std::cin.ignore();

    check((pos.first >= 0) && (pos.first < board.size()), "Invalid row");
    check((pos.second >= 0) && (pos.second < board[0].size()), "Invalid column");

    check(board[pos.first][pos.second] != ' ', "Start positions must be not empty (space)");
}

void ReadMaxMoves(size_t &max_moves)
{
    std::cout << "Enter the maximum number of moves:" << std::endl;
    std::cin >> max_moves;
    std::cin.ignore();

    check(max_moves > 0, "Max moves must be larger than zero");
}

#endif // if !defined(SOLVE_GHAWAR_16)

std::vector<Position> GetPositionsToTest(const Board &board, const Position &test_pos)
{
    std::vector<Position> result;

    if (test_pos.first > 0)
        result.push_back(Position(test_pos.first - 1, test_pos.second));
    if (test_pos.first < (board.size() - 1))
        result.push_back(Position(test_pos.first + 1, test_pos.second));

    if (test_pos.second > 0)
        result.push_back(Position(test_pos.first, test_pos.second - 1));
    if (test_pos.second < (board[0].size() - 1))
        result.push_back(Position(test_pos.first, test_pos.second + 1));

    return result;
}

bool ExpandOneSpace(const Board &board, std::set<Position> &recolorings, char target_color)
{
    bool made_any_moves = false;

    while (true)
    {
        // Find the next set of positions moving one space out

        std::set<Position> new_positions;

        for (const Position &pos : recolorings)
        {
            // For this existing pos in the start color set,
            // find all places one location away

            std::vector<Position> test_positions = GetPositionsToTest(board, pos);

            for (const Position test_pos : test_positions)
            {
                if ((recolorings.find(test_pos) == recolorings.end())
                    && (new_positions.find(test_pos) == new_positions.end()))
                {
                    // It's a new position we've not looked at yet

                    if (board[test_pos.first][test_pos.second] == target_color)
                    {
                        new_positions.insert(test_pos);
                    }
                }
            }
        }

        if (new_positions.empty())
        {
            // We've finished the search
            break;
        }
        else
        {
            made_any_moves = true;

            for (const Position &new_pos : new_positions)
                recolorings.insert(new_pos);
        }
    }

    return made_any_moves;
}

bool IsSolved(const Board &board, char move)
{
    for (const std::string &row : board)
    {
        for (char cell : row)
        {
            if ((cell != move) && (cell != ' '))
            {
                return false;
            }
        }
    }

    return true;
}

std::set<char> FindValidMoves(const Board &board, const char cur_color, const std::set<Position> &cur_positions)
{
    std::set<char> result;

    for (const Position &pos : cur_positions)
    {
        auto apply = [&](size_t row, size_t col)
        {
            if ((row > 0)
                && (row < board.size())
                && (col > 0)
                && (col < board[row].size()))
            {
                char pos_color = board[row][col];

                if ((pos_color != ' ')
                    && (pos_color != cur_color))
                {
                    result.insert(pos_color);
                }
            }
        };

        apply(pos.first - 1, pos.second);
        apply(pos.first + 1, pos.second);
        apply(pos.first, pos.second - 1);
        apply(pos.first, pos.second + 1);
    }

    return result;
}

size_t CountColorsOnBoard(const Board &board)
{
    std::set<char> colors;

    for (const std::string &row : board)
    {
        for (char color : row)
        {
            if (color != ' ')
            {
                colors.insert(color);
            }
        }
    }

    return colors.size();
}

void RecursiveSolve(const Board &board, const size_t max_moves, const char cur_color, const std::set<Position> &cur_positions, std::string &cur_solution, std::string &best_solution)
{
    // Check that searching further won't exceed
    // the max moves

    if ((cur_solution.size() + 1) > max_moves)
        return;

    // Check that we have enough moves remaining
    // to clear the number of colors remaining

    {
        size_t colors_left = CountColorsOnBoard(board);

        size_t moves_left = max_moves - cur_solution.size();

        // At the end we want one move and two colors -
        // so if the number of colors left is larger
        // than (moves_left+1) we can't solve it

        if (colors_left > (moves_left + 1))
        {
            //std::cout << "Rejecting partial solution " << cur_solution << " as "
            //    << moves_left << " moves left but "
            //    << colors_left << " colors remaining" << std::endl;

            return;
        }
    }

    // Work out which valid moves there are - i.e.
    // which colors touch the current position

    std::set<char> valid_moves = FindValidMoves(board, cur_color, cur_positions);

    for (char move : valid_moves)
    {
        // Copy the board and current positions

        Board new_board(board);
        std::set<Position> new_positions(cur_positions);

        // Expand the current positions to include the new move

        while (ExpandOneSpace(new_board, new_positions, move))
        {
            // Keep searching
        }

        // Recolor everything

        for (const Position &recolor_pos : new_positions)
            new_board[recolor_pos.first][recolor_pos.second] = move;

        // Record the move

        cur_solution.push_back(move);

        if (cur_solution.size() <= 5)
        {
            std::cout << "Trying " << cur_solution << "..." << std::endl;
        }

        // See if we've solved the solution, or
        // if now keep searching for new moves

        if (IsSolved(new_board, move))
        {
            std::cout << "   Found solution " << cur_solution << std::endl;

            if (best_solution.empty()
                || (cur_solution.size() < best_solution.size()))
            {
                std::cout << "      Recording as best" << std::endl;

                best_solution = cur_solution;
            }
        }
        else
        {
            RecursiveSolve(new_board, max_moves, move, new_positions, cur_solution, best_solution);
        }

        // Remove our move from the current solution

        cur_solution.pop_back();
    }
}

void SolveBoard(const Board &board, const Position &start_pos, size_t max_moves, std::string &solution)
{
    // First, calculate the first set of positions that match the start color

    char start_color = board[start_pos.first][start_pos.second];

    std::set<Position> positions_matching_start;

    positions_matching_start.insert(start_pos);

    while (ExpandOneSpace(board, positions_matching_start, start_color))
    {
        // Just keep searching...
    }

    // Now, recursively expand the search

    std::string recursive_state;
    
    RecursiveSolve(board, max_moves, start_color, positions_matching_start, recursive_state, solution);
}

int main(int argc, char *argv[])
{
    Board board;

    ReadBoard(board);

    Position start;

    ReadPosition(board, start);

    size_t max_moves;

    ReadMaxMoves(max_moves);

    std::string solution;

    SolveBoard(board, start, max_moves, solution);

    if (solution.empty())
    {
        check(false, "The board has no solution");
    }
    else
    {
        std::cout << "Solution: " << solution << std::endl;
    }

    return 0;
}