#include "stats.h"

Stats::Stats()
{
    reset();
}

void Stats::addExplodedCandy(CandyType type)
{
    int index = static_cast<int>(type);
    if (index >= 0 && index < 6)
    {
        m_caramelosExplotados[index]++;
    }
}

int Stats::getCandyCount(CandyType type) const
{
    int index = static_cast<int>(type);
    if (index >= 0 && index < 6)
    {
        return m_caramelosExplotados[index];
    }
    return 0;
}

void Stats::reset()
{
    for (int i = 0; i < 6; i++)
    {
        m_caramelosExplotados[i] = 0;
    }
}

void Stats::dump(std::ofstream& out) const
{
    for (int i = 0; i < 6; i++) 
    {
        out << "STAT" << i << " " << m_caramelosExplotados[i] << "\n";
    }
}

void Stats::load(std::ifstream& in, const std::string& clave)
{
    if (clave == "STAT0") in >> m_caramelosExplotados[0];
    else if (clave == "STAT1") in >> m_caramelosExplotados[1];
    else if (clave == "STAT2") in >> m_caramelosExplotados[2];
    else if (clave == "STAT3") in >> m_caramelosExplotados[3];
    else if (clave == "STAT4") in >> m_caramelosExplotados[4];
    else if (clave == "STAT5") in >> m_caramelosExplotados[5];
}