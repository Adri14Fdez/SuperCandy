#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"

bool test() {
    Board miTablero(10, 10);

    if (miTablero.getWidth() != 10) {
        return false;
    }
    if (miTablero.getHeight() != 10) {
        return false;
    }

    return true;
}