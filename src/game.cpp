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

    m_blockX = 5;
    m_blockY = -3;

    //Generar los caramelos aleatoriamente

    srand(time(NULL));
    int numAleatorio = 0;

    for (int i = 0; i < 3; i++)
    {
        numAleatorio = rand() % 6; //Hacemos el módulo del número aleatorio con 6, para que nos de 1, 2, 3, 4 o 5.

        // Convertimos ese número entero a un tipo de caramelo (CandyType) y lo creamos
        m_bloqueCaramelos[i] = new Candy(CandyType(numAleatorio));
    }
}

Game::~Game()
{
    // Implement your code here
    for (int i = 0; i < 3; i++)
    {
        if (m_bloqueCaramelos[i] != nullptr)
        {
            delete m_bloqueCaramelos[i];
            m_bloqueCaramelos[i] = nullptr;
        }
    }
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

    moverIzq(controller);
    moverDer(controller);
    moverAbajo(controller);
    rotarCaramelos(controller);

    if (m_frameCounter % 60 == 0)
    {
        bool puedeCaer = true;
        int bottomY = m_blockY + 3;

        if (m_blockY >= m_limSuelo || bottomY >= 0 && m_tablero.getCell(m_blockX, bottomY) != nullptr)
        {
            puedeCaer = false;
        }

        if (puedeCaer)
        {
            m_blockY++;
        }
        else
        {
            if (!checkGameOver())
            {
                for (int i = 0; i < 3; i++)
                {
                    m_tablero.setCell(m_bloqueCaramelos[i], m_blockX, m_blockY + i);
                    delete m_bloqueCaramelos[i];
                    m_bloqueCaramelos[i] = nullptr;
                }

                // Explotamos los caramelos y los guardamos en un vector
                std::vector<Candy*> caramelosExplotados = m_tablero.explodeAndDrop();

                //Actualizar el score PROVISIONAL
                m_score += (caramelosExplotados.size() * 10);

                // Liberamos la memoria de los caramelos que acaban de explotar
                for (size_t i = 0; i < caramelosExplotados.size(); i++)
                {
                    m_stats.addExplodedCandy(caramelosExplotados[i]->getType());
                }

                nuevoBloque();
            }            
        }

        cout << "X: " << m_blockX << " Y: " << m_blockY << endl;
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
    graphics.drawText("Movement: [Down] [Left] [Right]  --  "
                      "Rotate: [Q] -- Pause: [E]  --  Exit [ESC]",
                      25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: " + to_string(m_score), 450, 10, 54, 125, 200, 125);
    graphics.drawText("Save: [Mouse Left] -- Load: [Mouse Right]", 25, 675, 20, 100, 100, 100);
    graphics.drawText("Segundos: " + to_string(m_frameCounter / 60), 25, 650, 20, 100, 100, 100);

    if (m_pause)
    {
        graphics.drawText("PAUSA", 15, 150, 32, 125, 200, 125);
    }

    // Dibuja las estadisticas
    graphics.drawText("Explotados:", 7, 170, 24, 100, 100, 100);
    graphics.drawText("Rojos: " + to_string(m_stats.getCandyCount(static_cast<CandyType>(0))), 7, 200, 20, 255, 0, 0);
    graphics.drawText("Azules: " + to_string(m_stats.getCandyCount(static_cast<CandyType>(1))), 7, 230, 20, 0, 0, 255);
    graphics.drawText("Verdes: " + to_string(m_stats.getCandyCount(static_cast<CandyType>(2))), 7, 260, 20, 0, 200, 0);
    graphics.drawText("Amarillos: " + to_string(m_stats.getCandyCount(static_cast<CandyType>(3))), 7, 290, 20, 200, 200, 0);
    graphics.drawText("Lilas: " + to_string(m_stats.getCandyCount(static_cast<CandyType>(4))), 7, 320, 20, 128, 0, 128);
    graphics.drawText("Naranjas: " + to_string(m_stats.getCandyCount(static_cast<CandyType>(5))), 7, 350, 20, 255, 165, 0);

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
        if (m_bloqueCaramelos[i] != nullptr)
        {
            int pixelX = (CANDY_IMAGE_WIDTH * board_padding) + (m_blockX * CANDY_IMAGE_WIDTH);
            int pixelY = (CANDY_IMAGE_HEIGHT * board_padding) + ((m_blockY + i) * CANDY_IMAGE_HEIGHT);

            graphics.drawImage(m_bloqueCaramelos[i]->getResourceName(), pixelX, pixelY);
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
        gameDump << "BLOCKX " << m_blockX << "\n";
        gameDump << "BLOCKY " << m_blockY << "\n";
        gameDump << "GAMEOVER " << m_gameOver << "\n";
        gameDump << "PAUSE " << m_pause << "\n";

        m_stats.dump(gameDump);

        for (int i = 0; i < 3; i++) 
        {
            if (m_bloqueCaramelos[i] != nullptr)
            {
                gameDump << "CANDY" << i << " " << gameTipoAString(m_bloqueCaramelos[i]->getType()) << "\n";
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
        m_stats.reset();

        // Borramos todo lo que habia en el tablero.
        for (int i = 0; i < 3; i++)
        {
            Candy* actual = m_bloqueCaramelos[i];
            if (actual != nullptr)
            {
                delete actual; // Como usamos memoria dinamica al cargar, tenemos que borrarlo manualmente.
                m_bloqueCaramelos[i] = nullptr;
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
                m_blockX = temp;
            }
            else if (clave == "BLOCKY")
            {
                int temp;
                gameLoad >> temp;
                m_blockY = temp;
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
                m_bloqueCaramelos[0] = new Candy(gameStringATipo(temp));
            }
            else if (clave == "CANDY1")
            {
                string temp;
                gameLoad >> temp;
                m_bloqueCaramelos[1] = new Candy(gameStringATipo(temp));
            }
            else if (clave == "CANDY2")
            {
                string temp;
                gameLoad >> temp;
                m_bloqueCaramelos[2] = new Candy(gameStringATipo(temp));
            }else if (clave.substr(0, 4) == "STAT")
            {
                m_stats.load(gameLoad, clave);
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
                    delete nuevoCaramelo;
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
        (m_blockX != other.m_blockX) ||
        (m_blockY != other.m_blockY) ||
        (m_pause != other.m_pause))
    {
        esIgual = false;
    }

    // Comprobar bloqueCaramelos
    for (int i = 0; i < 3 && esIgual; i++)
    {
        Candy* miCaramelo = m_bloqueCaramelos[i];
        Candy* suCaramelo = other.m_bloqueCaramelos[i];

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

void Game::moverIzq(const Controller& controller)
{
    if (controller.isLeftPressed())
    {
        bool posible = true;
        for (int i = 0; i < 3; i++)
        {
            if (m_tablero.getCell(m_blockX -1, m_blockY + i) != nullptr)
            {
                posible = false;
            }
        }

        if (m_blockX <= m_limIzq || m_blockX > m_limDer)
        {
            posible = false;
        }

        if (posible)
        {
            m_blockX--;
        }
    }
}

void Game::moverDer(const Controller& controller)
{
    if (controller.isRightPressed())
    {
        bool posible = true;
        for (int i = 0; i < 3; i++)
        {
            if (m_tablero.getCell(m_blockX + 1, m_blockY + i) != nullptr)
            {
                posible = false;
            }
        }

        if (m_blockX < m_limIzq || m_blockX >= m_limDer)
        {
            posible = false;
        }

        if (posible)
        {
            m_blockX++;
        }
    }
}

void Game::moverAbajo(const Controller& controller)
{
    if (controller.isDownPressed())
    {
        if (m_blockY < m_limSuelo && m_tablero.getCell(m_blockX, m_blockY + 3) == nullptr)
        {
            m_blockY++;
        }
    }
}

void Game::rotarCaramelos(const Controller& controller)
{
    if (controller.isKey1Pressed())
    {
        Candy* aux = m_bloqueCaramelos[0];
        m_bloqueCaramelos[0] = m_bloqueCaramelos[1];
        m_bloqueCaramelos[1] = m_bloqueCaramelos[2];
        m_bloqueCaramelos[2] = aux;
    }
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
    if (controller.isMouseLeftPressed())
    {
        dump(getDataDirPath() + "/test_dump.txt");
    }

    if (controller.isMouseRightPressed())
    {
        load(getDataDirPath() + "/test_dump.txt");
    }
}

bool Game::checkGameOver()
{
    bool piezaFuera = false;

    for (int i = 0; i < 3; i++)
    {
        int posPieza = m_blockY + i;

        if (posPieza < 0)
        {
            piezaFuera = true;
            delete m_bloqueCaramelos[i];
            m_bloqueCaramelos[i] = nullptr;
        }
    }

    if (piezaFuera)
    {
        m_gameOver = true;
    }

    return m_gameOver;
}

void Game::nuevoBloque()
{
    m_blockX = 5;
    m_blockY = -3;

    //Generar otra vez los caramelos aleatoriamente

    srand(time(NULL));
    int numAleatorio = 0;

    for (int i = 0; i < 3; i++)
    {
        numAleatorio = rand() % 6; //Hacemos el módulo del número aleatorio con 6, para que nos de 1, 2, 3, 4 o 5.

        // Convertimos ese número entero a un tipo de caramelo (CandyType) y lo creamos
        m_bloqueCaramelos[i] = new Candy(CandyType(numAleatorio));
    }
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