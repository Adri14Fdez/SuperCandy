#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"

bool test() {
    Board b(10, 10);

    if (b.getWidth() != 10) {
        return false;
    }
    if (b.getHeight() != 10) {
        return false;
    }

    Candy c(CandyType::TYPE_ORANGE);
    b.setCell(&c, 0, 0);
    if (b.getCell(0, 0) != &c) {
        return false;
    }

    return true;
}