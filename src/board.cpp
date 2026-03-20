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

    Candy* origen = getCell(x, y); //si el caramelo es caramelo vacio salimos
    if (origen == nullptr) {
        return false;
    }

    CandyType tipoOrigen = origen->getType(); //guardamos el tipo del caramelo
    int contador = 1; //no contar el del centro
    int compX = 0;
    int compY = 0;
    Candy* vecino = nullptr;

    //Eje X

    //izq
    compX = x - 1;
    vecino = getCell(compX, y);
    while (vecino != nullptr and vecino->getType() == tipoOrigen) {
        contador++; //va comprobando si son del mismo tipo hacia la izquierda
        compX--;
        vecino = getCell(compX, y);
    }

    //der
    compX = x + 1;
    vecino = getCell(compX, y);
    while (vecino != nullptr and vecino->getType() == tipoOrigen) {
        contador++; //va comprobando si son del mismo tipo hacia la derecha
        compX++;
        vecino = getCell(compX, y);
    }

    //todos los del mismo tipo se guardan en contador para saber si deberían explotar

    if (contador >= SHORTEST_EXPLOSION_LINE) {
        return true;
    }

    //Eje Y
    contador = 1;

    //arriba
    compY = y - 1;
    vecino = getCell(x, compY);
    while (vecino != nullptr and vecino->getType() == tipoOrigen) {
        contador++;
        compY--;
        vecino = getCell(x, compY);
    }

    //abajo
    compY = y + 1;
    vecino = getCell(x, compY);
    while (vecino != nullptr and vecino->getType() == tipoOrigen) {
        contador++;
        compY++;
        vecino = getCell(x, compY);
    }

    if (contador >= SHORTEST_EXPLOSION_LINE) {
        return true;
    }

    //Eje ArribaIzq -> AbajoDer
    contador = 1;

    //ArribaIzq
    compX = x - 1;
    compY = y - 1;
    vecino = getCell(compX, compY);
    while (vecino != nullptr and vecino->getType() == tipoOrigen) {
        contador++;
        compX--;
        compY--;
        vecino = getCell(compX, compY);
    }

    //AbajoDer
    compX = x + 1;
    compY = y + 1;
    vecino = getCell(compX, compY);
    while (vecino != nullptr and vecino->getType() == tipoOrigen) {
        contador++;
        compX++;
        compY++;
        vecino = getCell(compX, compY);
    }

    if (contador >= SHORTEST_EXPLOSION_LINE) {
        return true;
    }

    //Eje AbajoIzq -> ArribaDer
    contador = 1;

    //AbajoIzq
    compX = x - 1;
    compY = y + 1;
    vecino = getCell(compX, compY);
    while (vecino != nullptr && vecino->getType() == tipoOrigen) {
        contador++;
        compX--; compY++;
        vecino = getCell(compX, compY);
    }

    //ArribaDer
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

    //Si el contador no ha llegado minimo hasta 3 en todos los ejes significa que no tiene que explotar
    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    // Implement your code here
    // Hacemos un vector para ir guardando los caramelos que hemos explotado.
    vector<Candy*> explotados;
    do
    {
        int contador = 0;
        // Lista para guardar las posiciones que se explotaran posteriormente.
        bool lista[boardX][boardY];
        for (int x = 0; x < boardX; x++)
        {
            for (int y = 0; y < boardY; y++)
            {
                // Si puede explotar lo guardamos en la lista.
                if (shouldExplode(x,y))
                {
                    lista[x][y] = true;
                }
            }
        }

        
        for (int x = 0; x < boardX; x++)
        {
            for (int y = 0; y < boardY; y++)
            {
                // Recorremos el tablero y si alguna casilla puede explotar, añadimos el caramelo
                // a la lista de explotados y vaciamos la casilla. Sumando 1 al contador para que
                // posteriormente se vuelva a comprobar posibles reacciones en cadena.
                if (lista[x][y])
                {
                    explotados.push_back(getCell(x,y));
                    setCell(nullptr, x, y);
                    contador++;
                }
            }
        }
        
        for (int x = 0; x < boardX - 1; x++)
        {
            for (int y = boardY - 1; y > 0; y++)
            {
                // Comprobamos cada columna de abajo a arriba hasta encontrar una casilla vacia.
                if (getCell(x,y) = nullptr)
                {
                    // Una vez encontrada buscamos una casilla NO vacia encima de esa.
                    for (int k = y - 1; k > 0; k++)
                    {
                        // Si se encuentra, bajamos el caramelo y vaciamos la casilla donde estaba.
                        if(getCell(x,k) != nullptr)
                        {
                            setCell(getCell(x,k), x, y);
                            setCell(nullptr, x, k);
                            break;
                        }
                    }
                }
            }
        }
        //Si se ha explotado algo, volvemos a comprobar.
    } while (contador != 0);
    return explotados;
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

//La funcion deshace el cambio para poder leerlo en el tablero.
CandyType stringATipo(string type)
{
    if (type == "BLUE") return CandyType::TYPE_BLUE;
    else if (type == "GREEN") return CandyType::TYPE_GREEN;
    else if (type == "ORANGE") return CandyType::TYPE_ORANGE;
    else if (type == "PURPLE") return CandyType::TYPE_PURPLE;
    else if (type == "RED") return CandyType::TYPE_RED;
    else if (type == "YELLOW") return CandyType::TYPE_YELLOW;
    else return CandyType::COUNT;
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
    ifstream boardLoad(input_path);
    // Solo se ejecuta si se ha podido abrir bien el archivo.
    if (boardLoad)
    {
        for (int c = 0; c < boardX; c++) {
            for (int f = 0; f < boardY; f++) {
                setCell(nullptr, c, f);
            }
        }

        string tipoStr;
        int c;
        int f;

        //El bucle se repite mientras queden lineas por leer.
        while (boardLoad >> tipoStr >> c >> f) {
            CandyType tipoEnum = stringATipo(tipoStr); //devuelve el tipo del caramelo
            // El "new" hace que no se destruya el objeto al salir del bucle.
            Candy* nuevoCaramelo = new Candy(tipoEnum);
            setCell(nuevoCaramelo, c, f);
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
