#ifndef STATS_H
#define STATS_H

#include "candy.h"
#include <fstream>

class Stats 
{
public:
    Stats();

    void addExplodedCandy(CandyType type);
    int getCandyCount(CandyType type) const;
    void reset();

    void dump(std::ofstream& out) const;
    void load(std::ifstream& in, const std::string& clave);

private:
    int m_caramelosExplotados[6];
};

#endif