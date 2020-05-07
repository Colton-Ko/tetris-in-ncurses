#ifndef TYPES_H
#define TYPES_H 

#include <string>
#include <vector>
using namespace std;

struct block
{
    int ysize;
    int xsize;
    string content;  
};

struct blocksOnGameboard
{
    int blockNum;
    int posy;
    int posx;
    blocksOnGameboard * head;
};

typedef vector< vector<int> > blockMatrix;

#endif