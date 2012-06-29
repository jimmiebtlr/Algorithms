/* 
 * File:   Point.cpp
 * Author: jimmie
 * 
 * Created on April 24, 2012, 3:07 PM
 */

#include "Point.h"
using namespace std;

string Point::showPoint()
{
    stringstream aString;
    aString << "(" << x << ", " << y << ", " << z << ")";

    return aString.str();
}

// // // // //
// calcManDist(vector<pairs>&) : fills cost w/ Manhattan dist between p1 & p2
//  

long Point::getManhattanDist(Point p2)
{
    long xSum, ySum, zSum;

    xSum = abs(x - p2.x);
    ySum = abs(y - p2.y);
    zSum = abs(z - p2.z);

    return xSum + ySum + zSum;
}

// // // // //
// calcEucDist(vector<pairs>&) : fills cost w/ Euclidean distance
//      - does not calculate square root - sum of squares only
//      - should be valid for a comparison
//

long Point::getEuclideanDist(Point p2)
{
    long xSum, ySum, zSum;

    xSum = pow((x - p2.x), 2.0);
    ySum = pow((y - p2.y), 2.0);
    zSum = pow((z - p2.z), 2.0);

    return xSum + ySum + zSum;
}

// // // // //
// Determine if this point is equal to pt2
//
//
bool Point::equals(Point p2)
{
    if( x == p2.x && y == p2.y && z == p2.z )
    {
        return true;
    }
    return false;
}