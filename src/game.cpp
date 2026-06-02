#include "game.h"
#include "graphics.h"
#include "candy.h"
#include <fstream>
#include <string>
#include "util.h"

Game::Game()
{
    // Implement your code here
    m_frameCounter = 0;
    m_gameOver = false;

    // Al instanciar m_bloqueCaramelos, su propio constructor 
    // ya se ha encargado de generar los caramelos iniciales.
}

Game::~Game()
{
    // Implement your code here
}

void Game::update(const Controller& controller)
{
    
    // Implement your code here
    if (m_gameOver)
    {
        return;
    }

    saveAndLoad(controller);

    pauseCheck(controller);

    if (m_pause)
    {
        return;
    }  

    m_frameCounter++;

    m_bloqueCaramelos.moverIzq(controller, m_tablero, m_limIzq);
    m_bloqueCaramelos.moverDer(controller, m_tablero, m_limDer);
    m_bloqueCaramelos.rotarCaramelos(controller);

    //Decidimos si el bloque debe caer en este frame (por gravedad normal o por pulsar abajo)
    bool tocaCaer = (m_frameCounter % 60 == 0) || controller.isDownPressed();

    if (tocaCaer)
    {
        if (m_bloqueCaramelos.puedeCaer(m_tablero, m_limSuelo))
        {
            m_bloqueCaramelos.setY(m_bloqueCaramelos.getY() + 1);
        }
        else
        {
            if (!checkGameOver())
            {
                for (int i = 0; i < 3; i++)
                {
                    m_tablero.setCell(m_bloqueCaramelos.getCandy(i), m_bloqueCaramelos.getX(), m_bloqueCaramelos.getY() + i);
                    m_bloqueCaramelos.setCandy(i, nullptr);
                }

                // Explotamos los caramelos y los guardamos en un vector
                std::vector<Candy*> caramelosExplotados = m_tablero.explodeAndDrop();

                //Actualizar el score PROVISIONAL
                m_score += (caramelosExplotados.size() * 10);

                // Liberamos la memoria de los caramelos que acaban de explotar
                for (size_t i = 0; i < caramelosExplotados.size(); i++)
                {
                    m_explodedCandiesCount[static_cast<int>(caramelosExplotados[i]->getType())]++;
                    delete caramelosExplotados[i];
                }

                m_bloqueCaramelos.nuevoBloque();
            }            
        }

        cout << "X: " << m_bloqueCaramelos.getX() << " Y: " << m_bloqueCaramelos.getY() << endl;
    }
}

void Game::render(GraphicManager& graphics)
{
    // Implement your code here
    
    // Note: the following code exhibits the main graphic library features
    // Board: border [draw rectangles] and a single piece of candy

    // Con board_size = m_tablero.getHeight() conseguimos que si el tablero tiene una
    // medida diferente se sigan dibujando bien los limites.
    const int board_size = m_tablero.getHeight();
    const int board_padding = 3;
    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * board_padding, CANDY_IMAGE_HEIGHT * board_padding,
        CANDY_IMAGE_WIDTH * board_size,
        CANDY_IMAGE_HEIGHT * board_size,
        5, 150, 150, 150);
    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [draw text]
    graphics.drawText("Movement: [↓] [←] [→]  --  "
                      "Rotate: [Q] -- Pause: [E]  --  Exit [ESC]",
                      25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: " + to_string(m_score), 450, 10, 54, 125, 200, 125);
    graphics.drawText("Save: [W] -- Load: [Mouse Right]", 25, 675, 20, 100, 100, 100);
    graphics.drawText("Segundos: " + to_string(m_frameCounter / 60), 25, 650, 20, 100, 100, 100);

    if (m_pause)
    {
        graphics.drawText("PAUSA", 15, 130, 32, 125, 200, 125);
    }

    // Dibuja las estadisticas
    graphics.drawText("Explotados:", 7, 200, 24, 100, 100, 100);
    graphics.drawText("Rojos: " + to_string(m_explodedCandiesCount[0]), 7, 230, 20, 255, 0, 0);
    graphics.drawText("Azules: " + to_string(m_explodedCandiesCount[1]), 7, 260, 20, 0, 0, 255);
    graphics.drawText("Verdes: " + to_string(m_explodedCandiesCount[2]), 7, 290, 20, 0, 200, 0);
    graphics.drawText("Amarillos: " + to_string(m_explodedCandiesCount[3]), 7, 320, 20, 200, 200, 0);
    graphics.drawText("Lilas: " + to_string(m_explodedCandiesCount[4]), 7, 350, 20, 128, 0, 128);
    graphics.drawText("Naranjas: " + to_string(m_explodedCandiesCount[5]), 7, 380, 20, 255, 165, 0);

    // Dibuja los caramelos del tablero
    for (int x = 0; x < m_tablero.getWidth(); x++)
    {
        for (int y = 0; y < m_tablero.getHeight(); y++)
        {
            Candy* candy = m_tablero.getCell(x, y);
            if (candy != nullptr)
            {
                int pixelX = (CANDY_IMAGE_WIDTH * board_padding) + (x * CANDY_IMAGE_WIDTH);
                int pixelY = (CANDY_IMAGE_HEIGHT * board_padding) + (y * CANDY_IMAGE_HEIGHT);
                graphics.drawImage(candy->getResourceName(), pixelX, pixelY);
            }
        }
    }

    // Dibuja los caramelos del bloqueCaramelos
    for (int i = 0; i < 3; i++)
    {
        if (m_bloqueCaramelos.getCandy(i) != nullptr)
        {
            int pixelX = (CANDY_IMAGE_WIDTH * board_padding) + (m_bloqueCaramelos.getX() * CANDY_IMAGE_WIDTH);
            int pixelY = (CANDY_IMAGE_HEIGHT * board_padding) + ((m_bloqueCaramelos.getY() + i) * CANDY_IMAGE_HEIGHT);

            graphics.drawImage(m_bloqueCaramelos.getCandy(i)->getResourceName(), pixelX, pixelY);
        }
    }

    if (m_gameOver)
    {
        graphics.drawText("HAS PERDIDO", 160, 300, 64, 255, 0, 0);
    }
}

void Game::run()
{
    const int screen_width = 750;
    const int screen_height = 750;
    const int bg_red = 255;
    const int bg_green = 255;
    const int bg_blue = 255;
    runGraphicGame(*this, screen_width, screen_height, bg_red, bg_green, bg_blue);
}

bool Game::dump(const std::string& output_path) const
{
    // Implement your code here
    if (!m_tablero.dump(output_path))
    {
        return false; //si no encuentra el archivo devuelve false
    }

    ofstream gameDump(output_path, std::ofstream::app); //usamos app de append, para que los datos del tablero que ya estaban no se borren
    if (gameDump.is_open())
    {
        gameDump << "SCORE " << m_score << "\n";
        gameDump << "FRAME " << m_frameCounter << "\n";
        gameDump << "BLOCKX " << m_bloqueCaramelos.getX() << "\n";
        gameDump << "BLOCKY " << m_bloqueCaramelos.getY() << "\n";
        gameDump << "GAMEOVER " << m_gameOver << "\n";
        gameDump << "PAUSE " << m_pause << "\n";

        for (int i = 0; i < 6; i++) 
        {
            gameDump << "STAT" << i << " " << m_explodedCandiesCount[i] << "\n";
        }

        for (int i = 0; i < 3; i++) 
        {
            if (m_bloqueCaramelos.getCandy(i) != nullptr)
            {
                gameDump << "CANDY" << i << " " << gameTipoAString(m_bloqueCaramelos.getCandy(i)->getType()) << "\n";
            }
            else
            {
                gameDump << "CANDY" << i << " VACIO\n"; //por si acaso no hay caramelo
            }
        }

        gameDump.close();
        return true;
    }

    return false;

}

bool Game::load(const std::string& input_path)
{
    // Implement your code here
    ifstream gameLoad(input_path);

    // Solo se ejecuta si se ha podido abrir bien el archivo.
    if (gameLoad)
    {
        // Reiniciamos las estadisticas
        for (int i = 0; i < 6; i++) {
            m_explodedCandiesCount[i] = 0;
        }

        // Borramos todo lo que habia en el tablero.
        for (int i = 0; i < 3; i++)
        {
            Candy* actual = m_bloqueCaramelos.getCandy(i);
            if (actual != nullptr)
            {
                delete actual; // Como usamos memoria dinamica al cargar, tenemos que borrarlo manualmente.
                m_bloqueCaramelos.setCandy(i, nullptr);
            }
        }

        for (int c = 0; c < m_tablero.getWidth(); c++)
        {
            for (int f = 0; f < m_tablero.getHeight(); f++)
            {
                Candy* actual = m_tablero.getCell(c, f);
                if (actual != nullptr)
                {
                    delete actual; // Como usamos memoria dinamica al cargar, tenemos que borrarlo manualmente.
                    m_tablero.setCell(nullptr, c, f);
                }
            }
        }

        string clave;
        int c, f;

        //El bucle se repite mientras queden lineas por leer.
        while (gameLoad >> clave)
        {
            // Dependiendo del "titulo" que lea establecemos una variable o no.
            if (clave == "SCORE")
            {
                int temp;
                gameLoad >> temp;
                m_score = temp;
            }
            else if (clave == "FRAME")
            {
                int temp;
                gameLoad >> temp;
                m_frameCounter = temp;
            }
            else if (clave == "BLOCKX")
            {
                int temp;
                gameLoad >> temp;
                m_bloqueCaramelos.setX(temp);
            }
            else if (clave == "BLOCKY")
            {
                int temp;
                gameLoad >> temp;
                m_bloqueCaramelos.setY(temp);
            }
            else if (clave == "GAMEOVER")
            {
                int temp;
                gameLoad >> temp;
                m_gameOver = temp;
            }
            else if (clave == "PAUSE")
            {
                int temp;
                gameLoad >> temp;
                m_pause = temp;
            }
            else if (clave == "CANDY0")
            {
                string temp;
                gameLoad >> temp;
                m_bloqueCaramelos.setCandy(0, new Candy(gameStringATipo(temp)));
            }
            else if (clave == "CANDY1")
            {
                string temp;
                gameLoad >> temp;
                m_bloqueCaramelos.setCandy(1, new Candy(gameStringATipo(temp)));
            }
            else if (clave == "CANDY2")
            {
                string temp;
                gameLoad >> temp;
                m_bloqueCaramelos.setCandy(2, new Candy(gameStringATipo(temp)));
            }
            else if (clave == "STAT0")
            {
                gameLoad >> m_explodedCandiesCount[0];
            }
            else if (clave == "STAT1")
            {
                gameLoad >> m_explodedCandiesCount[1];
            }
            else if (clave == "STAT2")
            {
                gameLoad >> m_explodedCandiesCount[2];
            }
            else if (clave == "STAT3")
            {
                gameLoad >> m_explodedCandiesCount[3];
            }
            else if (clave == "STAT4")
            {
                gameLoad >> m_explodedCandiesCount[4];
            }
            else if (clave == "STAT5")
            {
                gameLoad >> m_explodedCandiesCount[5];
            }
            else
            {
                // Antes usaba m_tablero.load(input_path) pero el gemini me decia
                // que de esa forma se abre el archivo por segunda vez y se corromperia,
                // entonces he copiado el Board::load aqui y ya :/
                gameLoad >> c >> f;
                CandyType tipoEnum = gameStringATipo(clave);

                if (tipoEnum != CandyType::COUNT)
                {
                    Candy* nuevoCaramelo = new Candy(tipoEnum);
                    m_tablero.setCell(nuevoCaramelo, c, f);
                }
            }
        }

        gameLoad.close();
        return true;
    }
    else
    {
        cout << "Error de apertura del archivo." << endl;
        return false;
    }
    return false;
}

bool Game::operator==(const Game& other) const
{
    // Implement your code here
    bool esIgual = true;

    // Comprobar variables basicas
    if ((m_gameOver != other.m_gameOver) ||
        ( m_score != other.m_score) ||
        (m_frameCounter != other.m_frameCounter) ||
        (m_bloqueCaramelos.getX() != other.m_bloqueCaramelos.getX()) ||
        (m_bloqueCaramelos.getY() != other.m_bloqueCaramelos.getY()) ||
        (m_pause != other.m_pause))
    {
        esIgual = false;
    }

    // Comprobar bloqueCaramelos
    for (int i = 0; i < 3 && esIgual; i++)
    {
        Candy* miCaramelo = m_bloqueCaramelos.getCandy(i);
        Candy* suCaramelo = other.m_bloqueCaramelos.getCandy(i);

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

    // Comprobar tablero
    if (!(m_tablero == other.m_tablero))
    {
        esIgual = false;
    }

    return esIgual;
}

void Game::pauseCheck(const Controller& controller)
{
    if (m_pause)
    {
        if (controller.isKey3Pressed())
        {
            m_pause = false;
        }
    }
    else
    {
        if (controller.isKey3Pressed())
        {
            m_pause = true;
        }
    }
}

void Game::saveAndLoad(const Controller& controller)
{
    if (controller.isKey2Pressed())
    {
        dump(getDataDirPath() + "/save.txt");
    }

    if (controller.isMouseRightPressed())
    {
        load(getDataDirPath() + "/save.txt");
    }
}

bool Game::checkGameOver()
{
    bool piezaFuera = false;

    for (int i = 0; i < 3; i++)
    {
        int posPieza = m_bloqueCaramelos.getY() + i;

        if (posPieza < 0)
        {
            piezaFuera = true;
            delete m_bloqueCaramelos.getCandy(i);
            m_bloqueCaramelos.setCandy(i, nullptr);
        }
    }

    if (piezaFuera)
    {
        m_gameOver = true;
    }

    return m_gameOver;
}

string Game::gameTipoAString(CandyType type) const //func auxiliar para el dump
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

CandyType Game::gameStringATipo(const std::string& type) const //func auxiliar para el load
{
    if (type == "AZUL") return CandyType::TYPE_BLUE;
    if (type == "VERDE") return CandyType::TYPE_GREEN;
    if (type == "NARANJA") return CandyType::TYPE_ORANGE;
    if (type == "LILA") return CandyType::TYPE_PURPLE;
    if (type == "ROJO") return CandyType::TYPE_RED;
    if (type == "AMARILLO") return CandyType::TYPE_YELLOW;

    return CandyType::COUNT;
}