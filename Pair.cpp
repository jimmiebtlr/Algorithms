/* 
 * File:   Pair.cpp
 * Author: jimmie
 * 
 * Created on April 24, 2012, 9:33 PM
 */

#include "Pair.h"

using namespace std;

Pair::Pair( std::vector<Point>& points, unsigned short p1, unsigned short p2, DistMethod method )
{
    pt1 = p1;
    pt2 = p2;
    if( method == Manhattan )
    {
        cost = points[pt1].getManhattanDist(points[pt2]);
    }
    else if( method == Euclidean )
    {
        cost = points[pt1].getEuclideanDist(points[pt2]);
    }
    else
    {
	    //cout << Manhattan << endl;
	    //cout << Euclidean << endl;
	cout << "Not a valid method of distance calculation" << endl;
    }
}