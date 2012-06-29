/* 
 * File:   ConstraintResolver.h
 * Author: jimmie
 *
 * Created on April 25, 2012, 12:37 PM
 */

#ifndef CONSTRAINTRESOLVER_H
#define	CONSTRAINTRESOLVER_H

#include <vector>
#include <deque>
#include <ctime>
#include "Point.h"
#include "Pair.h"
#include "Move.h"

using namespace std;

enum OutputMode{ INDEX, POINT };

class ConstraintResolver {
public:
    ConstraintResolver( vector<Point> points, unsigned short n, unsigned short k, vector< Pair > pairs);
    ConstraintResolver( vector<Point> points, vector< deque< unsigned short > > heuristicStart, unsigned short n, unsigned short k, vector< Pair > pairs);

    // Set the max phase reached
    void setMaxPhase(int phase);

    // Solve, taking into account maxPhase
    void solve();    
    
    // Set output variables
    void setVerbosity(int v);
    void setRunTime(int phase);
    void setRunTime(double phase);
    void setOutputMode( OutputMode mode );
    
    // Return the best solution found, not necessarily correct
    vector< deque< unsigned short > > getSolution() const { return currentContainerState; };
    
    // Print it 
    void printSolution();

private:
    // Used in run_time calculation
    
    // Control output variables
    unsigned int verbosity;
    unsigned int output_mode;
    
    // How thorough, default is highest currently supported
    unsigned int max_phase;
    unsigned short maxDepth;
    
    // Control time limitations
    static const unsigned int SECONDS_PER_MINUTE = 60;
    clock_t start_time;
    clock_t run_time;
    bool deadLineReached();

    // Simulation varables
    unsigned short n;
    unsigned short k;
    unsigned short current_constraint;
    unsigned int current_phase;

    // TODO: Change currentContainerState to Vector? HashMap? Map?
    // should start instead be a tree?, or perhaps vector of hashMaps
    vector< deque< unsigned short > > currentContainerState;    
    vector< Pair > pairs;                                                  // Should be sorted before constraint Resolver takes over
    vector< Point > points;                                                // Needed for solution output in points mode
    vector< unsigned short > whichContainer;                               // Contains which container an element is stored in and is n in size
    vector< vector< bool > > pastConstraints;

    void init(vector<Point> points, vector< deque< unsigned short > > heuristicStart, unsigned short n, unsigned short k, vector< Pair > pairs); // Initialize the constraints 

    bool resolveNextConstraint( deque<Move>& moves, vector< bool >& moved, unsigned short allowed_moves);
    bool determineConstraintResMode( bool resolved );
    void findConflicts( vector< unsigned short >& new_conflicts, deque<Move>& moves );
    bool enumerateMoves( deque< deque<Move> >& moves,  vector< bool >& moved, vector< unsigned short >& new_conflicts);
    void applyMoves( deque<Move>& moves, vector< bool >& moved);
    void rollbackMoves( deque<Move>& moves, vector< bool >& moved);
    bool constraintsResolved( deque<Move> moves );
    
    // Checks for solution found
    bool isSolution( vector< vector< bool > >& pastConstraints );
    bool findCliques( vector< int >& cliquePool, vector< int >& clique );
    bool cliqueMember( int newMember, vector< int >& clique );
    
    // printSolutionHelpers
    string outputModePoint( unsigned short ptIndex );
    string outputModePair( unsigned short ptIndex );
};

#endif	/* CONSTRAINTRESOLVER_H */

