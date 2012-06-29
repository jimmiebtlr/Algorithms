/* 
 * File:   PairEnumerator.h
 * Author: jimmie
 *
 * Created on April 24, 2012, 9:56 PM
 * 
 * Enumerates the pairs and sorts them
 * 
 * TODO: If time implement heavy multithreading on this class
 * 
 */

#ifndef PAIRENUMERATOR_H
#define	PAIRENUMERATOR_H

#include <vector>
#include "Point.h"
#include "Pair.h"

class PairEnumerator {
public:    
    void fillPairs( std::vector<Point>& points, std::vector<Pair>& pairs, DistMethod method );
private:

};

#endif	/* PAIRENUMERATOR_H */

