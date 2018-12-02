// author Peter Lowe

#include "Game.h"
#include <iostream>


/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800, 600, 32 }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setUpScene(m_ground, sf::Vector2f{ 0, 500 }, sf::Vector2f{ 800, 100 }); // set up ground rectangle
	m_ground.setFillColor(sf::Color(2, 99, 20)); // dark green color
	setUpScene(m_base, sf::Vector2f{ 360, 440 }, sf::Vector2f{ 80, 60 }); // set up ground rectangle
	m_base.setFillColor(sf::Color(219, 199, 52)); // golden color
	setUpScene(m_powerBar, sf::Vector2f{ 10, 530 }, sf::Vector2f{ 200, 30 }); // set up power bar
	m_powerBar.setFillColor(sf::Color(188, 5, 5)); // red color
}

/// <summary>
/// default destructor for game, nothing to release
/// </summary>
Game::~Game()
{
}


/// <summary>
/// game loop running at 60fps
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event nextEvent;
	while (m_window.pollEvent(nextEvent))
	{
		if (sf::Event::Closed == nextEvent.type) // window message
		{
			m_window.close();
		}

		if (sf::Event::KeyPressed == nextEvent.type) //user key press
		{
			if (sf::Keyboard::Escape == nextEvent.key.code)
			{
				m_exitGame = true;
			}
		}

		if (sf::Event::MouseButtonPressed == nextEvent.type)
		{
			processMouseEvents(nextEvent);
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	AsteroidProperties();
}

void Game::processMouseEvents(sf::Event t_mouseEvents)
{
	sf::Vertex lineStart{ sf::Vector2f{ m_base.getPosition().x + (m_base.getSize().x / 2), m_base.getPosition().y} };
	sf::Vertex lineEnd{};
	sf::Vector2i mouseClick{};

	m_laser.clear();

	if (sf::Mouse::Left == t_mouseEvents.mouseButton.button)
	{
		mouseClick = sf::Mouse::getPosition(m_window);
		m_laserDestination = sf::Vector2f(static_cast<float>(mouseClick.x), static_cast<float>(mouseClick.y));
		lineEnd.position = m_laserDestination;
	}

	else
	{
		m_laser.clear();
	}

	m_laser.append(lineStart);
	m_laser.append(lineEnd);
}

void Game::AsteroidProperties()
{
	sf::Vertex asteroidStart{};
	sf::Vertex asteroidEnd{};
	sf::Vector2f startPoint{200, 200};
	sf::Vector2f endPoint{500, 500};

	asteroidStart.position = startPoint;
	asteroidEnd.position = endPoint;

	m_asteroid.append(asteroidStart);
	m_asteroid.append(asteroidEnd);
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear();
	//m_window.draw(m_welcomeMessage);
	//m_window.draw(m_logoSprite);
	m_window.draw(m_ground);
	m_window.draw(m_base);
	m_window.draw(m_laser);
	m_window.draw(m_asteroid);
	m_window.draw(m_powerBar);

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}

void Game::setUpScene(sf::RectangleShape & t_rectangle, sf::Vector2f t_position, sf::Vector2f t_size)
{
	t_rectangle.setPosition(t_position);
	t_rectangle.setSize(t_size);
}
