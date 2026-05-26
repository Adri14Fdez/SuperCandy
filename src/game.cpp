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

    m_bloqueCaramelos[0] = new Candy(CandyType::TYPE_RED);
    m_bloqueCaramelos[1] = new Candy(CandyType::TYPE_BLUE);
    m_bloqueCaramelos[2] = new Candy(CandyType::TYPE_GREEN);
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

    while (m_pause)
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
            for (int i = 0; i < 3; i++)
            {
                m_tablero.setCell(m_bloqueCaramelos[i], m_blockX, m_blockY + i);
                m_bloqueCaramelos[i] = nullptr;
            }

            // Explotamos los caramelos y los guardamos en un vector
            std::vector<Candy*> caramelosExplotados = m_tablero.explodeAndDrop();

            // Liberamos la memoria de los caramelos que acaban de explotar
            for (size_t i = 0; i < caramelosExplotados.size(); i++)
            {
                delete caramelosExplotados[i];
            }

            m_blockX = 5;
            m_blockY = -3;

            m_bloqueCaramelos[0] = new Candy(CandyType::TYPE_RED);
            m_bloqueCaramelos[1] = new Candy(CandyType::TYPE_BLUE);
            m_bloqueCaramelos[2] = new Candy(CandyType::TYPE_GREEN);
        }

        cout << "X: " << m_blockX << " Y: " << m_blockY << endl;
    }
}

void Game::render(GraphicManager& graphics)
{
    // Implement your code here
    
    // Note: the following code exhibits the main graphic library features
    // Board: border [draw rectangles] and a single piece of candy
    const int board_size = 10;
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
    graphics.drawText("Score: " + to_string(m_score), 450, 10, 70, 125, 200, 125);
    graphics.drawText("Segundos: " + to_string(m_frameCounter / 60), 25, 675, 20, 100, 100, 100);

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
    return false;
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

