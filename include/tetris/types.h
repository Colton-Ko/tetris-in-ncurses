#ifndef TYPES_H
#define TYPES_H 

#include <string>
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

#endif