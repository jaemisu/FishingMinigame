/*********************************************
SFML Fishing Minigame Window
cc: Sets up the game window, background, and click detection.
*********************************************/

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <iostream>
#include <vector>
#include <map>


// CONSTANTS & GLOBALS
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

// CLICKLABLE POSITIONS
sf::IntRect playbuttonUI({ 380, 480 }, { 430, 130 });
sf::IntRect exitbuttonUI({ 15, 15 }, { 100, 60 });

// POSTION LISTS
std::vector<sf::IntRect> xyMasterList = { playbuttonUI, exitbuttonUI };
std::vector<sf::IntRect> xyAvailableList = { playbuttonUI, exitbuttonUI };

// PNGS -> TEXTURES
sf::Texture titleTexture("sprites/TitleScreen.png");
sf::Texture backgroundTexture("sprites/BackgroundImage.png");

// TEXTURES -> SPRITES
sf::Sprite titleSprite(titleTexture);
sf::Sprite backgroundSprite(backgroundTexture);

// SPRITE LIST (for ui management)
std::vector<sf::Sprite> allSprites = { backgroundSprite, titleSprite };

// FUNCTIONS
void playFunction()
{
    //put a function here that closes the title screen and spawns in the bulk of the game
    //part of the function is also supposed to remove the "playbuttonUI" variable from the xyAvailableList
}

void exitFunction()
{
    exit(1);
}

// CLICK CHECKING FUNCTION
int clickCheck(sf::Vector2i mousePos)
{
    std::size_t z = 0;
    std::size_t x = 0;

    for (std::size_t x = 0; x < xyAvailableList.size(); x++)
    {
        if (xyAvailableList[x].contains(mousePos))
        {
            for (std::size_t z = 0; z < xyMasterList.size(); z++)
            {
                if (xyAvailableList[x] == xyMasterList[z])
                    return z;
            }
        }
    }
    return -1;
}

//MAIN GAME
int main()
{
    // FUNCTION MAPPING
    std::map<int, std::function<void()>> functionMap = { { 0, playFunction },{1, exitFunction} };

    // MAIN WINDOW
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Fishing Minigame");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int y = clickCheck(mousePos);
                    std::cout << y;
                    if (y >= 0)
                    {
                        functionMap[y]();
                    }
                }
            }
        }

        window.clear();
        for (const auto& lastSprite : allSprites) {
            window.draw(lastSprite); // Draw remaining sprites
        }
        window.display();

    }


    return 0;
}