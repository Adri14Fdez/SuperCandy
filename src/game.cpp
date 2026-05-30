#include "game.h"
#include <random>
#include "graphics.h"
#include "candy.h"

Game::Game()
{
    // Implement your code here
    m_frameCounter = 0;
    m_gameOver = false;

    m_blockX = 5;
    m_blockY = -3;

    //Generar los caramelos aleatoriamente

    std::random_device rd; //Genera una semilla random
    std::mt19937 gen(rd()); //Con la semilla generamos el número aleatorio
    int numAleatorio = 0;

    for (int i = 0; i < 3; i++)
    {
        numAleatorio = gen() % 6; //Hacemos el módulo del número aleatorio con 6, para que nos de 1, 2, 3, 4 o 5.

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

    if (m_pause)
    {
        if (controller.isMouseRightPressed())
        {
            m_pause = false;
        }
    }
    else
    {
        if (controller.isMouseRightPressed())
        {
            m_pause = true;
        }
    }

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
                    m_bloqueCaramelos[i] = nullptr;
                }

                // Explotamos los caramelos y los guardamos en un vector
                std::vector<Candy*> caramelosExplotados = m_tablero.explodeAndDrop();

                //Actualizar el score PROVISIONAL
                m_score += (caramelosExplotados.size() * 10);

                // Liberamos la memoria de los caramelos que acaban de explotar
                for (size_t i = 0; i < caramelosExplotados.size(); i++)
                {
                    delete caramelosExplotados[i];
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
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
                      "Buttons: [Q] [W] [E]  --  Exit [ESC]",
                      25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: " + to_string(m_score), 450, 10, 54, 125, 200, 125);
    graphics.drawText("Pause: [Mouse Right]", 25, 675, 20, 100, 100, 100);
    graphics.drawText("Segundos: " + to_string(m_frameCounter / 60), 25, 650, 20, 100, 100, 100);

    if (m_pause)
    {
        graphics.drawText("PAUSA", 15, 150, 32, 125, 200, 125);
    }

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
    return false;
}

bool Game::load(const std::string& input_path)
{
    // Implement your code here
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

    std::random_device rd; //Genera una semilla random
    std::mt19937 gen(rd()); //Con la semilla generamos el número aleatorio
    int numAleatorio = 0;

    for (int i = 0; i < 3; i++)
    {
        numAleatorio = gen() % 6; //Hacemos el módulo del número aleatorio con 6, para que nos de 1, 2, 3, 4 o 5.

        // Convertimos ese número entero a un tipo de caramelo (CandyType) y lo creamos
        m_bloqueCaramelos[i] = new Candy(CandyType(numAleatorio));
    }
}