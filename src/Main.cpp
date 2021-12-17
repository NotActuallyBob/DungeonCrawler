#include "Platform/Platform.hpp"
#include "game.hpp"

int main()
{
    Game dungeonCrawler;
    // Game loop
    while (dungeonCrawler.Running()) {
        dungeonCrawler.UpdateGame();
        dungeonCrawler.RenderGame();
    }
    return 0;
}
