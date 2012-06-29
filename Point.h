/* 
 * File:   Point.h
 * Author: jimmie
 *
 * Created on April 24, 2012, 3:07 PM
 */

#ifndef POINT_H
#define	POINT_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>      // abs function
#include <cmath>        // pow function
#include <sstream>      // for string stream

// Enumerate distance calculation methods
enum DistMethod { Manhattan, Euclidean };

class Point {
public:
    int x;
    int y;
    int z;
    std::string showPoint();
    long getEuclideanDist( Point pt2 );
    long getManhattanDist( Point pt2 );
    bool equals( Point pt2 );
private:
};

#endif	/* POINT_H */

