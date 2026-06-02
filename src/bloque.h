#ifndef BLOQUE_H
#define BLOQUE_H

#include "controller.h"
#include "candy.h"
#include "board.h"

class Bloque
{
public:
    Bloque();
    ~Bloque();
    void nuevoBloque();

    int getX() const { return m_x; }
    int getY() const { return m_y; }
    Candy* getCandy(int index) const { return m_bloqueCaramelos[index]; }

    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setCandy(int index, Candy* candy) { m_bloqueCaramelos[index] = candy; }

    bool puedeCaer(const Board& tablero, int lim);
    void moverAbajo(const Controller& controller, const Board& tablero, int limSuelo);
    void moverIzq(const Controller& controller, const Board& tablero, int limIzq, int limDer);
    void moverDer(const Controller& controller, const Board& tablero, int limIzq, int limDer);
    void rotarCaramelos(const Controller& controller);
private:
    Candy* m_bloqueCaramelos[3];
    int m_x, m_y;
};

#endif