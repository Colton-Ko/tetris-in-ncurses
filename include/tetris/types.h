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

typedef vector< vector<int> > blockMatrix;
typedef string blockString;

struct scoreEntry
{
        string dateTimeString;
        int playerScore;
        scoreEntry * next;
};

#endif