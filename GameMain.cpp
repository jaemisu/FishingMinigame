#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>
#include <filesystem>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Best Fishing Game Ever"); //Opens the screen

    //Texture Loading
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("BackgroundImage.png");

    sf::Texture bookMenu;
    bookMenu.loadFromFile("Book menu.png");

    sf::Texture titleScreen;
    titleScreen.loadFromFile("TitleScreen.png");

    sf::Texture playerIdle;
    playerIdle.loadFromFile("PlayerIdle.png");

    sf::Texture shopTexture;
    shopTexture.loadFromFile("Shop.png");

    sf::Texture fishButtonTexture;
    fishButtonTexture.loadFromFile("fishingbutton.png");

    //Sprite Creation
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Sprite titleSprite;
    titleSprite.setTexture(titleScreen);

    sf::Sprite playerIdleSprite;
    playerIdleSprite.setTexture(playerIdle);

    sf::Sprite shopSprite;
    shopSprite.setTexture(shopTexture);

    sf::Sprite fishingButton;
    fishingButton.setTexture(fishButtonTexture);

    window.clear();
    window.draw(backgroundSprite);
    window.draw(titleSprite);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {

            if (event.type == sf::Event::Closed)
                window.close();
                
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (clickChecking(mousePos, clickAreaMenuExit))
                    {
                        exit(0);
                    }
                    if (clickChecking(mousePos, clickAreaMenuPlay))
                    {
                        window.clear();
                        window.draw(backgroundSprite);
                        window.draw(fishingButton);
                        window.draw(shopSprite);
                        window.draw(playerIdleSprite);
                        continue;
                    }
                }
            }
        }       
        window.display();
    }
    return 0;
}