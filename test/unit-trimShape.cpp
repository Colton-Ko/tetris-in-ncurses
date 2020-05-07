#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

const int BLOCK_WIDTH = 4;
const string BLOCK_EMPTY_LINE = "....";
const char DELETE_CHAR = 'X';

struct block
{
    int ysize = 0;
    int xsize = 0;
    string content = "";  
};


void printShapeString(string shapeString, int ysize, int xsize)
{
    for (int i = 0; i < ysize*xsize; ++i)
    {
        cout << shapeString[i];
        if (i % xsize == xsize-1)
            cout << endl;
    }
}

void printBlockMatrix(vector< vector<int> > blockMatrix, int ysize, int xsize)
{
    for (int i = 0; i < blockMatrix.size(); i++)
    {
        for (int j = 0; j < blockMatrix[i].size(); j++)
        {
            cout << blockMatrix[i][j];
        }
        cout << endl;
    }
}

vector< vector<int> > shapeStringToArray(string shapeString, int blockNum, int ysize, int xsize)
{
	vector< vector<int> > blockMatrix;
	vector<int> row;
    cout << ysize << " " << xsize << endl;
	for (int i = 0; i < ysize; ++i)
	{
		for (int j = 0; j < xsize; ++j)
		{
				switch(shapeString[4*i+j])
				{
					case '.':							// Empty space
						row.push_back(0);
						break;
					case '#':						
						row.push_back(blockNum);		// Solid block
						break;
				}
		}
		blockMatrix.push_back(row);
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

int main()
{

    string y = "";
    cout << "Give me a string! > ";
    cin >> y;

    block result = blockStringToBlockObj(y);

    printShapeString(result.content, result.ysize, result.xsize);

    vector< vector<int> > blockMatrix = shapeStringToArray(result.content, 1, result.ysize, result.xsize);
    printBlockMatrix(blockMatrix, result.ysize, result.xsize);

}