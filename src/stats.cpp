#include "stats.h"

Stats::Stats()
{
    reset();
}

void Stats::reset()
{
    for (int i = 0; i < 6; i++)
    {
        m_contadorCaramelos[i] = 0;
    }
}

void Stats::addCandy(CandyType type)
{
    // Transformamos el tipo a int para usarlo como indice del array
    int index = static_cast<int>(type);
    if (index >= 0 && index < 6)
    {
        m_contadorCaramelos[index]++;
    }
}

int Stats::getCount(int i) const
{
    if (i >= 0 && i < 6)
    {
        return m_contadorCaramelos[i];
    }
    return 0;
}

void Stats::setCount(int i, int count)
{
    if (i >= 0 && i < 6)
    {
        m_contadorCaramelos[i] = count;
    }
}

void Stats::dump(std::ofstream& file) const
{
    for (int i = 0; i < 6; i++) 
    {
        file << "STAT" << i << " " << m_contadorCaramelos[i] << "\n";
    }
}