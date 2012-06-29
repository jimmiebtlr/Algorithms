/* 
 * File:   PointsReader.cpp
 * Author: jimmie
 * 
 * Created on April 24, 2012, 2:51 PM
 */

#include "PointsReader.h"

using namespace std;

// // // // //
// getInput(string) : vector of filled points
//      - captures setup parameters, assigns to globals
//      - fills n points with (x,y,z) coordinates
//
void PointsReader::getInput(string fName, vector<Point>& points) {
    
    ifstream theFile(fName.c_str());
    if (not theFile.good()) {
        cerr << "Problem opening input file" << endl;
        exit(1);
    }
    
    // set global parameters from file
    theFile >> n;
    theFile >> k;
    theFile >> mm;
    
    // fill the points
    int i=0;
    
    while (theFile.good()) {
        if (i >= n) break;
        
        Point aPoint = Point();
        theFile >> aPoint.x >> aPoint.y >> aPoint.z;
        points.push_back(aPoint);
        i++;
    }
}

