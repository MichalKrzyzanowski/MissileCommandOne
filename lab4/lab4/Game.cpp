/// Author: Michal K.
/// Date: 22/11/18
/// Login: C00240696
/// +----------------------+---------------------------------------------------------------
///	| Missile Command: One |
/// +----------------------+---------------------------------------------------------------
/// Simple based on simple vector formulas and calculations
/// Player is tasked with shooting down asteroids coming down from the top of the screen
/// Player is firing a laser
/// Player gets score if asteroid collides with laser explosion
/// Player loses if laser hits the ground once
/// Two game Modes: classic and custom
/// ---------------------------------------------------------------------------------------
/// Estimated Time: 24 hrs
/// Actual Time: 17 hrs
/// Known Bugs:
/// N/A

#include "Game.h"
#include <iostream>
#include "VectorFormulas.h"


/// default constructor
/// pass parameters for sfml window, setup m_exitGame
Game::Game() :
	m_window{ sf::VideoMode{ 800u, 600u, 32u }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupGameOverText(); // set up game over title text in game over screen
	setupTitleText(); // set up game title text in main menu
	
	setupScene(); // gives rectangle shapes properties based on setupSceneProperties function
	setupText(); // gives text variables properties based on setupTextProperties function

	// setup start position of laser
	m_laserStartPoint = sf::Vector2f{ m_base.getPosition().x + (m_base.getSize().x / 2.0f), m_ground.getPosition().y };

	// set outline thickness and change it's color, rest of the color is transparent
	m_explosion.setOutlineThickness(2.0f);
	m_explosion.setOutlineColor(sf::Color(191u, 73u, 0u));
	m_explosion.setFillColor(sf::Color(0u, 0u, 0u, 0u));


	m_asteroidInterval = rand() % 100 + 1.0f; // interval between asteroid's respawn set to random number
}


/// default destructor for game, nothing to release
Game::~Game()
{
}


/// game loop running at 60fps
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


/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
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

		// only do in classic or custom mode
		if (m_currentGameState == classicMode || m_currentGameState == customMode)
		{
			if (m_currentLaserState == standby) // once laser is waiting for input
			{
				if (sf::Event::MouseButtonPressed == nextEvent.type)
				{
					processMouseEvents(nextEvent);
					m_currentLaserState = firing; // laser has been fired
				}
			}
		}

		if (m_currentGameState == mainMenu) // if main menu is the current game screen
		{
			if (sf::Event::KeyPressed == nextEvent.type)
			{
				// checks if either number 1 is pressed (num1 or numpad1)
				if (sf::Keyboard::Num1 == nextEvent.key.code || sf::Keyboard::Numpad1 == nextEvent.key.code)
				{
					m_currentGameState = classicMode; // game mode is set to classic mode
				}

				// checks if either number 2 is pressed (num2 or numpad2)
				if (sf::Keyboard::Num2 == nextEvent.key.code || sf::Keyboard::Numpad2 == nextEvent.key.code)
				{
					m_currentGameState = customMode; // game mode is set to classic mode
				}
			}
		}

		if (m_currentGameState == gameOver) // if game is over
		{
			if (sf::Event::KeyPressed == nextEvent.type)
			{
				if (sf::Keyboard::Space == nextEvent.key.code)
				{
					m_currentGameState = mainMenu; // return to main menu screen
				}
			}
		}
	}
}


/// Update the game world
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame) // close window if true
	{
		m_window.close();
	}

	if (m_currentGameState == mainMenu) // if main menu is current game screen
	{
		resetAttributes(); // reset player stats such as xp, score, laser speed, etc. to default values
		m_currentLaserState = standby; // laser is waiting for input
	}

	if (m_currentGameState == classicMode) // if classic mode is currently played
	{
		m_laserSpeed = 2.0f; // set laser speed to 2.0f
	}

	// if classic mode or custom mode is currently played
	if (m_currentGameState == classicMode || m_currentGameState == customMode)
	{
		if (m_currentLaserState == standby) // when laser is waiting for input
		{
			m_laser.clear(); // clear laser vertex array
			m_scoreAwarded = false; // score is not awarded yet
		}

		if (m_currentLaserState == firing) // if laser is currently firing
		{
			animateLaser(); // laser's path to mouse click is animated
		}

		if (m_currentLaserState == explosion) // if explosion is animated
		{
			m_laser.clear(); // clear laser vertex array
			m_currentPower = 0.0f; // reset power of power bar
			animateExplosion(); // explosion's radius enlargement is animated
		}

		if (m_currentAsteroidState == launch) // asteroid is about to launch
		{
			asteroidProperties(); // sets start and end point's of asteroid and gets direction of asteroid
			m_currentAsteroidState = flight; // asteroid is moving to it's destination
		}

		if (m_currentAsteroidState == flight) // if asteroid is moving
		{
			animateAsteroid(); // asteroid's path to mouse click is animated
		}

		if (m_currentAsteroidState == collision) // if asteroid collided with something
		{
			m_asteroid.clear(); // clear asteroid vertex array
			m_asteroidIntervalCounter++; // asteroid interval counter incremented
			if (m_asteroidIntervalCounter > m_asteroidInterval) // if counter is greater than random number
			{
				m_currentAsteroidState = launch; // asteroid is ready to launch
				m_asteroidInterval = rand() % 100 + 1.0f; // interval number randomized
				m_asteroidIntervalCounter = 0.0f; // counter reset
			}
		}

		m_expBar.setSize(sf::Vector2f{ m_xp, 20.0f }); // update size of xp bar
		m_scoreText.setString("Score: " + std::to_string(m_score) + "pts"); // update string of score text
		
		// update string of final score text
		m_totalScoreText.setString("TOTAL SCORE: " + std::to_string(m_score) + "pts");
		m_playerLvlText.setString("Level: " + std::to_string(m_playerLvl)); // update string of player level text
		
		// update string of score multiplier text
		m_scoreMultiplier.setString("Multiplier: x" + std::to_string(m_playerLvl));
		animatePowerBar(); // animates power bar's growth
	}
}


/// checks if left mouse button has been clicked
/// sets the start point, direction and velocity of laser
void Game::processMouseEvents(sf::Event t_mouseEvents)
{
	sf::Vector2i mouseClick{}; // position of mouse click

	if (sf::Mouse::Left == t_mouseEvents.mouseButton.button) // if left mouse button clicked
	{
		mouseClick = sf::Mouse::getPosition(m_window); // mouseClick set to position of mouse in the window

		// static cast mouse x and y coordinate into a vector
		m_laserDestination = sf::Vector2f(static_cast<float>(mouseClick.x), static_cast<float>(mouseClick.y));

		m_directionNormalised = m_laserDestination - m_laserStartPoint; // end point(Q) - start point(P)
		m_directionNormalised = vectorUnitVector(m_directionNormalised); // calculate direction of laser
		m_laserVelocity = m_directionNormalised * m_laserSpeed; // calculate speed of laser in given direction
		m_laserEndPoint = m_laserStartPoint + m_laserVelocity; // add fractions of line from start point to end point
	}
}


/// laser's journey to it's destination is animated
/// if reached either current max altitude or mouse click, trigger explosion
void Game::animateLaser()
{
	sf::Vertex lineStart{ m_laserStartPoint }; // start point vertex of laser
	sf::Vertex lineEnd{}; // end point vertex of laser, updated every frame
	
	if (m_laserEndPoint.y <= m_laserDestination.y) // laser reached mouse click location
	{
		m_currentLaserState = explosion; // explosion is triggered
	}

	else if (m_laserEndPoint.y <= m_altitude) // laser reaches max altitude based on power of power bar
	{
		m_currentLaserState = explosion; // explosion is triggered
	}

	else
	{
		m_laserEndPoint += m_laserVelocity; // end point updated with velocity

		lineEnd.position = m_laserEndPoint; // end point vertex updated

		// start and end point vertex added to laser vertex array
		m_laser.append(lineStart);
		m_laser.append(lineEnd);
	}
}


/// explosion is called to end point of laser, radius enlarged gradually
void Game::animateExplosion()
{
	m_explosion.setPosition(m_laserEndPoint); // position of explosion set to laser end point
	m_explosion.setOrigin(m_explosionRadius, m_explosionRadius); // origin of explosion set to explosion radius
	m_explosion.setRadius(m_explosionRadius); // explosion radius set to itself, radius is being updated
	m_explosionRadius++; // radius enlarged

	// distance between origin of explosion and end point of asteroid
	m_explosionCollisionDistance = (m_laserEndPoint.x - m_asteroidEndPoint.x) * (m_laserEndPoint.x - m_asteroidEndPoint.x)
		+ (m_laserEndPoint.y - m_asteroidEndPoint.y) * (m_laserEndPoint.y - m_asteroidEndPoint.y);
	m_explosionCollisionDistance = sqrt(m_explosionCollisionDistance);

	collisionDetection(); // check for collisions

	if (m_explosionRadius >= 30.0f) // if max radius reached
	{
		m_explosionRadius = 0.0f; // reset radius
		m_currentLaserState = standby; // laser set to 'waiting for input' state
	}
}


/// power bar width is enlarged based on current power
void Game::animatePowerBar()
{
	m_powerBar.setSize(sf::Vector2f{ m_currentPower, 30.0f }); // set size of power bar to updated width

	if (m_currentLaserState == standby) // if laser is waiting for input
	{
		if (m_currentPower >= MAX_POWER) // if max power reached
		{
			m_currentPower = MAX_POWER; // limit power
		}

		else
		{
			m_currentPower += m_powerInc; // increase power
		}
	}

	else if (m_currentLaserState == firing) // if laser is firing
	{
		m_altitude = (m_window.getSize().y - m_ground.getSize().y) - m_currentPower; // calculate altitude
	}
}

/// sets random start and end position of asteroid
/// sets the direction and velocity of asteroid
void Game::asteroidProperties()
{	
	float randomStartPoint = rand() % m_window.getSize().x + 1.0f; // random number <0 - window length>
	float randomEndPoint = rand() % m_window.getSize().x + 1.0f; // random number <0 - window length>

	m_asteroidStartPoint = sf::Vector2f{ randomStartPoint, 0.0f }; // set x coordinate of start point to random number
	m_asteroidEndPoint = sf::Vector2f{ randomEndPoint, 600.0f }; // set x coordinate of start point to random number

	m_asteroidDirectionNormalised = m_asteroidEndPoint - m_asteroidStartPoint; // end point(Q) - start point(P)
	m_asteroidDirectionNormalised = vectorUnitVector(m_asteroidDirectionNormalised); // calculate direction of laser
	m_asteroidVelocity = m_asteroidDirectionNormalised * m_asteroidSpeed; // calculate speed of asteroid in given direction
	m_asteroidEndPoint = m_asteroidStartPoint + m_asteroidVelocity; // add fractions of line from start point to end point
}


/// asteroid's journey from random start point to random end point is animated
/// if destination reached, game over
/// if shot down, respawn
void Game::animateAsteroid()
{
	sf::Vertex asteroidStart{ m_asteroidStartPoint }; // start point vertex of asteroid
	sf::Vertex asteroidEnd{ m_asteroidEndPoint }; // end point vertex of asteroid

	collisionDetection(); // checks if any collision has been detected

	m_asteroidEndPoint += m_asteroidVelocity; // end point updated with velocity

	// start and end point vertex added to asteroid vertex array
	m_asteroid.append(asteroidStart);
	m_asteroid.append(asteroidEnd);
}


/// checks for collisions
void Game::collisionDetection()
{
	// collision <asteroid end point - ground>
	if (m_asteroidEndPoint.y > m_ground.getPosition().y)
	{
		m_currentAsteroidState = collision; // asteroid's collision detected
		m_currentGameState = gameOver; // game is over
	}

	// collision <asteroid end point - explosion>
	if (m_explosionCollisionDistance < m_explosionRadius)
	{
		if (!m_scoreAwarded) // checks if score and xp was not yet rewarded
		{
			m_asteroidSpeed += 0.2f; // asteroid animation speed is increased
			m_score += 1 * m_playerLvl; // add score to player, multiplier increases score gained per player level
			m_xp += m_playerXpGain; // player gains xp
			m_scoreAwarded = true; // score has been awarded
		}

		if (m_currentGameState == customMode) // if custom mode is played
		{
			if (m_xp >= MAX_XP) // if player is eligible for a level up
			{
				levelUp(); // increases player level and improves players stats
			}
		}

		m_currentAsteroidState = collision; // asteroid's collision detected
	}
}


/// level up player, reduce xp gain per asteroid shot down and improved laser's speed
void Game::levelUp()
{
	m_playerLvl++; // level increased by 1
	m_playerXpGain /= 1.1f; // reduced player xp gain
	m_xp = 0.0f; // reset current player xp
	m_powerInc++; // power bar fills up faster
	m_laserSpeed += 1.0f; // laser speed improved

	if (m_laserSpeed >= 5.0f) // if max laser speed
	{
		m_laserSpeed = 5.0f; // limit laser speed
	}

	if (m_powerInc >= 10.0f) // if max power increment speed
	{
		m_powerInc = 10.0f; // limit power increment speed
	}
}


/// reset player stats such as xp, score, laser speed, etc. to default values
void Game::resetAttributes()
{
	m_playerLvl = 1; // player level rest
	m_playerXpGain = 50.0f; // xp gain reset
	m_xp = 0.0f; // xp reset
	m_score = 0; // score reset
	m_asteroidSpeed = 0.2f; // speed reset
	m_laserSpeed = 1.0f; // laser speed reset
	m_powerInc = 1.0f; // power bar increment reset
	m_currentPower = 0.0f; // current power reset
}


/// draw the frame and then switch buffers
void Game::render()
{
	m_window.clear();

	if (m_currentGameState == mainMenu) // only draw in main menu
	{
		m_window.draw(m_classicModeButton);
		m_window.draw(m_customModeButton);
		m_window.draw(m_classicModeText);
		m_window.draw(m_customModeText);
		m_window.draw(m_titleText);

		m_window.display();
	}

	// draw all basic components of game regardless of game mode
	if (m_currentGameState == classicMode || m_currentGameState == customMode)
	{
		m_window.draw(m_ground);
		m_window.draw(m_base);
		m_window.draw(m_laser);
		m_window.draw(m_asteroid);
		m_window.draw(m_powerBarBackground);
		m_window.draw(m_powerBar);
		m_window.draw(m_scoreText);
		

		if (m_currentLaserState == explosion)
		{
			m_window.draw(m_explosion);
		}

		if (m_currentGameState == customMode) // only draw when in custom mode
		{
			m_window.draw(m_expBarBackground);
			m_window.draw(m_expBar);
			m_window.draw(m_scoreMultiplier);
			m_window.draw(m_playerLvlText);
		}

		m_window.display();
	}

	if (m_currentGameState == gameOver) // only draw when game is over
	{
		m_window.clear();

		m_window.draw(m_gameOverText);
		m_window.draw(m_totalScoreText);
		m_window.draw(m_returnToMenuText);

		m_window.display();
	}
}


/// set up game over title text in game over screen
void Game::setupGameOverText()
{
	// arial black font is loaded
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	// set text attributes to game over title text
	m_gameOverText.setFont(m_ArialBlackfont);
	m_gameOverText.setString("GAME OVER!");
	m_gameOverText.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_gameOverText.setPosition(100.0f, 250.0f);
	m_gameOverText.setCharacterSize(80);
	m_gameOverText.setOutlineColor(sf::Color::Red);
	m_gameOverText.setFillColor(sf::Color::Black);
	m_gameOverText.setOutlineThickness(3.0f);
}


/// set up game over title text in game over screen
void Game::setupTitleText()
{
	// set text attributes to game title text in main menu
	m_titleText.setFont(m_ArialBlackfont);
	m_titleText.setString("MISSILE COMMAND: ONE");
	m_titleText.setStyle(sf::Text::Underlined | sf::Text::Bold);
	m_titleText.setPosition(120.0f, 100.0f);
	m_titleText.setCharacterSize(40);
	m_titleText.setOutlineColor(sf::Color::Yellow);
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setOutlineThickness(3.0f);
}


/// sets up general text for use in HUD elements
void Game::setupTextProperties(sf::Text & t_text, sf::Vector2f t_position, std::string t_string, int t_characterSize)
{
	t_text.setFont(m_ArialBlackfont); // set font of text
	t_text.setString(t_string); // set string of text
	t_text.setFillColor(sf::Color::White); // set color of text
	t_text.setPosition(t_position); // set position of text
	t_text.setCharacterSize(t_characterSize); // set character size of text
}


/// gives text variables properties based on setupTextProperties function
void Game::setupText()
{
	// set up score text
	setupTextProperties(m_scoreText, sf::Vector2f{ 10.0f, 555.0f }, "Score: " + std::to_string(m_score) + "pts", 18);
	
	// set up score multiplier text
	setupTextProperties(m_scoreMultiplier, sf::Vector2f{ 10.0f, 575.0f }, "Multiplier: x" + std::to_string(m_playerLvl), 18);
	
	// set up player level text
	setupTextProperties(m_playerLvlText, sf::Vector2f{ 648.0f, 518.0f }, "Level: " + std::to_string(m_playerLvl), 14);
	m_playerLvlText.setFillColor(sf::Color(232, 202, 9)); // custom yellow color

	// set up classic mode text
	setupTextProperties(m_classicModeText, sf::Vector2f{ 280.0f, 230.0f }, "<1> CLASSIC MODE", 24);
	m_classicModeText.setFillColor(sf::Color::Black);
	m_classicModeText.setStyle(sf::Text::Bold);

	// set up custom mode text
	setupTextProperties(m_customModeText, sf::Vector2f{ 280.0f, 380.0f }, "<2> CUSTOM MODE", 24);
	m_customModeText.setFillColor(sf::Color::Black);
	m_customModeText.setStyle(sf::Text::Bold);

	// set up return to menu mode text
	setupTextProperties(m_returnToMenuText, sf::Vector2f{ 100.0f, 500.0f }, "PRESS <SPACE> TO RETURN TO MAIN MENU", 24);
	setupTextProperties(m_totalScoreText, sf::Vector2f{ 100.0f, 350.0f }, "TOTAL SCORE: " + std::to_string(m_score) + "pts", 18);
	m_totalScoreText.setFillColor(sf::Color::Yellow);
}


/// sets up a rectangle shape's position and size
void Game::setupSceneProperties(sf::RectangleShape & t_rectangle, sf::Vector2f t_position, sf::Vector2f t_size)
{
	t_rectangle.setPosition(t_position); // position of rectangle shape
	t_rectangle.setSize(t_size); // size of rectangle shape
}


/// gives rectangle shapes properties based on setupSceneProperties function
void Game::setupScene()
{
	// set up ground rectangle
	setupSceneProperties(m_ground, sf::Vector2f{ 0.0f, 500.0f }, sf::Vector2f{ 800.0f, 100.0f });
	m_ground.setFillColor(sf::Color(2, 99, 20)); // dark green color
	
	// set up ground rectangle
	setupSceneProperties(m_base, sf::Vector2f{ 360.0f, 440.0f }, sf::Vector2f{ 80.0f, 60.0f });
	m_base.setFillColor(sf::Color(219, 199, 52)); // golden color
	
	// set up power bar rectangle
	setupSceneProperties(m_powerBar, sf::Vector2f{ 10.0f, 520.0f }, sf::Vector2f{ m_currentPower, 30.0f });
	m_powerBar.setFillColor(sf::Color(188, 5, 5)); // red color
	
	// set up power bar background rectangle
	setupSceneProperties(m_powerBarBackground, sf::Vector2f{ 8.0f, 518.0f }, sf::Vector2f{ 454.0f, 34.0f });
	m_powerBarBackground.setFillColor(sf::Color::Black);

	// set up xp bar rectangle
	setupSceneProperties(m_expBar, sf::Vector2f{ 650.0f, 540.0f }, sf::Vector2f{ m_xp, 20.0f });
	m_expBar.setFillColor(sf::Color(232, 202, 9));
	
	// set up xp bar background rectangle
	setupSceneProperties(m_expBarBackground, sf::Vector2f{ 648.0f, 538.0f }, sf::Vector2f{ 104.0f, 24.0f });
	m_expBarBackground.setFillColor(sf::Color::Black);

	// set up classic mode rectangle
	setupSceneProperties(m_classicModeButton, sf::Vector2f{ 270.0f, 200.0f }, sf::Vector2f{ 300.0f, 100.0f });
	m_classicModeButton.setFillColor(sf::Color(255, 199, 15)); // custom yellow color
	
	// set up custom mode rectangle
	setupSceneProperties(m_customModeButton, sf::Vector2f{ 270.0f, 350.0f }, sf::Vector2f{ 300.0f, 100.0f });
	m_customModeButton.setFillColor(sf::Color(216, 41, 10)); // custom red color
}
