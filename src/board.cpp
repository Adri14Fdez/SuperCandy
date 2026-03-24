#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>
using namespace std;

Board::Board(int width, int height)
{
    if (width > 0 && height > 0 && width <= MAX_BOARD_WIDTH && height <= MAX_BOARD_HEIGHT)
    {
        boardX = width;
        boardY = height;
    }
    else
    {
        boardX = DEFAULT_BOARD_WIDTH;
        boardY = DEFAULT_BOARD_HEIGHT;
        cout << "El tamaño del tablero es inválido, se ha establecido el tamaño por defecto." << endl;
    }
    
    for (int x = 0; x < boardX; x++)
    {
        for (int y = 0; y < boardY; y++)
        {
            // Inicializa la array para que esté vacía.
            grid[x][y] = nullptr;
        }
    }
}

Board::~Board()
{/*
    // Borramos todos los caramelos que queden en el tablero.
    for (int c = 0; c < boardX; c++) {
        for (int f = 0; f < boardY; f++) {
            Candy* actual = getCell(c,f);
            if (actual != nullptr) {
                delete actual; // Como usamos memoria dinamica al cargar, tenemos que borrarlo manualmente.
            }
        }
    }
*/}

Candy* Board::getCell(int x, int y) const
{
    // Devuelve la celda de grid solo si las coordenadas son válidas.
    if (x >= 0 && x < boardX && y >= 0 && y < boardY)
    {
        return grid[x][y];
    }
    return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
    // Acualiza la celda solo si las coordenadas son válidas.
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
    // Si el caramelo es caramelo vacio no hace falta comprobar si explota.
    Candy* origen = getCell(x, y);
    if (origen == nullptr) {
        return false;
    }

    // Guardamos el tipo del caramelo y contamos 1 en cada eje por el caramelo del centro.
    CandyType tipoOrigen = origen->getType();
    int contador;
    int compX = 0;
    int compY = 0;
    Candy* vecino = nullptr;
    
    // Eje X
    {
        contador = 1;

        // Eje X izq
        compX = x - 1;
        vecino = getCell(compX, y);
        while (vecino != nullptr && vecino->getType() == tipoOrigen) {
            contador++;
            compX--;
            vecino = getCell(compX, y);
        }

        // Eje X der
        compX = x + 1;
        vecino = getCell(compX, y);
        while (vecino != nullptr && vecino->getType() == tipoOrigen) {
            contador++;
            compX++;
            vecino = getCell(compX, y);
        }

        // Todos los del mismo tipo se guardan en contador para saber si deberían explotar
        if (contador >= SHORTEST_EXPLOSION_LINE) {
            return true;
        }
    }

    // Eje Y
    {
        contador = 1;
        
        // Eje Y arriba
        compY = y - 1;
        vecino = getCell(x, compY);
        while (vecino != nullptr && vecino->getType() == tipoOrigen) {
            contador++;
            compY--;
            vecino = getCell(x, compY);
        }

        // Eje Y abajo
        compY = y + 1;
        vecino = getCell(x, compY);
        while (vecino != nullptr && vecino->getType() == tipoOrigen) {
            contador++;
            compY++;
            vecino = getCell(x, compY);
        }

        // Todos los del mismo tipo se guardan en contador para saber si deberían explotar
        if (contador >= SHORTEST_EXPLOSION_LINE) {
            return true;
        }
    }
    
    // Eje Diagonal (\) 
    {
        contador = 1;

        //ArribaIzq
        compX = x - 1;
        compY = y - 1;
        vecino = getCell(compX, compY);
        while (vecino != nullptr && vecino->getType() == tipoOrigen) {
            contador++;
            compX--;
            compY--;
            vecino = getCell(compX, compY);
        }

        //AbajoDer
        compX = x + 1;
        compY = y + 1;
        vecino = getCell(compX, compY);
        while (vecino != nullptr && vecino->getType() == tipoOrigen) {
            contador++;
            compX++;
            compY++;
            vecino = getCell(compX, compY);
        }

        // Todos los del mismo tipo se guardan en contador para saber si deberían explotar
        if (contador >= SHORTEST_EXPLOSION_LINE) {
            return true;
        }
    }

    // Eje Diagonal (/)
    {
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

        // Todos los del mismo tipo se guardan en contador para saber si deberían explotar
        if (contador >= SHORTEST_EXPLOSION_LINE) {
            return true;
        }
    }

    // Si el contador no ha llegado minimo hasta SHORTEST_EXPLOSION_LINE
    // en todos los ejes significa que no tiene que explotar.
    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    // Hacemos un vector para ir guardando los caramelos que hemos explotado.
    vector<Candy*> explotados;
    int contador;

    do
    {
        contador = 0;
        // Lista para guardar las posiciones que se explotaran posteriormente.
        bool lista[MAX_BOARD_WIDTH][MAX_BOARD_HEIGHT] = {false};
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
        
        for (int x = 0; x < boardX; x++)
        {
            for (int y = boardY - 1; y >= 0; y--)
            {
                // Comprobamos cada columna de abajo a arriba hasta encontrar una casilla vacia.
                if (getCell(x,y) == nullptr)
                {
                    // Una vez encontrada buscamos una casilla NO vacia encima de esa.
                    for (int k = y - 1; k >= 0; k--)
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
    } while (contador > 0);
    return explotados;
}

// NUEVA FUNCION: Devuelve un string con el tipo de caramelo 
// para que sea mas intuitiva la lectura del archivo de guardado.
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

//La funcion deshace el cambio para poder leerlo en el tablero.
CandyType stringATipo(const string& type)
{
    if (type == "AZUL") return CandyType::TYPE_BLUE;
    if (type == "VERDE") return CandyType::TYPE_GREEN;
    if (type == "NARANJA") return CandyType::TYPE_ORANGE;
    if (type == "LILA") return CandyType::TYPE_PURPLE;
    if (type == "ROJO") return CandyType::TYPE_RED;
    if (type == "AMARILLO") return CandyType::TYPE_YELLOW;
    
    return CandyType::COUNT; // Devuelve COUNT si el texto es corrupto o no se reconoce
}

bool Board::dump(const std::string& output_path) const
{
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
    // Solo se ejecuta si se ha podido abrir bien el archivo.
    if (boardLoad)
    {
        // Borramos todo lo que habia en el tablero.
        for (int c = 0; c < boardX; c++) {
            for (int f = 0; f < boardY; f++) {
                Candy* actual = getCell(c,f);
                if (actual != nullptr) {
                    delete actual; // Como usamos memoria dinamica al cargar, tenemos que borrarlo manualmente.
                    setCell(nullptr, c, f);
                }
            }
        }

        string tipoStr;
        int c, f;

        //El bucle se repite mientras queden lineas por leer.
        while (boardLoad >> tipoStr >> c >> f) {
            CandyType tipoEnum = stringATipo(tipoStr); //devuelve el tipo del caramelo

            // Solo creamos el caramelo si el texto leído es válido
            if (tipoEnum != CandyType::COUNT) {
                Candy* nuevoCaramelo = new Candy(tipoEnum); // El "new" hace que no se destruya el objeto al salir del bucle.
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
