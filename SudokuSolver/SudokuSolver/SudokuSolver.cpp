// SudokuSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

//this class helps keep track of the important info associated to each
//cell such as value, index, and which collections it belongs to
class InputItem {
public:
    int Value, Index, Row, Column, Block;
    InputItem(int value, int index)
    {
        Value = value;
        Index = index;
        Row = GetCurrentRow(Index);
        Column = GetCurrentColumn(Index);
        Block = GetCurrentBlock(Row, Column);
    }

    int GetCurrentRow(int index)
    {
        if (index <= 8) return 0;
        if (index <= 17) return 1;
        if (index <= 26) return 2;
        if (index <= 35) return 3;
        if (index <= 44) return 4;
        if (index <= 53) return 5;
        if (index <= 62) return 6;
        if (index <= 71) return 7;
        return 8;
    }

    int GetCurrentColumn(int index)
    {
        if (index % 9 == 0) return 0;
        if (index % 9 == 1) return 1;
        if (index % 9 == 2) return 2;
        if (index % 9 == 3) return 3;
        if (index % 9 == 4) return 4;
        if (index % 9 == 5) return 5;
        if (index % 9 == 6) return 6;
        if (index % 9 == 7) return 7;
        return 8;
    }

    int GetCurrentBlock(int row, int col)
    {
        if (row <= 2)
        {
            if (col <= 2) return 0;
            if (col <= 5) return 1;
            return 2;
        }

        if (row <= 5)
        {
            if (col <= 2) return 3;
            if (col <= 5) return 4;
            return 5;
        }

        if (col <= 2) return 6;
        if (col <= 5) return 7;
        return 8;
    }
};

//Data initializations
//static int Input[81];
static int PreviousCount;
static int NewCount;
static int PreviousIndex;
static int PreviousInt;
//I have 3 collections that each hold 9 collections that represent
//a specific row, column, or block from the puzzle to analyze
static vector<vector<int>> HorizontalCollection;
static vector<vector<int>> VerticalCollection;
static vector<vector<int>> BlockCollection;
static vector<InputItem> Output;

//Function declarations
void GetInput();
void PopulateOutput(int[]);
void LoadHorizontalCollection();
vector<int> LoadHorizontal(int);
void LoadVerticalCollection();
vector<int> LoadVertical(int);
void LoadBlockCollection();
vector<int> LoadBlock(int);
void LoadSolArray();
vector<int> PossibleValues(int, int, int);
int CountZeros();
void TryInt();
int NumberCount(int);


int Input[81];

//This method just gets the input sudoku puzzle values
void GetInput()
{
    int input;
    cout << "Welcome to Sam's Sudoku Solver!" << endl;
    cout << "Type in the puzzle you'd like to solve by placing zeros in the blank cells." << endl;
    for (int i = 0; i < 81; i++)
    {
        cin >> input;
        Input[i] = input;
    }
    cout << "Input:" << endl;
    for (int i = 0; i < 81; i++)
    {
        if (i % 9 == 0) cout << endl;
        cout << Input[i];
    }
    cout << endl << endl;
}

void PopulateOutput(int input[])
{
    Output.clear();
    int index = 0;
    for (int i = 0; i != 81; i++)
    {
        InputItem item(Input[i], index);
        Output.push_back(item);
        index++;
    }
}


//Each of these "load" functions load the 27 collections discussed above
void LoadHorizontalCollection()
{
    HorizontalCollection.clear();
    for (int row = 0; row <= 80; row += 9) HorizontalCollection.push_back(LoadHorizontal(row));
}

vector<int> LoadHorizontal(int start)
{
    vector<int> Row;
    for (int col = start; col <= start + 8; col++)
    {
        Row.push_back(Input[col]);
    }

    return Row;
}

void LoadVerticalCollection()
{
    VerticalCollection.clear();
    for (int col = 0; col < 9; col++) VerticalCollection.push_back(LoadVertical(col));
}

vector<int> LoadVertical(int start)
{
    vector<int> Column;
    for (int row = start; row <= 80; row += 9)
    {
        Column.push_back(Input[row]);
    }

    return Column;
}

void LoadBlockCollection()
{
    BlockCollection.clear();
    for (int start = 0; start <= 80; start += 27)
    {
        for (int i = start; i <= start + 8; i += 3) BlockCollection.push_back(LoadBlock(i));
    }
}

vector<int> LoadBlock(int start)
{
    vector<int> Block;
    for (int i = start; i <= start + 2; i++)
    {
        Block.push_back(Input[i]);
    }

    for (int i = start + 9; i <= start + 11; i++)
    {
        Block.push_back(Input[i]);
    }

    for (int i = start + 18; i <= start + 20; i++)
    {
        Block.push_back(Input[i]);
    }

    return Block;
}

//this function does most of the work of analyzing and loading the output data
void LoadSolArray()
{
    PreviousCount = NewCount;
    //cycle through 0's using the Horizontal vectors since that's the easiest to visualize
    //start by loading the values that only have a single possible value
    for(int i = 0; i <= 80; i++)
    {
        InputItem item = Output[i];
        if (item.Value != 0) continue;

        if (PossibleValues(item.Row, item.Column, item.Block).size() == 1)
        {
            int newValue = PossibleValues(item.Row, item.Column, item.Block)[0];
            Input[item.Index] = newValue;
        }

        else if (PossibleValues(item.Row, item.Column, item.Block).size() == 0)
            Input[PreviousIndex] = PreviousInt;
    }

    LoadHorizontalCollection();
    LoadVerticalCollection();
    LoadBlockCollection();
    PopulateOutput(Input);
    NewCount = CountZeros();

}

//this function matches the cell with it's corresponding collections 
//and finds all of it's possible values by ruling out the ones
//that are already accounted for
vector<int> PossibleValues(int row, int col, int block)
{
    //find the Hor, Vert, & Block vectors that corrispond to the current number we're looking at
    vector<int> possibleVals;
    vector<int> hor = HorizontalCollection[row];
    vector<int> ver = VerticalCollection[col];
    vector<int> blk = BlockCollection[block];
    for (int i = 1; i <= 9; i++)
    {
        bool checkHor = (find(hor.begin(), hor.end(), i) != hor.end());
        bool checkVer = (find(ver.begin(), ver.end(), i) != ver.end());
        bool checkBlk = (find(blk.begin(), blk.end(), i) != blk.end());

        if (!checkHor  && !checkVer && !checkBlk)
        {
            possibleVals.push_back(i);
        }
    }
    return possibleVals;
}

//just for keeping track of how close we are
int CountZeros()
{
    int count = 0;
    for (int i = 1; i < 81; i++)
    {
        InputItem item = Output[i];
        if (item.Value == 0) count++;
    }
    return count;
}

//once we can't find any more cells with a single possible value,
//we attempt to move things along by picking a possible value
//and trying to solve the rest of the puzzle
void TryInt()
{
    for (int i = 0; i < 81; i++)
    {
        InputItem item = Output[i];
        if (item.Value != 0) continue;
        int newValue = PossibleValues(item.Row, item.Column, item.Block)[0];
        Input[item.Index] = newValue;
        PreviousIndex = item.Index;
        PreviousInt = newValue;
        LoadHorizontalCollection();
        LoadVerticalCollection();
        LoadBlockCollection();
        PopulateOutput(Input);
        LoadSolArray();
        return;
    }
}

//just for final metrics
int NumberCount(int num)
{
    int count = 0;
    for (int i = 0; i < 81; i++)
    {
        InputItem item = Output[i];
        if (item.Value == num) count++;
    }
    return count;
}

int main()
{
    GetInput();
    PopulateOutput(Input);
    LoadHorizontalCollection();
    LoadVerticalCollection();
    LoadBlockCollection();
    cout << "Before: " << CountZeros() <<endl;
    while (CountZeros() != 0)
    {
        while (PreviousCount != NewCount)
        {
            LoadSolArray();
        }
        TryInt();
    }
    cout << "After: " << CountZeros() << endl;
    cout << endl;
    for (int i = 1; i <= 9; i++)
    {
        cout << i << ": " << NumberCount(i) <<endl;
    }

    cout << endl << endl;

    for (int i = 0; i < 9; i++)
    {
        vector<int> row = HorizontalCollection[i];
        for (int j = 0; j < 9; j++)
        {
            if (j != 0 && j % 3 == 0) cout << "| ";
            int num = row[j];
            cout << num << " ";
        }
        if(i != 8 && (i+1) % 3 == 0) cout << endl << "---------------------";
        cout << endl;
    }
    cin.get();
    cin.get();
    return 0;
}
