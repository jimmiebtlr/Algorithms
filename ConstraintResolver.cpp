/* 
 * File:   ConstraintResolver.cpp
 * Author: jimmie
 * 
 * Created on April 25, 2012, 12:37 PM
 */

#include "ConstraintResolver.h"
#include "Pair.h"


using namespace std;

/*
 * API for instantiating without Heuristict Generator present 
 */
ConstraintResolver::ConstraintResolver(vector<Point> points, unsigned short n, unsigned short k, vector< Pair > pairs)
{
    vector< deque< unsigned short > > heuristicStart;

    heuristicStart.resize(k);
    // Place all points in container 0 to start
    for (int i = 0; i < points.size(); i++)
    {
        heuristicStart[0].push_back(i);
    }

    init(points, heuristicStart, n, k, pairs);
}

/*
 * API for instantiating with Heuristict Generator present 
 */
ConstraintResolver::ConstraintResolver(vector<Point> points, vector< deque< unsigned short > > heuristicStart, unsigned short n, unsigned short k, vector< Pair > pairs)
{
    init(points, heuristicStart, n, k, pairs);
}

/*
 * sets all the needed variables, and their needed sizes 
 */
void ConstraintResolver::init(vector<Point> points, vector< deque< unsigned short > > heuristicStart, unsigned short n, unsigned short k, vector< Pair > pairs)
{
    verbosity = 0;
    max_phase = 2;
    current_phase = 0;
    maxDepth = 2;
    this->points = points;
    this->pairs = pairs;
    this->n = n;
    this->k = k;
    whichContainer.resize(n);
    start_time = clock();

    currentContainerState = heuristicStart;
    current_constraint = 0;

    for (unsigned short i = 0; i < currentContainerState.size(); i++)
    {
        for (unsigned short j = 0; j < currentContainerState.at(i).size(); j++)
        {
            whichContainer.at(currentContainerState.at(i).at(j)) = i;
        }
    }

    pastConstraints.resize(n);
    for (int i = 0; i < n; i++)
    {
        pastConstraints.at(i).resize(n);
        for (int j = 0; j < n; j++)
        {
            pastConstraints.at(i).at(j) = 0;
        }
    }
}

/*
 * Solve the system
 */
void ConstraintResolver::solve()
{
    if (verbosity >= 1)
    {
        cout << "Phase " << current_phase << " starting." << endl;
    }

    while (current_constraint < pairs.size())
    {
        if (deadLineReached())
        {
            break;
        }

        // Placing here instead of later prevents moving both in DFS to a different same container
        pastConstraints.at(pairs.at(current_constraint).pt1).at(pairs.at(current_constraint).pt2) = 1;
        pastConstraints.at(pairs.at(current_constraint).pt2).at(pairs.at(current_constraint).pt1) = 1;

        vector<bool> moved;
        moved.resize(n);
        for (int i = 0; i < n; i++)
        {
            moved.at(i) = 0;
        }

        bool resolved = 0;

        if (whichContainer.at(pairs.at(current_constraint).pt1) == whichContainer.at(pairs.at(current_constraint).pt2))
        {
            for (int i = 0; i < k; i++)
            {
                if (i != whichContainer.at(pairs.at(current_constraint).pt1))
                {
                    Move move(pairs.at(current_constraint).pt1, i, whichContainer.at(pairs.at(current_constraint).pt1));
                    deque<Move> tmp;
                    tmp.push_front(move);
                    if (resolveNextConstraint(tmp, moved, maxDepth))
                    {
                        resolved = 1;
                        break;
                    }
                    else
                    {
                        Move move(pairs.at(current_constraint).pt2, i, whichContainer.at(pairs.at(current_constraint).pt2));
                        deque<Move> tmp;
                        tmp.push_front(move);
                        if (resolveNextConstraint(tmp, moved, maxDepth))
                        {
                            resolved = 1;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            resolved = 1;
        }

        if (determineConstraintResMode(resolved))
        {
            break;
        }

    }
}

/*
 * Handles switching between phases, and search depths
 */
bool ConstraintResolver::determineConstraintResMode(bool resolved)
{
    // If a resolution to the constraint wasn't found using current_phase and depth
    if (!resolved)
    {
        cout << "Checking for solution" << endl;
        if (isSolution())
        {
            cout << "Found the solution." << endl;
            return 1;
        }
        
        if (current_phase == 0)
        {
            if (verbosity >= 1)
            {
                cout << "Phase " << current_phase << " complete." << endl;
            }
            if (current_phase == max_phase)
            {
                return 1;
            }
            current_phase++;
        }
        else if (current_phase == 1)
        {
            if (maxDepth > n)
            {
                if (current_phase == max_phase)
                {
                    cout << "Max phase was 2. Now I'm done." << endl;
                    return 1;
                }
                if (verbosity >= 1)
                {
                    cout << "Phase " << current_phase << " complete." << endl;
                }
                current_phase++;
                max_phase = 2;
            }
            maxDepth = maxDepth * 2;
            if (verbosity >= 1)
            {
                cout << "Increasing search depth to " << maxDepth << endl;
            }


        }
        else if (current_phase == 2)
        {
            if (maxDepth > n)
            {
                return 1;
            }

            maxDepth = maxDepth * 2;
            if (verbosity >= 1)
            {
                cout << "Increasing search depth to " << maxDepth << endl;
            }
        }

        if (verbosity >= 1)
        {
            cout << "Phase " << current_phase << " starting at constraint " << current_constraint << endl;
        }
    }
    else // if a resolution was found
    {
        current_phase = 0;
        current_constraint++;
    }
    return false;
}

/*
 * Check if the deadline is reached
 */
bool ConstraintResolver::deadLineReached()
{
    if (clock() - start_time >= run_time)
    {
        return true;
    }
    return false;
}

/*
 * Resolve as many constraints as possible
 */
bool ConstraintResolver::resolveNextConstraint(deque<Move>& moves, vector< bool >& moved, unsigned short allowed_moves)
{
    if (deadLineReached())
    {
        return false;
    }

    applyMoves(moves, moved);

    vector< unsigned short > new_conflicts;

    findConflicts(new_conflicts, moves);

    // if it didn't run through each option which would resolve the constraints
    // cannot currently move multiple so

    if (new_conflicts.size() == 0)
    {
        return 1;
    }
    else if ((allowed_moves <= 0) && (current_phase > 0))
    {
        rollbackMoves(moves, moved);
        return false;
    }
    else
    {
        bool resolved = 0;
        deque< deque<Move> > enumerated_moves;

        if (enumerateMoves(enumerated_moves, moved, new_conflicts))
        {
            for (int j = 0; j < enumerated_moves.size(); j++)
            {
                if (resolveNextConstraint(enumerated_moves.at(j), moved, allowed_moves - 1))
                {
                    resolved = 1;
                    break;
                }
            }

            if (resolved)
            {
                return true;
            }

        }

        rollbackMoves(moves, moved);

        return false;
    }
}

/*
 * Find conflicts resulting from moves and previous resolved constraints
 */
void ConstraintResolver::findConflicts(vector< unsigned short >& new_conflicts, deque<Move>& moves)
{
    // for each move
    for (int j = 0; j < moves.size(); j++)
    {

        // for each element in the new container
        for (int i = 0; i < currentContainerState.at(moves.at(j).moved_to).size(); i++)
        {
            if (pastConstraints.at(moves.at(j).element_moved).at(currentContainerState.at(moves.at(j).moved_to).at(i)) == 1)
            {
                bool already_has_it = 0;

                for (int k = 0; k < new_conflicts.size(); k++)
                {
                    if (new_conflicts.at(k) == currentContainerState.at(moves.at(j).moved_to).at(i))
                    {
                        already_has_it = 1;
                    }
                }
                if (!already_has_it)
                {
                    new_conflicts.push_back(currentContainerState.at(moves.at(j).moved_to).at(i));
                }
            }
        }
    }
}

/*
 * Enumerate all possible move sets ( depends on the phase)
 */
bool ConstraintResolver::enumerateMoves(deque< deque<Move> >& moves, vector< bool >& moved, vector< unsigned short >& new_conflicts)
{
    // if any of the new conflicts has already been moved then a solution is not along this branch of the tree
    for (int i = 0; i < new_conflicts.size(); i++)
    {
        if (moved.at(new_conflicts.at(i)) == 1)
        {
            return 0;
        }
    }

    if (current_phase < 1) // Phase 0
    {
        if (new_conflicts.size() != 1)
        {
            return 0;
        }

        // for each container
        for (int i = 0; i < k; i++)
        {
            if (whichContainer.at(new_conflicts.at(0)) != i)
            {
                Move move(new_conflicts.at(0), i, whichContainer.at(new_conflicts.at(0)));
                deque<Move> tmp;
                tmp.push_front(move);
                moves.push_front(tmp);
            }
        }
        return 1;
    }
    else if (current_phase < 2) // Phase 1
    {
        // for each container
        for (int i = 0; i < k; i++)
        {
            if (whichContainer.at(new_conflicts.at(0)) != i)
            {
                deque<Move> tmp;
                // for each conflict add the move to the list of moves
                for (int k = 0; k < new_conflicts.size(); k++)
                {
                    Move move(new_conflicts.at(k), i, whichContainer.at(new_conflicts.at(k)));
                    tmp.push_front(move);
                }
                moves.push_front(tmp);
            }
        }

        return 1;
    }
    else if (current_phase < 3) // Phase 1
    {
        // for each move create a 

        return 0;
    }

}

/*
 * Apply the moves to the system 
 */
void ConstraintResolver::applyMoves(deque<Move>& moves, vector< bool >& moved)
{
    // for each move
    for (int j = 0; j < moves.size(); j++)
    {
        // Update containers
        for (int i = 0; i < currentContainerState.at(moves.at(j).moved_from).size(); i++)
        {

            if (currentContainerState.at(moves.at(j).moved_from).at(i) == moves.at(j).element_moved)
            {
                currentContainerState.at(moves.at(j).moved_from).erase(currentContainerState.at(moves.at(j).moved_from).begin() + i);
            }
        }

        currentContainerState.at(moves.at(j).moved_to).push_front(moves.at(j).element_moved);

        // Update bool array moved
        moved.at(moves.at(j).element_moved) = 1;

        // Update whereAt
        whichContainer.at(moves.at(j).element_moved) = moves.at(j).moved_to;
    }
}

/*
 * Rollback the moves from the system
 */
void ConstraintResolver::rollbackMoves(deque<Move>& moves, vector< bool >& moved)
{
    // for each move
    for (int j = 0; j < moves.size(); j++)
    {
        // need to roll back any changes here
        // Update containers
        for (int i = 0; i < currentContainerState.at(moves.at(j).moved_to).size(); i++)
        {
            if (currentContainerState.at(moves.at(j).moved_to).at(i) == moves.at(j).element_moved)
            {
                currentContainerState.at(moves.at(j).moved_to).erase(currentContainerState.at(moves.at(j).moved_to).begin() + i);
            }
        }
        currentContainerState.at(moves.at(j).moved_from).push_front(moves.at(j).element_moved);

        // Update bool array moved
        moved.at(moves.at(j).element_moved) = 0;

        // Update whereAt
        whichContainer.at(moves.at(j).element_moved) = moves.at(j).moved_from;
    }
}

/*
 * Checks for no forward progress remaining possible (solution found)
 * Doesn't always activate but never generates false posatives
 */
bool ConstraintResolver::isSolution()
{
    vector< int > cliquePool;
    vector < int> clique;

    // See what constraints both pair.pt1 and pair.pt2 share
    for (int i = 0; i < pastConstraints.size(); i++)
    {
        if (pastConstraints[pairs[current_constraint].pt1]
                == pastConstraints[pairs[current_constraint].pt2]
                == 1)
        {
            cliquePool.push_back(i);
        }
    }

    return findClique(cliquePool, clique);
}

/*
 * Recursivly find cliques
 * clique contain indexs in cliquePool
 */
bool ConstraintResolver::findClique(vector< int >& cliquePool, vector< int >& clique)
{
    int start;
    if( clique.size() > 0 )
    {
        start = clique.back() +1;
    }
    else
    {
        start = 0;
    }
    // If there are less left to look at then are needed, its automatically false
    for (int i = start; i > k - clique.size() - 1; i--)
    {
        if (cliqueMember(i, clique, cliquePool))
        {
            clique.push_back(i);

            // If a clique was found of size k-1 the solution has been found
            // If not go to next recursion level
            if (clique.size() == k - 1)
            {
                return true;
            }
            else
            {
                if (findClique(cliquePool, clique))
                {
                    return true;
                }
                else
                {
                    // roll back change and continue for loop
                    clique.pop_back();
                }
            }
        }
    }

    // exahausted search
    return false;
}

/*
 * Determines if newMember is actually a valid addition to the clique
 */
bool ConstraintResolver::cliqueMember(int newMember, vector< int >& clique, vector< int >& cliquePool)
{
    for (int i = 0; i < clique.size(); i++)
    {
        if (!pastConstraints.at(cliquePool.at(newMember)).at(cliquePool.at(clique.at(i))))
        {
            return false;
        }
    }
    return true;
}

/*
 * Convert the point to whatever output mode was requested, Point or index
 */
string ConstraintResolver::outputModePoint(unsigned short ptIndex)
{
    stringstream aString;

    if (output_mode == 0)
    {
        aString << ptIndex;
    }
    else
    {
        aString << points.at(ptIndex).showPoint();
    }

    return aString.str();
}

/*
 * Convert the pair to whatever output mode was requested, Point or index
 */
string ConstraintResolver::outputModePair(unsigned short ptIndex)
{
    stringstream aString;

    if (output_mode == 0)
    {
        aString << pairs.at(current_constraint).pt1 << " " << pairs.at(current_constraint).pt2;
    }
    else
    {
        aString << points.at(pairs.at(current_constraint).pt1).showPoint() << " " << points.at(pairs.at(current_constraint).pt1).showPoint();
    }

    return aString.str();
}

/*
 * Print the solution
 */
void ConstraintResolver::printSolution()
{
    cout << "Min of maxes is: " << sqrt(pairs.at(current_constraint).cost) << endl;
    cout << current_constraint << " constraints resolved." << endl;

    if (verbosity == 1)
    {
        int size = 0;
        for (int i = 0; i < pastConstraints.at(pairs.at(current_constraint).pt1).size(); i++)
        {
            if (pastConstraints.at(pairs.at(current_constraint).pt1).at(i) == 1)
            {
                size++;
            }
        }
        cout << "Point " << outputModePoint(pairs.at(current_constraint).pt1) << " had " << size << " constraints" << endl;

        size = 0;
        for (int i = 0; i < pastConstraints.at(pairs.at(current_constraint).pt2).size(); i++)
        {
            if (pastConstraints.at(pairs.at(current_constraint).pt1).at(i) == 1)
            {
                size++;
            }
        }
        cout << "Point " << outputModePoint(pairs.at(current_constraint).pt2) << " had " << size << " constraints" << endl;

    }

    if (verbosity == 2)
    {

        int size = 0;
        for (int i = 0; i < pastConstraints.at(pairs.at(current_constraint).pt1).size(); i++)
        {
            if (pastConstraints.at(pairs.at(current_constraint).pt1).at(i) == 1)
            {
                size++;
            }
        }
        cout << "Point " << outputModePoint(pairs.at(current_constraint).pt1) << " had the following " << size << " constraints" << endl;

        for (int i = 0; i < pastConstraints.at(pairs.at(current_constraint).pt1).size(); i++)
        {
            if (pastConstraints.at(pairs.at(current_constraint).pt1).at(i) == 1)
            {
                cout << outputModePoint(i) << endl;
            }
        }


        size = 0;
        for (int i = 0; i < n; i++)
        {
            if (pastConstraints.at(pairs.at(current_constraint).pt2).at(i) == 1)
            {
                size++;
            }
        }
        cout << "Point " << outputModePoint(pairs.at(current_constraint).pt2) << " had the following " << size << " constraints" << endl;

        for (int i = 0; i < pastConstraints.at(pairs.at(current_constraint).pt2).size(); i++)
        {
            if (pastConstraints.at(pairs.at(current_constraint).pt2).at(i) == 1)
            {
                cout << outputModePoint(i) << endl;
            }
        }
    }

    if (verbosity >= 1)
    {
        for (int i = 0; i < currentContainerState.size(); i++)
        {
            cout << "Container " << i << " contians: " << endl;
            cout << "And is size: " << currentContainerState.at(i).size() << endl;
            for (int j = 0; j < currentContainerState.at(i).size(); j++)
            {
                if (output_mode == 0)
                {
                    cout << currentContainerState.at(i).at(j) << endl;
                }
                else
                {
                    cout << points.at(currentContainerState.at(i).at(j)).showPoint() << endl;

                }
            }
        }
    }
}

void ConstraintResolver::setVerbosity(int v)
{
    verbosity = v;
}

void ConstraintResolver::setMaxPhase(int phase)
{
    this->max_phase = phase;
}

void ConstraintResolver::setRunTime(int r_time)
{
    run_time = r_time * SECONDS_PER_MINUTE * CLOCKS_PER_SEC;
}

void ConstraintResolver::setRunTime(double r_time)
{
    run_time = r_time * SECONDS_PER_MINUTE * CLOCKS_PER_SEC;
}

void ConstraintResolver::setOutputMode(OutputMode mode)
{
    output_mode = mode;
}