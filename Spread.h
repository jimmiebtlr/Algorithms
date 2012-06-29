#ifndef SPREAD_H
#define SPREAD_H

#include <vector>
#include <deque>
#include <algorithm>
#include "Point.h"
#include "Pair.h"
using namespace std;

// sort the vector named total

struct Total {
    unsigned short p2;
    unsigned short sum;
};

class Spread {
public:
    Spread(vector<Point> points, vector<Pair> pairs, unsigned short K, unsigned short N, vector< deque<unsigned short> > &Breaks);
};

bool TCompare(Total T1, Total T2);

#endif // SPREAD_H
