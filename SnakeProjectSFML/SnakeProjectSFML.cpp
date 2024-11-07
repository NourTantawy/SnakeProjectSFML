#include <SFML/Graphics.hpp>
#include "Game.h"


using namespace sf;

int main() {

    srand(static_cast<unsigned>(time(NULL)));

    Game game;
    game.Run();
   


    return 0;
}