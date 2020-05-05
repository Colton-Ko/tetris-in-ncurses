#include <ncurses.h>
#include <string>
#include "blocks.h"
#include <iostream>


using namespace std;

void getBlocksReady(string blocks[BLOCK_SHAPE_COUNT])
{
        // Clear all exisitng garabge in blocks[7]
        for (int i = 0; i < BLOCK_SHAPE_COUNT; ++i)
        {
                blocks[i] = "";
        }
        // Create blocks
        // The blocks arrangements and implementation ideas 
        // are from https://youtu.be/8OK8_tHeCIA

        blocks[0] = SHAPE_0;
        blocks[1] = SHAPE_1;
        blocks[2] = SHAPE_2;
        blocks[3] = SHAPE_3;
        blocks[4] = SHAPE_4;
        blocks[5] = SHAPE_5;
        blocks[6] = SHAPE_6;

}

// The idea of implementing rotation through index functions
// are from https://youtu.be/8OK8_tHeCIA

int rotateBlock(int i, int mode)
{
        switch (mode)
        {
                case 0:
                        i = i;
                        break;
                case 1:  // 90 degrees
                        i = 12 - 4*(i % 4) + (i / 4);
                        break;
                case 2: // 180 degrees
                        i = 15 - 4*(i / 4) - (i % 4);
                        break;
                case 3: // 270 degrees
                        i = 3 + 4*( i % 4) - (i / 4);
                default:
                        break;
        }
        return i;
}



// test function for rotation
void printBlock()
{
        string blocks[7];
        getBlocksReady(blocks);
        for (int i = 0; i < BLOCK_SHAPE_COUNT; ++i)
        {
                for ( int j = 0; j < BLOCK_WIDTH_SQR; ++j)
                {
                        if (isprint(blocks[i][j]))
                        {
                                cout << blocks[i][rotateBlock(j,0)];
                        }
                        if ((j+1)%4 == 0)
                        {
                                cout << endl;
                        }
                }
                cout << endl;
        }
}