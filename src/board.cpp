#include "board.h"
#include <memory>
#include <iostream>

Board::Board(int width, int height)
{
    // Implement your code here
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grid[i][j] = nullptr;
        }
    }
}

Board::~Board()
{
    // Implement your code here
}


Candy* Board::getCell(int x, int y) const
{
    // Implement your code here

    return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
    // Implement your code here
    if (x > 0 && x < boardX
        && y > 0 && y < boardY)
    {
        grid[x][y] = candy;
    }
}


int Board::getWidth() const
{
    // Implement your code here
    return boardX;
}


int Board::getHeight() const
{
    // Implement your code here
    return boardY;
}

bool Board::shouldExplode(int x, int y) const
{
    // Implement your code here
    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    // Implement your code here
    return {};
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    return false;
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}
