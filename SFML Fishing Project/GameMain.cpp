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
#include <chrono>
#include <thread>
#include <string>
#include <utility>
#include <map>

// CONSTANTS & GLOBALS
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

// CLICKLABLE POSITIONS
sf::IntRect playbuttonUI({ 380, 480 }, { 430, 130 });
sf::IntRect exitbuttonUI({ 15, 15 }, { 100, 60 });
sf::IntRect fishbuttonUI({ 20, 600 }, {180, 180});
sf::IntRect bookbuttonUI({ 950, 600 }, { 200, 130 });
sf::IntRect bookxbuttonUI({ 850, 680 }, { 110, 105 });

// POSTION LISTS
std::vector<sf::IntRect> xyMasterList = { playbuttonUI, exitbuttonUI, fishbuttonUI, bookbuttonUI, bookxbuttonUI };
std::vector<sf::IntRect> xyAvailableList = { /*empty because the ui elements get added or removed based on gamestate, established at the beginning of the gamestate*/ };

// PNGS -> TEXTURES
sf::Texture titleTexture("sprites/TitleScreen.png");
sf::Texture backgroundTexture("sprites/BackgroundImage.png");
sf::Texture playerIdleTexture("sprites/PlayerIdle.png");
sf::Texture playerCastingTexture("sprites/PlayerCasting.png");
sf::Texture playerFishingTexture("sprites/PlayerFishing.png");
sf::Texture fishingButtonTexture("sprites/FishingButton.png");
sf::Texture fishingButtonBGTexture("sprites/FishingButtonBG.png");
sf::Texture notificationTexture("sprites/NotificationBox.png");
sf::Texture bookMenuTexture("sprites/Book menu.png");
sf::Texture bookXButtonTexture("sprites/xButton.png");
sf::Texture fishingGameTexture("sprites/CatchGameScreen.png");
sf::Texture fishingGamePromptTexture("sprites/CatchGamePrompt.png");
sf::Texture shopTexture("sprites/Shop.png");
sf::Texture shopPurchasedTexture("sprites/ShopBoughtItem.png");

// TEXTURES -> SPRITES
sf::Sprite titleSprite(titleTexture);
sf::Sprite backgroundSprite(backgroundTexture);
sf::Sprite playerIdleSprite(playerIdleTexture);
sf::Sprite playerCastingSprite(playerCastingTexture);
sf::Sprite playerFishingSprite(playerFishingTexture);
sf::Sprite fishingButtonSprite(fishingButtonTexture);
sf::Sprite fishingButtonBGSprite(fishingButtonBGTexture);
sf::Sprite notificationSprite(notificationTexture);
sf::Sprite bookMenuSprite(bookMenuTexture);
sf::Sprite bookXButtonSprite(bookXButtonTexture);
sf::Sprite fishingGameSprite(fishingGameTexture);
sf::Sprite fishingGamePromptSprite(fishingGamePromptTexture);
sf::Sprite shopSprite(shopTexture);
sf::Sprite shopPurchasedSprite(shopPurchasedTexture);

// SPRITE LIST (for ui management)
std::vector<sf::Sprite> allSprites = { backgroundSprite, titleSprite, playerIdleSprite, playerCastingSprite, playerFishingSprite, fishingButtonSprite, notificationSprite, bookMenuSprite, fishingGameSprite, fishingGamePromptSprite, shopSprite, shopPurchasedSprite };

//GAMESTATE 
enum class GameState { Title, MainMenu, Fishing, Book, Exit };
GameState currentState = GameState::Title;

/* FUNCTIONS */

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

void exitFunction()
{
    exit(1);
}

void showTitleScreen(sf::RenderWindow& window) 
{
    window.clear();
    window.draw(backgroundSprite);
    window.draw(titleSprite);
    window.display();

    while (window.isOpen()) 
    {
        sf::Vector2i mousePosVec = sf::Mouse::getPosition(window);
        int mousePos = clickCheck(mousePosVec);

        while (const auto event = window.pollEvent()) 
        {
            if (const auto MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                std::cout << (mousePos);
                if (mousePos == 1) //exit button
                {
                    currentState = GameState::Exit;
                    window.close();
                }
                else if (mousePos == 0) //play button
                {
                    std::cout << ("\nplay button pressed\n");
                    currentState = GameState::MainMenu;
                    //need to add piece of code that then removes the function assigned to 0 from the available list
                }
                return;
            }
        }
    }
}

//Main menu that pops up showing the shop ui on the same, and the fish button, need to 
void showMainMenu(sf::RenderWindow& window)
{
    playerIdleSprite.setPosition({ 210, 60 });
    fishingButtonSprite.setPosition({ 20, 600 });
    fishingButtonBGSprite.setPosition({ 20, 600 });

    window.clear();
    window.draw(backgroundSprite);
    window.draw(playerIdleSprite);
    window.draw(shopSprite);
    window.draw(fishingButtonBGSprite);
    window.draw(fishingButtonSprite);
    window.display();

    while (window.isOpen())
    {
        sf::Vector2i mousePosVec = sf::Mouse::getPosition(window);
        int mousePos = clickCheck(mousePosVec);

        while (const auto event = window.pollEvent())
        {
            if (const auto MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                std::cout << (mousePos); //exit button
                if (mousePos == 1)
                {
                    currentState = GameState::Exit;
                    window.close();
                }
                else if (mousePos == 2) //fishing button pressed
                {
                    std::cout << ("\nfishing button pressed\n");
                    playerCastingSprite.setPosition({ 210, 60 });
                    window.clear();
                    window.draw(backgroundSprite);
                    window.draw(playerCastingSprite);
                    window.display();

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    playerFishingSprite.setPosition({ 210, 60 });
                    window.clear();
                    window.draw(backgroundSprite);
                    window.draw(playerFishingSprite);
                    window.display();

                    currentState = GameState::Fishing;
                }
                else if (mousePos == 3) //book button pressed
                {
                    std::cout << ("\nbook button pressed\n");
                    currentState = GameState::Book;
                    //the book
                }
                return;
            }
        }
    }
}

void showBookGallery(sf::RenderWindow& window)
{
    playerIdleSprite.setPosition({ 210, 60 });
    fishingButtonSprite.setPosition({ 20, 600 });
    fishingButtonBGSprite.setPosition({ 20, 600 });
    bookXButtonSprite.setPosition({ 850, 680 });


    window.clear();
    window.draw(backgroundSprite);
    window.draw(playerIdleSprite);
    window.draw(bookMenuSprite);
    window.draw(bookXButtonSprite);
    window.display();

    while (window.isOpen())
    {
        sf::Vector2i mousePosVec = sf::Mouse::getPosition(window);
        int mousePos = clickCheck(mousePosVec);

        while (const auto event = window.pollEvent())
        {
            if (const auto MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                std::cout << (mousePos); //exit button
                if (mousePos == 1)
                {
                    currentState = GameState::Exit;
                    window.close();
                }
                else if (mousePos == 4) //book x button pressed, since this is book gamestate, this closes the book and reopens the regular main menu gamestate
                {
                    std::cout << ("\nbook button pressed in book menu\n");
                    currentState = GameState::MainMenu;
                    //the book
                }
                return;
            }
        }
    }
}

//MAIN GAME
int main() 
{
    // MAIN WINDOW
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Fishing Minigame");

    while (window.isOpen() && currentState != GameState::Exit) {
        switch (currentState) 
        {
            case GameState::Title:
                xyAvailableList.clear();
                xyAvailableList.push_back(playbuttonUI); xyAvailableList.push_back(exitbuttonUI);
                showTitleScreen(window);
                break;
            case GameState::MainMenu:
                xyAvailableList.clear();
                xyAvailableList.push_back(exitbuttonUI); xyAvailableList.push_back(fishbuttonUI); xyAvailableList.push_back(bookbuttonUI);
                showMainMenu(window);
                break;
            case GameState::Book:
                xyAvailableList.clear();
                xyAvailableList.push_back(exitbuttonUI); xyAvailableList.push_back(bookxbuttonUI);
                showBookGallery(window);
                break;
            default:
                break;
        }
    }
    return 0;
}