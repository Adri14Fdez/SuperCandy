#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>
using namespace std;

Board::Board(int width, int height)
{
	boardX = width;
	boardY = height;

	grid = new Candy * *[boardX];

	for (int x = 0; x < boardX; x++)
	{
		grid[x] = new Candy * [boardY];

		for (int y = 0; y < boardY; y++)
		{
			grid[x][y] = nullptr;
		}
	}
}

Board::~Board()
{
	for (int x = 0; x < boardX; x++)
	{
		delete[] grid[x];
	}
	delete[] grid;
}

Candy* Board::getCell(int x, int y) const
{
	if (x >= 0 && x < boardX && y >= 0 && y < boardY)
	{
		return grid[x][y];
	}
	return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
	if (x >= 0 && x < boardX && y >= 0 && y < boardY)
	{
		grid[x][y] = candy;
	}
}

int Board::getWidth() const
{
	return boardX;
}

int Board::getHeight() const
{
	return boardY;
}

bool Board::shouldExplode(int x, int y) const
{
	Candy* origen = getCell(x, y);
	if (origen == nullptr) {
		return false;
	}

	CandyType tipoOrigen = origen->getType();
	int contador;
	int compX = 0;
	int compY = 0;
	Candy* vecino = nullptr;

	// Eje X
	{
		contador = 1;
		compX = x - 1;
		vecino = getCell(compX, y);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compX--;
			vecino = getCell(compX, y);
		}
		compX = x + 1;
		vecino = getCell(compX, y);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compX++;
			vecino = getCell(compX, y);
		}
		if (contador >= SHORTEST_EXPLOSION_LINE) {
			return true;
		}
	}

	// Eje Y
	{
		contador = 1;
		compY = y - 1;
		vecino = getCell(x, compY);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compY--;
			vecino = getCell(x, compY);
		}
		compY = y + 1;
		vecino = getCell(x, compY);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compY++;
			vecino = getCell(x, compY);
		}
		if (contador >= SHORTEST_EXPLOSION_LINE) {
			return true;
		}
	}

	// Eje Diagonal (\) 
	{
		contador = 1;
		compX = x - 1;
		compY = y - 1;
		vecino = getCell(compX, compY);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compX--; compY--;
			vecino = getCell(compX, compY);
		}
		compX = x + 1;
		compY = y + 1;
		vecino = getCell(compX, compY);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compX++; compY++;
			vecino = getCell(compX, compY);
		}
		if (contador >= SHORTEST_EXPLOSION_LINE) {
			return true;
		}
	}

	// Eje Diagonal (/)
	{
		contador = 1;
		compX = x - 1;
		compY = y + 1;
		vecino = getCell(compX, compY);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compX--; compY++;
			vecino = getCell(compX, compY);
		}
		compX = x + 1;
		compY = y - 1;
		vecino = getCell(compX, compY);
		while (vecino != nullptr && vecino->getType() == tipoOrigen) {
			contador++;
			compX++; compY--;
			vecino = getCell(compX, compY);
		}
		if (contador >= SHORTEST_EXPLOSION_LINE) {
			return true;
		}
	}

	return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
	vector<Candy*> explotados;
	int contador;

	do
	{
		contador = 0;
		bool lista[MAX_BOARD_WIDTH][MAX_BOARD_HEIGHT] = { false };
		for (int x = 0; x < boardX; x++)
		{
			for (int y = 0; y < boardY; y++)
			{
				if (shouldExplode(x, y))
				{
					lista[x][y] = true;
				}
			}
		}

		for (int x = 0; x < boardX; x++)
		{
			for (int y = 0; y < boardY; y++)
			{
				if (lista[x][y])
				{
					explotados.push_back(getCell(x, y));
					setCell(nullptr, x, y);
					contador++;
				}
			}
		}

		for (int x = 0; x < boardX; x++)
		{
			for (int y = boardY - 1; y >= 0; y--)
			{
				if (getCell(x, y) == nullptr)
				{
					for (int k = y - 1; k >= 0; k--)
					{
						if (getCell(x, k) != nullptr)
						{
							setCell(getCell(x, k), x, y);
							setCell(nullptr, x, k);
							break;
						}
					}
				}
			}
		}
	} while (contador > 0);
	return explotados;
}

std::string tipoAString(CandyType type)
{
	switch (type)
	{
	case CandyType::TYPE_BLUE:   return "AZUL";
	case CandyType::TYPE_GREEN:  return "VERDE";
	case CandyType::TYPE_ORANGE: return "NARANJA";
	case CandyType::TYPE_PURPLE: return "LILA";
	case CandyType::TYPE_RED:    return "ROJO";
	case CandyType::TYPE_YELLOW: return "AMARILLO";
	default:                     return "DESCONOCIDO";
	}
}

CandyType stringATipo(const string& type)
{
	if (type == "AZUL") return CandyType::TYPE_BLUE;
	if (type == "VERDE") return CandyType::TYPE_GREEN;
	if (type == "NARANJA") return CandyType::TYPE_ORANGE;
	if (type == "LILA") return CandyType::TYPE_PURPLE;
	if (type == "ROJO") return CandyType::TYPE_RED;
	if (type == "AMARILLO") return CandyType::TYPE_YELLOW;

	return CandyType::COUNT;
}

bool Board::dump(const std::string& output_path) const
{
	ofstream boardSave(output_path);
	if (boardSave)
	{
		for (int c = 0; c < boardX; c++)
		{
			for (int f = 0; f < boardY; f++)
			{
				Candy* caramelo = getCell(c, f);
				if (caramelo != nullptr)
				{
					boardSave << tipoAString(caramelo->getType()) << " " << c << " " << f << "\n";
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
	ifstream boardLoad(input_path);
	if (boardLoad)
	{
		for (int c = 0; c < boardX; c++)
		{
			for (int f = 0; f < boardY; f++)
			{
				Candy* actual = getCell(c, f);
				if (actual != nullptr)
				{
					delete actual;
					setCell(nullptr, c, f);
				}
			}
		}

		string tipoStr;
		int c, f;

		while (boardLoad >> tipoStr >> c >> f)
		{
			CandyType tipoEnum = stringATipo(tipoStr);

			if (tipoEnum != CandyType::COUNT)
			{
				Candy* nuevoCaramelo = new Candy(tipoEnum);
				setCell(nuevoCaramelo, c, f);
			}
		}

		boardLoad.close();
		return true;
	}
	else
	{
		cout << "Error de apertura del archivo." << endl;
		return false;
	}
}

bool Board::operator==(const Board& other) const
{
	bool esIgual = true;

	if (boardX != other.boardX || boardY != other.boardY)
	{
		esIgual = false;
	}

	for (int x = 0; x < boardX && esIgual; x++)
	{
		for (int y = 0; y < boardY && esIgual; y++)
		{
			Candy* miCaramelo = getCell(x, y);
			Candy* suCaramelo = other.getCell(x, y);

			if ((miCaramelo == nullptr && suCaramelo != nullptr) ||
				(miCaramelo != nullptr && suCaramelo == nullptr))
			{
				esIgual = false;
			}
			else if (miCaramelo != nullptr && suCaramelo != nullptr)
			{
				if (miCaramelo->getType() != suCaramelo->getType())
				{
					esIgual = false;
				}
			}
		}
	}

	return esIgual;
}