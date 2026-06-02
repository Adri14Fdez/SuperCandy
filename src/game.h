/**
 * Game logic and rendering implementation. 
 */
#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "controller.h"

#include "candy.h"
#include "board.h"

/**
 * Main game class: keep track of the game state it. 
 * When run_graphic_game() is called, the game loop will call
 * update() and render() every frame, in that order.
 */
class Game
{
public:
    Game();

    ~Game();

    /// Run the game loop
    void run();

    /**
     * Update the game state. Called every frame when run().
     * This part is not expected to do any rendering.
     * 
     * @param controller the Controller to use for input handling.
     */
    void update(const Controller& controller);

    /**
     * Draw the next frame. Called once per frame, after update().
     * 
     * @param graphics the GraphicManager to use for rendering.
     */
    void render(GraphicManager& graphics);

    /**
     * Serialize and write the current game state (board and falling block) to a file.
     * @param output_path path where to save the game state.
     * @return true if the dump was successful, false otherwise.
     */
    bool dump(const std::string& output_path) const;

    /**
     * Load a serialized game state (board and falling block) from a file created with dump().
     * @param input_path path from which to load the game state.
     * @return true if the load was successful, false otherwise.
     */
    bool load(const std::string& input_path);

    /// @return true if this game is equal to the other game (same board state and falling block)
    bool operator==(const Game& other) const;

private:
    int m_frameCounter = 0;
    int m_score = 0;
    
    // Estadisticas: numero de caramelos explotados de cada tipo
    int m_explodedCandiesCount[6] = {0};

    int m_blockX;
    int m_blockY;
    Candy* m_bloqueCaramelos[3];

    bool m_gameOver = false;
    bool m_pause = false;

    Board m_tablero;
    int m_limDer = m_tablero.getWidth() - 1;
    int m_limIzq = 0;
    int m_limSuelo = m_tablero.getHeight() - 3;

    void moverIzq(const Controller& controller);
    void moverDer(const Controller& controller);
    void moverAbajo(const Controller& controller);
    void rotarCaramelos(const Controller& controller);
    void pauseCheck(const Controller& controller);
    void saveAndLoad(const Controller& controller);
    bool checkGameOver();
    void nuevoBloque();

    string gameTipoAString(CandyType type) const; //func auxiliar para el dump
    CandyType gameStringATipo(const std::string& type) const; //func auxiliar para el load
};
#endif
