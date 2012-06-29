/* 
 * File:   Pair.h
 * Author: jimmie
 *
 * Created on April 24, 2012, 9:33 PM
 */

#ifndef PAIR_H
#define	PAIR_H

#include <vector>
#include "Point.h"

using namespace std;

class Pair {
public:
    Pair( vector<Point>& points, unsigned short pt1, unsigned short pt2, DistMethod method );
    bool operator<(Pair pair2) const { return cost<pair2.cost; };
    
    // rather than saving 3 ints we can save a single short to describe location
    unsigned short pt1;
    unsigned short pt2;
    
    // cost will never be negative
    unsigned long cost;
    
private:

};

#endif	/* PAIR_H */

