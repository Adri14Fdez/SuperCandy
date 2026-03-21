#include <filesystem>
#include<iostream>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"

bool test() {
    bool resultado = true;

    // Test getters tablero
    {
        Board tableroGHGW(10, 10);

        if (tableroGHGW.getHeight() != 10)
        {
            cout << "Fallo en getHeight." << endl;
            resultado = false;
        }
        else if (tableroGHGW.getWidth() != 10)
        {
            cout << "Fallo en getWidth." << endl;
            resultado = false;
        }
        else
        {
            cout << "Funciones getHeight y getWidth funcionan correctamente." << endl;
        }
    }
    cout << "--------------------------------------------------" << endl;

    // Test getCell y setCell
    {

        // Caso valido
        {
            Board tableroGCSCV(10, 10);

            Candy* carameloRojo = new Candy(CandyType::TYPE_RED);
            tableroGCSCV.setCell(carameloRojo, 5, 5);
            if (tableroGCSCV.getCell(5, 5) == carameloRojo)
            {
                cout << "setCell y getCell funcionan en casos validos." << endl;
            }
            else
            {
                cout << "setCell y getCell fallan en casos validos." << endl;
                resultado = false;
            }
        }
        cout << "--------------------------------------------------" << endl;

        // Casos invalidos
        {
            // Negativos
            Board tableroGCSCI(10, 10);

            Candy* carameloRojo = new Candy(CandyType::TYPE_RED);
            tableroGCSCI.setCell(carameloRojo, -5, 5);
            if (tableroGCSCI.getCell(-5, 5) == nullptr)
            {
                cout << "setCell y getCell funcionan en casos invalidos negativos." << endl;
            }
            else
            {
                cout << "setCell y getCell fallan en casos invalidos negativos." << endl;
                resultado = false;
            }

            // Mayores
            tableroGCSCI.setCell(carameloRojo, 5, 15);
            if (tableroGCSCI.getCell(5, 15) == nullptr)
            {
                cout << "setCell y getCell funcionan en casos invalidos mayores." << endl;
            }
            else
            {
                cout << "setCell y getCell fallan en casos invalidos mayores." << endl;
                resultado = false;
            }

            // Como no se llega a cargar, no se eliminara con el destructor por lo que lo eliminamos manualmente.
            delete carameloRojo;
        }
        cout << "--------------------------------------------------" << endl;
    }

    // Test shouldExplode
    {
        // Casos verdaderos
        {
            // Horizontal
            {
                Board tableroSEH(10, 10);

                Candy* carameloRojo1 = new Candy(CandyType::TYPE_RED);
                Candy* carameloRojo2 = new Candy(CandyType::TYPE_RED);
                Candy* carameloRojo3 = new Candy(CandyType::TYPE_RED);
                tableroSEH.setCell(carameloRojo1, 2, 9);
                tableroSEH.setCell(carameloRojo2, 3, 9);
                tableroSEH.setCell(carameloRojo3, 4, 9);
                if (tableroSEH.shouldExplode(2, 9) && tableroSEH.shouldExplode(3, 9) && tableroSEH.shouldExplode(4, 9))
                {
                    cout << "shouldExplode funciona en casos validos horizontales." << endl;
                }
                else
                {
                    cout << "shouldExplode falla en casos validos horizontales." << endl;
                    resultado = false;
                }
            }
            // Vertical
            {
                Board tableroSEV(10, 10);

                Candy* carameloRojo1 = new Candy(CandyType::TYPE_RED);
                Candy* carameloRojo2 = new Candy(CandyType::TYPE_RED);
                Candy* carameloRojo3 = new Candy(CandyType::TYPE_RED);
                tableroSEV.setCell(carameloRojo1, 2, 9);
                tableroSEV.setCell(carameloRojo2, 3, 9);
                tableroSEV.setCell(carameloRojo3, 4, 9);
                if (tableroSEV.shouldExplode(9, 2) && tableroSEV.shouldExplode(9, 3) && tableroSEV.shouldExplode(9, 4))
                {
                    cout << "shouldExplode funciona en casos validos verticales." << endl;
                }
                else
                {
                    cout << "shouldExplode falla en casos validos verticales." << endl;
                    resultado = false;
                }
            }
            // Diagonal
            {
                Board tableroSED(10, 10);

                Candy* carameloRojo1 = new Candy(CandyType::TYPE_RED);
                Candy* carameloRojo2 = new Candy(CandyType::TYPE_RED);
                Candy* carameloRojo3 = new Candy(CandyType::TYPE_RED);
                tableroSED.setCell(carameloRojo1, 2, 9);
                tableroSED.setCell(carameloRojo2, 3, 9);
                tableroSED.setCell(carameloRojo3, 4, 9);
                if (tableroSED.shouldExplode(2, 9) && tableroSED.shouldExplode(3, 8) && tableroSED.shouldExplode(4, 7))
                {
                    cout << "shouldExplode funciona en casos validos diagonales." << endl;
                }
                else
                {
                    cout << "shouldExplode falla en casos validos diagonales." << endl;
                    resultado = false;
                }
            }

        }
        cout << "--------------------------------------------------" << endl;
        // Casos falsos
        {
            // Celda vacia.
            {
                Board tableroSECV(10, 10);

                if (tableroSECV.shouldExplode(0, 0))
                {
                    cout << "shouldExplode falla en casos invalidos con coordenada vacia." << endl;
                    resultado = false;
                }
                else {
                    cout << "shouldExplode funciona en casos invalidos con coordenada vacia." << endl;
                }
            }

            // Caramelo rodeado de caramelos de colores distintos.
            {
                Board tableroSECR(10, 10);

                Candy* carameloCentral = new Candy(CandyType::TYPE_RED);
                Candy* carameloArriba = new Candy(CandyType::TYPE_BLUE);
                Candy* carameloAbajo = new Candy(CandyType::TYPE_GREEN);
                Candy* carameloIzquierda = new Candy(CandyType::TYPE_YELLOW);
                Candy* carameloDerecha = new Candy(CandyType::TYPE_PURPLE);

                tableroSECR.setCell(carameloCentral, 5, 5);
                tableroSECR.setCell(carameloArriba, 5, 4);
                tableroSECR.setCell(carameloAbajo, 5, 6);
                tableroSECR.setCell(carameloIzquierda, 4, 5);
                tableroSECR.setCell(carameloDerecha, 6, 5);

                if (tableroSECR.shouldExplode(5, 5))
                {
                    cout << "shouldExplode falla en casos invalidos con caramelo rodeado de otros caramelos." << endl;
                    resultado = false;
                }
                else {
                    cout << "shouldExplode funciona en casos invalidos con caramelo rodeado de otros caramelos." << endl;
                }
            }
        }
        cout << "--------------------------------------------------" << endl;
    }

    // Test explodeAndDrop
    {
        Board tableroEAD(10, 10);

        // 3 iguales en el suelo que deben explotar.
        Candy* exp1 = new Candy(CandyType::TYPE_RED);
        Candy* exp2 = new Candy(CandyType::TYPE_RED);
        Candy* exp3 = new Candy(CandyType::TYPE_RED);
        tableroEAD.setCell(exp1, 5, 9);
        tableroEAD.setCell(exp2, 6, 9);
        tableroEAD.setCell(exp3, 7, 9);

        // Caramelos sueltos justo encima de ellos que deben caer.
        Candy* drop1 = new Candy(CandyType::TYPE_BLUE);
        Candy* drop2 = new Candy(CandyType::TYPE_GREEN);
        tableroEAD.setCell(drop1, 5, 8);
        tableroEAD.setCell(drop2, 6, 8);

        // Llamamos a la funcion
        vector<Candy*> destruidos = tableroEAD.explodeAndDrop();

        // Comprobamos que el vector contiene el numero de caramelos que deberian explotar.
        if (destruidos.size() != 3)
        {
            cout << "explodeAndDrop falla porque el vector devuelto no tiene 3 elementos (tiene " << destruidos.size() << ")." << endl;
            resultado = false;
        }

        // Comprobamos que las posiciones destruidas han quedado ocupadas por los caramelos que caian 
        // y que las posiciones origen de los caramelos que caian han quedado vacias.
        if (tableroEAD.getCell(5, 9) != drop1 || tableroEAD.getCell(6, 9) != drop2 || tableroEAD.getCell(7, 9) != nullptr
            || tableroEAD.getCell(5, 8) != nullptr || tableroEAD.getCell(6, 8) != nullptr)
        {
            cout << "explodeAndDrop falla porque los caramelos no han caido correctamente." << endl;
            resultado = false;
        } else
        {
            cout << "explodeAndDrop funciona correctamente." << endl;
        }

        // Borramos los caramelos destruidos.
        for (Candy* c : destruidos)
        {
            delete c;
        }
    }
    cout << "--------------------------------------------------" << endl;

    // Test dump y load
    {
        // Caso valido.
        {
            // Creamos un tablero y le añadimos caramelos.
            Board tableroDL;
            Candy* c1 = new Candy(CandyType::TYPE_RED);
            Candy* c2 = new Candy(CandyType::TYPE_GREEN);
            tableroDL.setCell(c1, 6, 7);
            tableroDL.setCell(c2, 8, 9);

            // Exportamos el tablero.
            tableroDL.dump("test_dump.txt");

            // Creamos un nuevo tablero cargando el anterior.
            Board tableroDL2;
            tableroDL2.load("test_dump.txt");

            // Comprobamos que los caramelos se han cargado correctamente.
            if (tableroDL2.getCell(6, 7) == nullptr || 
                tableroDL2.getCell(8, 9) == nullptr ||
                tableroDL.getCell(6, 7)->getType() != tableroDL2.getCell(6, 7)->getType() || 
                tableroDL.getCell(8, 9)->getType() != tableroDL2.getCell(8, 9)->getType())
            {
                cout << "dump y load fallan al guardar." << endl;
                resultado = false;
            } else
            {
                cout << "dump y load funcionan al guardar." << endl;
            }

            // Borramos los caramelos.
            delete c1;
            delete c2;
        }
        // Caso invalido.
        {
            Board tableroDL;
            if (tableroDL.load("noExiste.txt") == true)
            {
                cout << "dump y load fallan al cargar." << endl;
                resultado = false;
            } else
            {
                cout << "dump y load funcionan al cargar." << endl;
            }
        }
    }
    cout << "--------------------------------------------------" << endl;
    return resultado;
}