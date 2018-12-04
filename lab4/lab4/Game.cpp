// author Peter Lowe

#include "Game.h"
#include <iostream>
#include "VectorFormulas.h"


/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800u, 600u, 32u }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setUpScene(m_ground, sf::Vector2f{ 0.0f, 500.0f }, sf::Vector2f{ 800.0f, 100.0f }); // set up ground rectangle
	m_ground.setFillColor(sf::Color(2, 99, 20)); // dark green color
	setUpScene(m_base, sf::Vector2f{ 360.0f, 440.0f }, sf::Vector2f{ 80.0f, 60.0f }); // set up ground rectangle
	m_base.setFillColor(sf::Color(219, 199, 52)); // golden color
	setUpScene(m_powerBar, sf::Vector2f{ 10.0f, 530.0f }, sf::Vector2f{ m_currentPower, 30.0f }); // set up power bar
	m_powerBar.setFillColor(sf::Color(188, 5, 5)); // red color

	m_laserStartPoint = sf::Vector2f{ m_base.getPosition().x + (m_base.getSize().x / 2.0f), m_ground.getPosition().y };

	m_explosion.setFillColor(sf::Color(191u, 73u, 0u));
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
	sf::Time timePerFrame = sf::seconds(1.f / 60.0f); // 60 fps
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

		if (m_currentLaserState == standby)
		{
			if (sf::Event::MouseButtonPressed == nextEvent.type)
			{
				processMouseEvents(nextEvent);
				m_currentLaserState = firing;
			}
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

	if (m_currentLaserState == standby)
	{
		m_laser.clear();
	}

	if (m_currentLaserState == firing)
	{
		animateLaser();
	}

	if (m_currentLaserState == explosion)
	{
		m_laser.clear();
		m_currentPower = 0.0f;
		animateExplosion();
	}

	animatePowerBar();

	AsteroidProperties();
}

void Game::processMouseEvents(sf::Event t_mouseEvents)
{
	sf::Vector2i mouseClick{};

	if (sf::Mouse::Left == t_mouseEvents.mouseButton.button)
	{
		mouseClick = sf::Mouse::getPosition(m_window);
		m_laserDestination = sf::Vector2f(static_cast<float>(mouseClick.x), static_cast<float>(mouseClick.y));
		m_directionNormalised = m_laserDestination - m_laserStartPoint;
		m_directionNormalised = vectorUnitVector(m_directionNormalised);
		m_laserVelocity = m_directionNormalised * m_laserSpeed;
		m_laserEndPoint = m_laserStartPoint + m_laserVelocity;
	}
}

void Game::animateLaser()
{
	sf::Vertex lineStart{ m_laserStartPoint };
	sf::Vertex lineEnd{};
	
	if (m_laserEndPoint.y <= m_laserDestination.y)
	{
		std::cout << "Works!";
		m_currentLaserState = explosion;
	}

	else if (m_laserEndPoint.y <= m_altitude)
	{
		std::cout << "Works!";
		m_currentLaserState = explosion;
	}

	else
	{
		m_laserEndPoint += m_laserVelocity;

		lineEnd.position = m_laserEndPoint;

		m_laser.append(lineStart);
		m_laser.append(lineEnd);
	}
}

void Game::animateExplosion()
{
	m_explosion.setPosition(m_laserEndPoint);
	m_explosion.setOrigin(m_explosionRadius, m_explosionRadius);
	m_explosion.setRadius(m_explosionRadius);
	m_explosionRadius++;

	if (m_explosionRadius >= 30.0f)
	{
		m_explosionRadius = 0.0f;
		m_currentLaserState = standby;
	}
}

void Game::animatePowerBar()
{
	m_powerBar.setSize(sf::Vector2f{ m_currentPower, 30.0f });

	if (m_currentLaserState == standby)
	{
		if (m_currentPower >= MAX_POWER)
		{
			m_currentPower = MAX_POWER;
		}

		else
		{
			m_currentPower += m_powerInc;
		}
	}

	else if (m_currentLaserState == firing)
	{
		m_altitude = (m_window.getSize().y - m_ground.getSize().y) - m_currentPower;
	}
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

	if (m_currentLaserState == explosion)
	{
		m_window.draw(m_explosion);
	}

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
