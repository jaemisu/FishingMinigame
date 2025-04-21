/*********************************************
SFML Fishing Minigame Window
cc: Sets up the game window, background, and click detection.
*********************************************/

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <map>

// SAVE FUNCTION
void saveData(int unlocks, int money)
{
    std::ofstream file("savefile.txt");
    if (file.is_open()) {
        file << unlocks << "\n";
        file << money << "\n";
        file.close();
        std::cout << "Game saved!\n";
    }
}

void loadData(int& unlocks, int& money)
{
    std::ifstream file("savefile.txt");
    if (file.is_open()) {
        file >> unlocks;
        file >> money;
        file.close();
        std::cout << "Game loaded!\n";
    }

    else
    {
        std::cerr << "No save file found. Starting new game.\n";
        unlocks = 0;
        money = 0;
    }
}



// CONSTANTS & GLOBALS
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
int successfulHits = 0;
int userMoney;
int userUnlocks;


// CREATING TEXT


// CLICKLABLE POSITIONS
sf::IntRect playbuttonUI({ 380, 480 }, { 430, 130 });
sf::IntRect exitbuttonUI({ 15, 15 }, { 100, 60 });
sf::IntRect fishbuttonUI({ 20, 600 }, {180, 180});
sf::IntRect bookbuttonUI({ 950, 600 }, { 200, 130 });
sf::IntRect bookxbuttonUI({ 850, 680 }, { 110, 105 });
sf::IntRect unlock1UI({ 930, 300 }, { 100, 100});
sf::IntRect unlock2UI({ 1040, 450}, { 130, 130});


// POSTION LISTS
std::vector<sf::IntRect> xyMasterList = { playbuttonUI, exitbuttonUI, fishbuttonUI, bookbuttonUI, bookxbuttonUI, unlock1UI, unlock2UI };
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
sf::Texture shop1UnlockTexture("sprites/1UnlockShop.png");
sf::Texture shop2UnlockTexture("sprites/2UnlockShop.png");

sf::Texture fish1NTexture("fishsprites/1NSalmon.png");
sf::Texture fish1ATexture("fishsprites/1AAnomalocaris.png");
sf::Texture fish1PTexture("fishsprites/1PMagikarp.png");
sf::Texture fish2NTexture("fishsprites/2NOctopus.png");
sf::Texture fish2ATexture("fishsprites/2ADunkleostus.png");
sf::Texture fish2PTexture("fishsprites/2PLapras.png");
sf::Texture fish3NTexture("fishsprites/3NWhale.png");
sf::Texture fish3ATexture("fishsprites/3AMegalodon.png");
sf::Texture fish3PTexture("fishsprites/3PKyogre.png");


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
sf::Sprite shop1UnlockSprite(shop1UnlockTexture);
sf::Sprite shop2UnlockSprite(shop2UnlockTexture);

sf::Sprite fish1NSprite(fish1NTexture);
sf::Sprite fish1ASprite(fish1ATexture);
sf::Sprite fish1PSprite(fish1PTexture);
sf::Sprite fish2NSprite(fish2NTexture);
sf::Sprite fish2ASprite(fish2ATexture);
sf::Sprite fish2PSprite(fish2PTexture);
sf::Sprite fish3NSprite(fish3NTexture);
sf::Sprite fish3ASprite(fish3ATexture);
sf::Sprite fish3PSprite(fish3PTexture);

//Fish list
std::vector<sf::Sprite> fishSpriteList{ fish1NSprite , fish2NSprite , fish3NSprite , fish1ASprite , fish2ASprite , fish3ASprite , fish1PSprite, fish2PSprite , fish3PSprite };


//GAMESTATE 
enum class GameState { Title, MainMenu, Fishing, Book, Exit };
GameState currentState = GameState::Title;

/* FUNCTIONS */

//click checking function
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

//exit function
void exitFunction()
{
    saveData(userUnlocks, userMoney);
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
                }
                return;
            }
        }
    }
}

//main menu gamestate
void showMainMenu(sf::RenderWindow& window)
{
    playerIdleSprite.setPosition({ 210, 60 });
    fishingButtonSprite.setPosition({ 20, 600 });
    fishingButtonBGSprite.setPosition({ 20, 600 });

    sf::Font font("Short Baby.ttf");
    sf::Text userMoneyText(font);
    userMoneyText.setString(std::to_string(userMoney));
    userMoneyText.setCharacterSize(24);
    userMoneyText.setFillColor(sf::Color::Black);
    userMoneyText.setPosition({1025, 138});

    window.clear();
    window.draw(backgroundSprite);
    window.draw(playerIdleSprite);
    window.draw(fishingButtonBGSprite);
    window.draw(fishingButtonSprite);
    if (userUnlocks == 0)
    {
        window.draw(shopSprite);
    }
    else if (userUnlocks == 1)
    {
        window.draw(shop1UnlockSprite);
    }
    else if (userUnlocks == 2)
    {
        window.draw(shop2UnlockSprite);
    }
    window.draw(userMoneyText);
    window.display();

    while (window.isOpen())
    {
        sf::Vector2i mousePosVec = sf::Mouse::getPosition(window);
        int mousePos = clickCheck(mousePosVec);

        while (const auto event = window.pollEvent())
        {
            if (const auto MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                std::cout << (mousePos); // exit button
                if (mousePos == 1)
                {
                    currentState = GameState::Exit;
                    window.close();
                }
                else if (mousePos == 2) // fishing button pressed
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
                else if (mousePos == 3) // book button pressed
                {
                    std::cout << ("\nbook button pressed\n");
                    currentState = GameState::Book;
                    // the book
                }
                else if (mousePos == 5)
                {
                    if (userUnlocks == 0 && userMoney >= 6)
                    {
                        window.clear();
                        window.draw(backgroundSprite);
                        window.draw(playerIdleSprite);
                        window.draw(fishingButtonBGSprite);
                        window.draw(fishingButtonSprite);
                        window.draw(shop1UnlockSprite);
                        userMoney -= 6;
                        userUnlocks = 1;
                        window.draw(userMoneyText);
                        window.display();
                        saveData(userUnlocks, userMoney);
                    }
                }
                else if (mousePos == 6)
                {
                    if (userUnlocks == 1 && userMoney >= 9)
                    {
                        window.clear();
                        window.draw(backgroundSprite);
                        window.draw(playerIdleSprite);
                        window.draw(fishingButtonBGSprite);
                        window.draw(fishingButtonSprite);
                        window.draw(shop2UnlockSprite);
                        userMoney -= 9;
                        userUnlocks = 2;
                        window.draw(userMoneyText);
                        window.display();
                        saveData(userUnlocks, userMoney);
                    }
                }
                return;
            }
        }
    }
}

//Book Gamestate
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
                std::cout << (mousePos); // exit button
                if (mousePos == 1)
                {
                    currentState = GameState::Exit;
                    window.close();
                }
                else if (mousePos == 4) // x button pressed, closes the book
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


// minigame gamestate
void runFishingMinigame(int difficulty, sf::RenderWindow& window)
{
    int maxPrompts = 5 + difficulty * 2;
    const int timePerPromptMs = 1000 - difficulty * 50;

    // 9 fixed locations for prompts
    std::vector<sf::Vector2i> promptLocations =
    {
        {430, 300}, {555, 320}, {655, 290},
        {470, 365}, {600, 375}, {500, 300},
        {385, 420}, {590, 435}, {725, 420}
    };

    // minigame code

    for (int i = 0; i < maxPrompts;) 
    {
        sf::Vector2i promptPos = promptLocations[rand() % promptLocations.size()];
        fishingGamePromptSprite.setPosition(static_cast<sf::Vector2f>(promptPos));

        window.clear();
        window.draw(backgroundSprite);
        window.draw(playerFishingSprite);
        window.draw(fishingGameSprite);
        window.draw(fishingGamePromptSprite);
        window.display();

        std::random_device rd; // generating random fish choice seed
        std::mt19937 gen(rd()); 

        if (successfulHits == maxPrompts) // checking if the user has won yet
        {
            window.clear();
            window.draw(backgroundSprite);
            window.draw(playerFishingSprite);
            window.draw(notificationSprite);

            if (userUnlocks > 2) //slight save-scum prevention 
            {
                userUnlocks = 2;
            }

            if (userUnlocks == 0)
            {
                std::uniform_int_distribution<> dist(0, 2);
                int U1 = dist(gen);
                fishSpriteList[U1].setPosition({ 600, 100 });
                window.draw(fishSpriteList[U1]);
            }

            if (userUnlocks == 1)
            {
                std::uniform_int_distribution<> dist(0, 5);
                int U2 = dist(gen);
                fishSpriteList[U2].setPosition({ 600, 100 });
                window.draw(fishSpriteList[U2]);
            }

            if (userUnlocks == 2)
            {
                std::uniform_int_distribution<> dist(0, 8);
                int U3 = dist(gen);
                fishSpriteList[U3].setPosition({ 600, 100 });
                window.draw(fishSpriteList[U3]);
            }

            window.display();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            currentState = GameState::MainMenu;
            successfulHits -= successfulHits;
            userMoney += 3;
            return;
        }


        sf::Clock reactionTimer;

        while (window.isOpen())
        {
            sf::Vector2i mousePosVec = sf::Mouse::getPosition(window);
            int mousePos = clickCheck(mousePosVec);

            while (const auto event = window.pollEvent())
            {
                if (const auto MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    sf::FloatRect promptBounds = fishingGamePromptSprite.getGlobalBounds();

                    if (mousePos == 1)
                    {
                        currentState = GameState::Exit;
                        window.close();
                    }

                    if (promptBounds.contains(static_cast<sf::Vector2f>(mousePosVec)))
                    {
                        successfulHits++;
                        std::cout << (successfulHits);
                        return;
                    }
                }
            }

            if (reactionTimer.getElapsedTime().asMilliseconds() > 500)
            {
                std::cout << "fail" << std::endl;
                return;
            }
        }
    }
}

// MAIN
int main() 
{
    // MAIN WINDOW
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Fishing Minigame");
    loadData(userUnlocks, userMoney);

    while (window.isOpen() && currentState != GameState::Exit) {
        switch (currentState) 
        {
            case GameState::Title:
                xyAvailableList.clear();
                xyAvailableList.push_back(exitbuttonUI); xyAvailableList.push_back(playbuttonUI);
                showTitleScreen(window);
                break;
            case GameState::MainMenu:
                xyAvailableList.clear();
                xyAvailableList.push_back(exitbuttonUI); xyAvailableList.push_back(fishbuttonUI); xyAvailableList.push_back(bookbuttonUI); xyAvailableList.push_back(unlock1UI); xyAvailableList.push_back(unlock2UI);
                std::cout << ("user money:", userMoney);
                showMainMenu(window);
                break;
            case GameState::Book:
                xyAvailableList.clear();
                xyAvailableList.push_back(exitbuttonUI); xyAvailableList.push_back(bookxbuttonUI);
                showBookGallery(window);
                break;
            case GameState::Fishing:
                xyAvailableList.clear();
                xyAvailableList.push_back(exitbuttonUI);
                runFishingMinigame(1, window);
        }
    }
    return 0;
}