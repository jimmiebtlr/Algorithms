/* 
 * File:   PointsReader.h
 * Author: jimmie
 *
 * Created on April 24, 2012, 2:51 PM
 */

#ifndef POINTSREADER_H
#define	POINTSREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>        // pow function
#include <algorithm>    // sort function
#include <cstdlib>      // abs function
#include "Point.h"

class PointsReader {
public:
    
    void getInput(std::string fName, std::vector<Point>& points);
    int getN( ) const {return n;};
    int getK( ) const {return k;};
    DistMethod getMethod() const {      if( mm == 0 )
                                                { return Euclidean; }
                                        else if( mm == 1 )
                                                { return Manhattan; }
                                        };
private:
    int n;
    int k;
    int mm;
};

#endif	/* POINTSREADER_H */

