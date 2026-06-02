#ifndef STATS_H
#define STATS_H

#include "candy.h"
#include <fstream>

class Stats
{
public:
    Stats();
    
    // Pone todas las estadísticas a 0
    void reset();
    
    // Suma 1 al contador del tipo de caramelo especificado
    void addCandy(CandyType type);
    
    // Devuelve la cantidad de caramelos explotados por índice (0-5)
    int getCount(int i) const;
    
    // Permite establecer un valor directamente (útil para el load)
    void setCount(int i, int count);
    
    // Guarda las estadísticas en el archivo de guardado
    void dump(std::ofstream& file) const;

private:
    int m_contadorCaramelos[6];
};

#endif