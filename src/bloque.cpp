#include "bloque.h"
#include <ctime>

Bloque::Bloque()
{
	nuevoBloque();
}

Bloque::~Bloque()
{
    for (int i = 0; i < 3; i++)
    {
        if (m_bloqueCaramelos[i] != nullptr)
        {
            delete m_bloqueCaramelos[i];
            m_bloqueCaramelos[i] = nullptr;
        }
    }
}

void Bloque::nuevoBloque()
{
    m_x = 5;
    m_y = -3;

    srand(time(NULL));
    int numAleatorio = 0;

    for (int i = 0; i < 3; i++)
    {
        numAleatorio = rand() % 6; //Hacemos el módulo del número aleatorio con 6, para que nos de 1, 2, 3, 4 o 5.

        // Convertimos ese número entero a un tipo de caramelo (CandyType) y lo creamos
        m_bloqueCaramelos[i] = new Candy(CandyType(numAleatorio));
    }
}

bool Bloque::puedeCaer(const Board& tablero, int lim)
{
    bool puedeCaer = true;
    int bottomBloque = m_y + 3;

    if (m_y >= lim || bottomBloque >= 0 && tablero.getCell(m_x, bottomBloque) != nullptr)
    {
        puedeCaer = false;
    }

    return puedeCaer;
}

void Bloque::moverIzq(const Controller& controller, const Board& tablero, int limIzq, int limDer)
{
    if (controller.isLeftPressed())
    {
        bool posible = true;
        for (int i = 0; i < 3; i++)
        {
            if (tablero.getCell(m_x - 1, m_y + i) != nullptr)
            {
                posible = false;
            }
        }

        if (m_x <= limIzq || m_x > limDer)
        {
            posible = false;
        }

        if (posible)
        {
            m_x--;
        }
    }
}

void Bloque::moverDer(const Controller& controller, const Board& tablero, int limIzq, int limDer)
{
    if (controller.isRightPressed())
    {
        bool posible = true;
        for (int i = 0; i < 3; i++)
        {
            if (tablero.getCell(m_x + 1, m_y + i) != nullptr)
            {
                posible = false;
            }
        }

        if (m_x < limIzq || m_x >= limDer)
        {
            posible = false;
        }

        if (posible)
        {
            m_x++;
        }
    }
}

void Bloque::moverAbajo(const Controller& controller, const Board& tablero, int limSuelo)
{
    if (controller.isDownPressed())
    {
        if (m_y < limSuelo && tablero.getCell(m_x, m_y + 3) == nullptr)
        {
            m_y++;
        }
    }
}

void Bloque::rotarCaramelos(const Controller& controller)
{
    if (controller.isKey1Pressed())
    {
        Candy* aux = m_bloqueCaramelos[0];
        m_bloqueCaramelos[0] = m_bloqueCaramelos[1];
        m_bloqueCaramelos[1] = m_bloqueCaramelos[2];
        m_bloqueCaramelos[2] = aux;
    }
}