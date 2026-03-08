#include "board.h"
#include <memory>
#include <iostream>

Board::Board(int width, int height)
{
    // Implement your code here
    boardX = width;
    boardY = height;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            //Inicializa la array para que esté vacía.
            grid[x][y] = nullptr;
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
    if (x >= 0 && x < boardX
        && y >= 0 && y < boardY)
    {
        //Devuelve la celda de grid solo si las coordenadas son válidas.
        return grid[x][y];
    }
    return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
    // Implement your code here
    if (x >= 0 && x < boardX
        && y >= 0 && y < boardY)
    {
        //Acualiza la celda solo si las coordenadas son válidas.
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
