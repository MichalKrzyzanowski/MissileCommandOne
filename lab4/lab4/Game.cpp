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
	setupGameOverText(); // load font 
	setupSprite(); // load texture
	
	setupScene();
	setupText();
	m_laserStartPoint = sf::Vector2f{ m_base.getPosition().x + (m_base.getSize().x / 2.0f), m_ground.getPosition().y };

	m_explosion.setOutlineThickness(2.0f);
	m_explosion.setOutlineColor(sf::Color(191u, 73u, 0u));
	m_explosion.setFillColor(sf::Color(0u, 0u, 0u, 0u));


	m_asteroidInterval = rand() % 100 + 1.0f;
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

	if (m_currentGameState == gameRunning)
	{
		if (m_currentLaserState == standby)
		{
			m_laser.clear();
			m_scoreAwarded = false;
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

		if (m_currentAsteroidState == launch)
		{
			asteroidProperties();
			m_currentAsteroidState = flight;
		}

		if (m_currentAsteroidState == flight)
		{
			animateAsteroid();
		}

		if (m_currentAsteroidState == collision)
		{
			m_asteroid.clear();
			m_asteroidIntervalCounter++;
			if (m_asteroidIntervalCounter > m_asteroidInterval)
			{
				m_currentAsteroidState = launch;
				m_asteroidInterval = rand() % 100 + 1.0f;
				m_asteroidIntervalCounter = 0.0f;
			}
		}

		m_expBar.setSize(sf::Vector2f{ m_xp, 20.0f });
		m_scoreText.setString("Score: " + std::to_string(m_score) + "pts");
		m_playerLvlText.setString("Level: " + std::to_string(m_playerLvl));
		m_scoreMultiplier.setString("Multiplier: x" + std::to_string(m_playerLvl));
		animatePowerBar();
	}
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
		m_currentLaserState = explosion;
	}

	else if (m_laserEndPoint.y <= m_altitude)
	{
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

	m_explosionCollisionDistance = (m_laserEndPoint.x - m_asteroidEndPoint.x) * (m_laserEndPoint.x - m_asteroidEndPoint.x)
		+ (m_laserEndPoint.y - m_asteroidEndPoint.y) * (m_laserEndPoint.y - m_asteroidEndPoint.y);
	m_explosionCollisionDistance = sqrt(m_explosionCollisionDistance);

	collisionDetection();

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

void Game::asteroidProperties()
{	
	float randomStartPoint = rand() % m_window.getSize().x + 1.0f;
	float randomEndPoint = rand() % m_window.getSize().x + 1.0f;

	m_asteroidStartPoint = sf::Vector2f{ randomStartPoint, 0.0f };
	m_asteroidEndPoint = sf::Vector2f{ randomEndPoint, 600.0f };

	m_asteroidDirectionNormalised = m_asteroidEndPoint - m_asteroidStartPoint;
	m_asteroidDirectionNormalised = vectorUnitVector(m_asteroidDirectionNormalised);
	m_asteroidVelocity = m_asteroidDirectionNormalised * m_asteroidSpeed;
	m_asteroidEndPoint = m_asteroidStartPoint + m_asteroidVelocity;
}

void Game::animateAsteroid()
{
	sf::Vertex asteroidStart{ m_asteroidStartPoint };
	sf::Vertex asteroidEnd{ m_asteroidEndPoint };

	collisionDetection();

	m_asteroidEndPoint += m_asteroidVelocity;

	m_asteroid.append(asteroidStart);
	m_asteroid.append(asteroidEnd);
}

void Game::collisionDetection()
{
	if (m_asteroidEndPoint.y > m_ground.getPosition().y)
	{
		std::cout << "Collision Detected!";
		m_currentGameState = gameOver;
	}

	if (m_explosionCollisionDistance < m_explosionRadius)
	{
		std::cout << "explosion collision detected";

		if (!m_scoreAwarded)
		{
			m_score += 1 * m_playerLvl;
			m_xp += m_playerXpGain;
			m_scoreAwarded = true;
		}

		if (m_xp >= MAX_XP)
		{
			levelUp();
		}

		m_currentAsteroidState = collision;
	}
}

void Game::levelUp()
{
	m_playerLvl++;
	m_playerXpGain /= 1.1f;
	m_xp = 0.0f;
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear();

	if (m_currentGameState == !gameOver)
	{
		m_window.draw(m_ground);
		m_window.draw(m_base);
		m_window.draw(m_laser);
		m_window.draw(m_asteroid);
		m_window.draw(m_powerBarBackground);
		m_window.draw(m_powerBar);
		m_window.draw(m_expBarBackground);
		m_window.draw(m_expBar);
		m_window.draw(m_scoreText);
		m_window.draw(m_scoreMultiplier);
		m_window.draw(m_playerLvlText);

		if (m_currentLaserState == explosion)
		{
			m_window.draw(m_explosion);
		}

		m_window.display();
	}

	else
	{
		m_window.clear();

		m_window.draw(m_gameOverText);

		m_window.display();
	}
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupGameOverText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_gameOverText.setFont(m_ArialBlackfont);
	m_gameOverText.setString("GAME OVER!");
	m_gameOverText.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_gameOverText.setPosition(100.0f, 250.0f);
	m_gameOverText.setCharacterSize(80);
	m_gameOverText.setOutlineColor(sf::Color::Red);
	m_gameOverText.setFillColor(sf::Color::Black);
	m_gameOverText.setOutlineThickness(3.0f);

}

// sets up general text for use in HUD elements
void Game::setupTextProperties(sf::Text & t_text, sf::Vector2f t_position, std::string t_string, int t_characterSize)
{
	t_text.setFont(m_ArialBlackfont);
	t_text.setString(t_string);
	t_text.setFillColor(sf::Color::White);
	t_text.setPosition(t_position);
	t_text.setCharacterSize(t_characterSize);
}

void Game::setupText()
{
	setupTextProperties(m_scoreText, sf::Vector2f{ 10.0f, 555.0f }, "Score: " + std::to_string(m_score) + "pts", 18);
	setupTextProperties(m_scoreMultiplier, sf::Vector2f{ 10.0f, 575.0f }, "Multiplier: x" + std::to_string(m_playerLvl), 18);
	setupTextProperties(m_playerLvlText, sf::Vector2f{ 648.0f, 518.0f }, "Level: " + std::to_string(m_playerLvl), 14);
	m_playerLvlText.setFillColor(sf::Color(232, 202, 9));
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

void Game::setupSceneProperties(sf::RectangleShape & t_rectangle, sf::Vector2f t_position, sf::Vector2f t_size)
{
	t_rectangle.setPosition(t_position);
	t_rectangle.setSize(t_size);
}

void Game::setupScene()
{
	setupSceneProperties(m_ground, sf::Vector2f{ 0.0f, 500.0f }, sf::Vector2f{ 800.0f, 100.0f }); // set up ground rectangle
	m_ground.setFillColor(sf::Color(2, 99, 20)); // dark green color
	setupSceneProperties(m_base, sf::Vector2f{ 360.0f, 440.0f }, sf::Vector2f{ 80.0f, 60.0f }); // set up ground rectangle
	m_base.setFillColor(sf::Color(219, 199, 52)); // golden color
	setupSceneProperties(m_powerBar, sf::Vector2f{ 10.0f, 520.0f }, sf::Vector2f{ m_currentPower, 30.0f }); // set up power bar
	m_powerBar.setFillColor(sf::Color(188, 5, 5)); // red color
	setupSceneProperties(m_powerBarBackground, sf::Vector2f{ 8.0f, 518.0f }, sf::Vector2f{ 454.0f, 34.0f });
	m_powerBarBackground.setFillColor(sf::Color::Black);

	setupSceneProperties(m_expBar, sf::Vector2f{ 650.0f, 540.0f }, sf::Vector2f{ m_xp, 20.0f });
	m_expBar.setFillColor(sf::Color(232, 202, 9));
	setupSceneProperties(m_expBarBackground, sf::Vector2f{ 648.0f, 538.0f }, sf::Vector2f{ 104.0f, 24.0f });
	m_expBarBackground.setFillColor(sf::Color::Black);
}
