/* 
 * File:   main.h
 * Author: jimmie
 *
 * Created on April 24, 2012, 9:38 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <cmath>        // pow function
#include <algorithm>    // sort function
#include <cstdlib>      // abs function
#include "Point.h"
#include "PairEnumerator.h"
#include "ConstraintResolver.h"
#include "Spread.h"

const std::string DEF_FILE = "Maps/input1.txt";

// Function Prototypes
std::string procCmdLine(int argc, const char** argv);    // looks for input file and "-"

// todo use templates instead
void printVector( std::vector<Point> points );          // prints a vector of points( for debugging)     
void showPairs(std::vector<Pair> pairs, std::vector<Point> points);

#endif	/* MAIN_H */
