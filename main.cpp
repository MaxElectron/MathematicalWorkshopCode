#include <iostream>
#include <vector>

const int RADIUS = 3;
const std::vector<std::pair<int, int>> START_GAME_STATE = {std::make_pair(0, 0), std::make_pair(0, -3), std::make_pair(-3, -3), std::make_pair(-3, 0), std::make_pair(-3, 1), std::make_pair(0, 1)};
u_int64_t nodes_checked = 0;

// from https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
int ccw(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C)
{
    int val = (C.second-A.second) * (B.first-A.first) - (B.second-A.second) * (C.first-A.first);
    if (!val)
    {
        return 0;
    }
    if (val > 0)
    {
        return 1;
    }
    return -1;
}

// check if C is on AB
bool on_segment(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C)
{
    return ccw(A, B, C) == 0 && C.first <= std::max(A.first, B.first) && C.first >= std::min(A.first, B.first) && C.second <= std::max(A.second, B.second) && C.second >= std::min(A.second, B.second);
}

// check if AB and CD intersect 
// from https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
// and https://www.tutorialspoint.com/Check-if-two-line-segments-intersect
[[nodiscard]] bool is_intersecting (std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C, std::pair<int, int> D)
{
    if (ccw(A,C,D) != ccw(B,C,D) && ccw(A,B,C) != ccw(A,B,D))
    {
        return true;
    }
    if (on_segment(A, B, C)) 
    {
        return true;
    }
    if (on_segment(A, B, D)) 
    {
        return true;
    }
    if (on_segment(C, D, A)) 
    {
        return true;
    }
    if (on_segment(C, D, B)) 
    {
        return true;
    }
    return false;
}

bool is_winning_pos (std::vector<std::pair<int, int>> starting_gamestate)
{
    // check for available moves
    std::vector<std::pair<int, int>> available_moves;

    for (int i = -RADIUS; i <= RADIUS; ++i)
    {
        for (int j = -RADIUS; j <= RADIUS; ++j)
        {
            //check the point (i, j) as next move
            bool flag = false;

            // if no move
            if (std::make_pair(i, j) == starting_gamestate[starting_gamestate.size() - 1])
            {
                continue;
            }

            // if included in path already - it is not suitable
            for (int k = 0; k < starting_gamestate.size() - 1; ++k)
            {
                if (on_segment(starting_gamestate[k], starting_gamestate[k + 1], std::make_pair(i, j)))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                continue;
            }

            // if includes part of already - it is not suitable
            for (int k = 0; k < starting_gamestate.size() - 1; ++k)
            {
                if (on_segment(std::make_pair(i, j), starting_gamestate[starting_gamestate.size() - 1], starting_gamestate[k]))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                continue;
            }

            // if last segment intersects any of the previous - it is not suitable
            for (int k = 0; k + 2 < starting_gamestate.size(); ++k)
            {
                if (is_intersecting(starting_gamestate[k], starting_gamestate[k + 1], starting_gamestate[starting_gamestate.size() - 1], std::make_pair(i, j)))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                continue;
            }

            // if not against the rules - add to the list 
            available_moves.emplace_back(i, j);
        }
    }
    
    // debug
    nodes_checked++;
    std::cout << "depth: " << starting_gamestate.size() << "  recursion: " << available_moves.size() << "  states checked: " << nodes_checked << "  branch: ";
    for (int i = 0; i < starting_gamestate.size(); ++i)
    {
         std::cout << starting_gamestate[i].first << ' ' << starting_gamestate[i].second << "  ";
    }
    std::cout << '\n';

    // check the following states
    for (std::pair<int, int> state : available_moves)
    {
        starting_gamestate.push_back(state);
        if (!is_winning_pos(starting_gamestate))
        {
            return true;
        }
        starting_gamestate.pop_back();
    }

    // if none were losing, then the current is
    return false;
}

int main()
{
    // zero - second wins
    // one - first wins
    std::cout << is_winning_pos(START_GAME_STATE) << '\n';
}