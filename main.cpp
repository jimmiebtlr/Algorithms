/* 
 * File:   main.cpp
 * Author: jimmie
 *
 * Created on April 24, 2012, 2:29 PM
 */

#include "main.h"
#include "PointsReader.h"
#include "Pair.h"

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char* argv[])
{
    
    unsigned short n;
    unsigned short k;

    // get the filename from command line, or use the default
    //string fName = procCmdLine(argc, argv);
    string fName = DEF_FILE;
    cout << "File name is: " << fName << endl;

    // set up the reader and the vector to hold its outcome
    PointsReader ptReader;
    vector<Point> points;

    // Call the read in action
    ptReader.getInput(fName, points);
    n = ptReader.getN();
    k = ptReader.getK();
    DistMethod method = ptReader.getMethod();
    
    if( n<= k)
    {
        cout << "Do it yourself!!!!!!!!!" << endl;
        exit(0);
    }

    // create a vector to hold the pairs
    vector<Pair> pairs;

    cout << "Enumerating all pairs: " << endl;
    PairEnumerator pairsEnumerated;
    pairsEnumerated.fillPairs(points, pairs, method);

    cout << "Sorting Pairs" << endl;

    sort(pairs.rbegin(), pairs.rend());

    cout << "Generating Heuristic" << endl;
    // Heruistic grouping 
    vector<deque<unsigned short> > Breaks;
    Spread aSpread(points, pairs, k, n, Breaks);

    //showPairs(pairs, points);
    cout << "Initialize Constraint Solver" << endl;
    ConstraintResolver cs(points, n, k, pairs);
    cs.setVerbosity(1);
    cs.setMaxPhase(1);
    cs.setRunTime(1/4.0);
    cs.setOutputMode( POINT );

    cout << "Solving..." << endl;
    cs.solve();
    cs.printSolution();

    return 0;
}
//
//string procCmdLine(int argc, char* argv) {
//    
//    // parse command line for flags
//    //    
//    stringstream tString;
//    char c;
//
//    for (int i=0; i<argc; i++) {
//        tString << string(argv[i]);
//        tString >> c;
//        
//        if (c == '-') {
//            
//            // shift arguments over
//            argc--;
//            for (int j=i; j<argc; j++) {
//                argv[j] = argv[j+1];
//            }
//            break;
//        }
//        // clear tString for next argument
//        tString.str("");
//    }
//    
//    // parse command line for file names, assume valid input
//    //
//    if (argc == 1) {
//        // no command line arguments
//        return DEF_FILE;
//    }
//    else if (argc == 2) {
//        // one command line argument, input file
//        return string(argv[1]);
//    }
//    else {
//        // too many command line arguments, print a message and use default values
//        cerr << "Too many arguments. Using default values." << endl;
//    }
//
//    return DEF_FILE;
//}

/* 
 * for debugging/ NEEDS DEBUGED
 */
void printVector(vector<Point> points)
{
    cout << "Showing Points:" << endl;
    for (int i = 0; i < points.size(); i++)
    {
        cout << points[i].showPoint() << endl;
    }
    cout << endl;
}

void showPairs(vector<Pair> pairs, vector<Point> points)
{
    cout << "Showing Pairs:" << endl;
    for (unsigned int i = 0; i < pairs.size(); i++)
    {
        cout << "Pair[" << i << "]: " << endl;
        cout << "Cost=" << pairs[i].cost << endl;
        cout << points[pairs[i].pt1].showPoint();
        cout << points[pairs[i].pt2].showPoint();
        cout << endl;
    }
    cout << endl;
}