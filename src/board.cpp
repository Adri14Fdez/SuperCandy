#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>
using namespace std;

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

// NUEVA FUNCION: Devuelve un string con el tipo de caramelo 
// para que sea mas intuitiva la lectura del archivo de guardado.
std::string tipoAString(CandyType type)
{
    switch (type)
    {
    case CandyType::TYPE_BLUE:   return "BLUE";
    case CandyType::TYPE_GREEN:  return "GREEN";
    case CandyType::TYPE_ORANGE: return "ORANGE";
    case CandyType::TYPE_PURPLE: return "PURPLE";
    case CandyType::TYPE_RED:    return "RED";
    case CandyType::TYPE_YELLOW: return "YELLOW";
    default:                     return "UNKNOWN";
    }
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    ofstream boardSave(output_path);
    // Solo se ejecuta si se ha podido abrir bien el archivo.
    if (boardSave)
    {
        for (int c = 0; c < boardX; c++)
        {
            for (int f = 0; f < boardY; f++)
            {
                Candy* caramelo = getCell(c, f);

                // Si hay caramelo en la celda [c,f] entonces guarda
                if (caramelo != nullptr)
                {
                    boardSave << tipoAString(caramelo->getType()) << " " << c << " " << f << endl;
                }
            }
        }
        boardSave.close();
        return true;
    }
    else
    {
        cout << "Error de apertura del archivo." << endl;
        return false;
    }
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}
