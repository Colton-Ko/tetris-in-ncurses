
#include <iostream>
#include <string>
#include <vector>

using namespace std;
const int BLOCK_WIDTH = 4;
const string BLOCK_EMPTY_LINE = "....";
const char DELETE_CHAR = 'X';
const char BLOCK_WIDTH_SQR = 16;
const char OCCUPIED = '#';

struct block
{
    int ysize;
    int xsize;
    string content;  
};

void printBlockMatrix(vector< vector<int> > blockMatrix)
{
        vector<int> row;
        for (int i = 0; i < blockMatrix.size(); ++i)
        {
                row = blockMatrix[i];
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

vector< vector<int> > blockObjContToMatrix(block blockShape, int n)
{
        vector< vector<int> > blockMatrix;
        vector<int> row;

        for (int i = 0; i  < blockShape.ysize; ++i)
        {
                for (int j = 0; j < blockShape.xsize; ++j)
                {
                        if (blockShape.content.substr(i*blockShape.xsize,blockShape.xsize)[j] == OCCUPIED)
                                row.push_back(n);
                        else
                                row.push_back(0);                
                }

                blockMatrix.push_back(row);
                row.clear();
        }
        return blockMatrix;
}

block blockStringToBlockObj(string shapeString)
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

    // This code is from https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    output.erase(remove(output.begin(), output.end(), DELETE_CHAR), output.end());

    temp = "";
    for (int i = 0; i < BLOCK_WIDTH; ++i)
    {
        for (int j = 0; j < xsize; ++j)
        {
            temp.push_back(shapeString[4*i+j]);
        }
        if (temp == BLOCK_EMPTY_LINE)
        {
            ysize -= 1;
            // Mark these chars to be deleted with DELETE CHAR
            for (int j = 0; j < xsize; ++j)
            {
                output[4*i+j] = DELETE_CHAR;
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
        block cb = blockStringToBlockObj(blockString);
        cout << "Block String: " << cb.content  << " dy, dx = " << cb.ysize << " " << cb.xsize << endl;
        printBlockString(cb.content, cb.ysize, cb.xsize);
        vector< vector<int> > blockMatrix = blockObjContToMatrix(cb, 1);
        printBlockMatrix(blockMatrix);
}