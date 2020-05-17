
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

const int BLOCK_WIDTH = 4;
const string BLOCK_EMPTY_LINE = "....";
const char DELETE_CHAR = 'X';
const char BLOCK_WIDTH_SQR = 16;
const char OCCUPIED = '#';
typedef vector< vector<int> > blockMatrix;

struct block
{
    int ysize;
    int xsize;
    string content;  
};

void printBlockMatrix(blockMatrix bMatrix)
{
        vector<int> row;
        for (int i = 0; i < bMatrix.size(); ++i)
        {
                row = bMatrix[i];
                for (int j = 0; j < row.size(); ++j)
                {
                        cout << row[j];
                }
                cout << endl;
        }
}

void printVector(vector<int> row)
{
        for (int i = 0; i < row.size(); ++i)
        {
                cout << row[i];
        }
        cout << endl;
}

blockMatrix blockObjContToMatrix(block blockShape, int blockNum)
{
        blockMatrix bMatrix;
        vector<int> row;

        for (int i = 0; i  < blockShape.ysize; ++i)
        {
                for (int j = 0; j < blockShape.xsize; ++j)
                {
                        if (blockShape.content.substr(i*blockShape.xsize,blockShape.xsize)[j] == OCCUPIED)
                                row.push_back(blockNum);
                        else
                                row.push_back(0);                
                }

                bMatrix.push_back(row);
                row.clear();
        }
        return bMatrix;
}

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

string rotateBlockStr(string block, int mode)
{
        string output = "";
        for (int i = 0; i < BLOCK_WIDTH_SQR; ++i)
        {
                output.push_back(block[rotateBlock(i, mode)]);
        }
        return output;
}

block convertBlockStringToBlockObj(string shapeString)
{
        int xsize = BLOCK_WIDTH, ysize = BLOCK_WIDTH;
        string temp = "";               //  Temporary string for checking
        string output = shapeString;    //  Scratch pad string

        for (int j = 0; j < BLOCK_WIDTH; ++j)
	{
	        for (int i = 0; i < BLOCK_WIDTH; ++i)
		{
		        temp.push_back(shapeString[4*i+j]);
		}
	        if (temp == BLOCK_EMPTY_LINE)
			{
		        xsize -= 1;
			// Mark chars to be deleted with DELETE CHAR
		        for (size_t i = 0; i < BLOCK_WIDTH; ++i)
			{
			        output[4*i+j] = DELETE_CHAR;
			}
		}
	        temp = "";
	}

        output.erase(remove(output.begin(), output.end(), DELETE_CHAR), output.end());
	// Lines 
	// This code is from https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string

        temp = "";
        string emptyXsizeLine(xsize, '.');

        for (int i = 0; i < BLOCK_WIDTH; ++i)
	{
	        for (int j = 0; j < xsize; ++j)
		{
		        temp.push_back(output[xsize*i+j]);
		}
	        if (temp == emptyXsizeLine)
		{
		        ysize -= 1;
			// Mark these chars to be deleted with DELETE CHAR
		        for (int j = 0; j < xsize; ++j)
			{
			        output[xsize*i+j] = DELETE_CHAR;
			}
		}
	        temp = "";
	}

	// This code is from https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
        output.erase(remove(output.begin(), output.end(), DELETE_CHAR), output.end());

        block result;
        result.ysize = ysize;
        result.xsize = xsize;
        result.content = output;

        return result;
}

void printBlockString(string blockString, int ysize, int xsize)
{
        for (int i = 0; i < ysize; ++i)
        {
                cout << blockString.substr(i*xsize,xsize) << endl;
        }
}

int main()
{
        string blockString = "";
        cout << "Give me blockString!" << endl;
        cin >> blockString;
        string copyStr = blockString;
        block cb;

        printBlockString(blockString, BLOCK_WIDTH, BLOCK_WIDTH);

        for (int i = 0; i < 4; ++ i)
        {
	        copyStr = rotateBlockStr(copyStr, i);
	        cout << " OK! " << copyStr << endl;
                cb = convertBlockStringToBlockObj(copyStr);
                cout << "Block String: " << cb.content  << " dy, dx = " << cb.ysize << " " << cb.xsize << endl;
                printBlockString(cb.content, cb.ysize, cb.xsize);
                vector< vector<int> > blockMatrix = blockObjContToMatrix(cb, 1);
                printBlockMatrix(blockMatrix);
	        copyStr = blockString;
        }
}