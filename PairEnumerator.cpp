/* 
 * File:   PairEnumerator.cpp
 * Author: jimmie
 * 
 * Created on April 24, 2012, 9:56 PM
 */

#include "PairEnumerator.h"

using namespace std;

void PairEnumerator::fillPairs(vector<Point>& points, vector<Pair>& pairs, DistMethod method)
{
    for (unsigned int i = 0; i < points.size(); i++)
    {
        for (unsigned int j = i + 1; j < points.size(); j++)
        {
            // distance for same pairs will be 0 so the edges where two pts are equal are never used
		
            Pair aPair(points, i, j, method);
            pairs.push_back(aPair);
        }
    }
}