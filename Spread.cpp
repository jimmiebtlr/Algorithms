#include "Spread.h"
#include <vector>

Spread::Spread(vector<Point> points, vector<Pair> pairs, unsigned short K, unsigned short N, vector<deque<unsigned short> > &Breaks)
{
    deque<unsigned short> Abreak;
    vector<unsigned short> total;
    total.resize( points.size() );
    // calculate distance between each point to every other point
    //  a b c d e
    //  total[a]= ab+ac+ad+ae
    //  
    for (unsigned int i = 0; i < points.size(); i++)
    {
        total[i] = 0;
        for (unsigned int j = 0; j < pairs.size(); j++)
        {
            if ((i == pairs[j].pt1) || (i == pairs[j].pt2))
            {
                total[i] = total[i] + pairs[j].cost;
            }
        }
    }

    vector<Total> DistList;
    DistList.resize( points.size() );
    for (unsigned int i; i < points.size(); i++)
    {
        DistList[i].p2 = i;
        DistList[i].sum = total[i];
    }
    
    sort(DistList.begin(), DistList.end(), TCompare);
    
    Breaks.resize(K);
    //for the largest k-1 points (largest being the sum of the distance
    // from one point to all other points), give them their own group
    for (unsigned short m = 0; m < K -1; m++)
    {
        Abreak.push_back(DistList[m].p2);
        Breaks[m].push_back(Abreak[m]);
    }

    Abreak.clear();
    //put all remaining points in the last group
    for (unsigned int i =  0; i < (N+1-K); i++)
    {
        Abreak.push_back( DistList[ DistList.size() - i ].p2 );
    }
    Breaks[K-1] = Abreak;
}


bool TCompare(Total T1, Total T2) {
    if (T1.sum > T2.sum) {
        return true;
    }

    return false;
}