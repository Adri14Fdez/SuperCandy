/// Project entry point
#include <iostream>
#include "game.h"
#include "../test/test.h"

int main(int argc, const char* argv[])
{
    if (!test())
    {
        std::cout << "Tests NO superados. Los has definido ya?" << std::endl;
    }
    else
    {
        std::cout << "Test superados." << std::endl;
    }

    // The following code runs the graphic part
    Game game;

    // Si se le pasa un argumento por la terminal, carga
    // la partida antes de empezar a jugar
    if (argc > 1)
    {
        game.load(argv[1]);
    }

    // Run until ESC is pressed
    game.run();
    return 0;
}
