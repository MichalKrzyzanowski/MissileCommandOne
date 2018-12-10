// Author: Michal K.

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	// functions
	void processEvents();
	void update(sf::Time t_deltaTime); // Update the game world
	void processMouseEvents(sf::Event t_mouseEvent); // checks if left mouse button has been clicked
	void animateLaser(); // laser's journey to it's destination is animated
	void animateExplosion(); // explosion is called to end point of laser, radius enlarged gradually
	void animatePowerBar(); // power bar width is enlarged based on current power
	void asteroidProperties(); // sets random start and end position of asteroid
	void animateAsteroid(); // asteroid's journey from random start point to random end point is animated
	void collisionDetection(); // checks for collisions
	void levelUp(); // level up player, reduce xp gain per asteroid shot down and improved laser's speed
	void resetAttributes(); // reset player stats such as xp, score, laser speed, etc. to default values
	void render(); // draw the frame and then switch buffers

	void setupGameOverText(); // set up game over title text in game over screen
	void setupTitleText(); // set up game over title text in game over screen
	
	// sets up general text for use in HUD elements
	void setupTextProperties(sf::Text & t_text, sf::Vector2f t_position, std::string t_string, int t_characterSize);
	
	void setupText(); // gives text variables properties based on setupTextProperties function
	
	// sets up a rectangle shape's position and size
	void setupSceneProperties(sf::RectangleShape & t_rectangle, sf::Vector2f t_position, sf::Vector2f t_size);
	
	void setupScene(); // gives rectangle shapes properties based on setupSceneProperties function


	// variables
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	sf::Text m_gameOverText; // game over text when game is over
	sf::Text m_titleText; // game title text in main menu
	sf::Text m_scoreText; // score Text
	sf::Text m_totalScoreText; // final score  text display when game is over
	sf::Text m_scoreMultiplier; // multiplier of score gained
	sf::Text m_playerLvlText; // player current level text
	sf::Text m_classicModeText; // classic mode text on button
	sf::Text m_customModeText; // custom mode text on button
	sf::Text m_returnToMenuText; // return to main menu prompt text

	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	int m_score = 0; // current player score
	int m_playerLvl = 1; // current player level
	float m_xp = 0.0f; // experience points earned by shooting down asteroids
	float m_playerXpGain = 50.0f; // current xp gain from shooting down asteroids

	const float MAX_XP = 100.0f; // max length of xp bar

	sf::RectangleShape m_ground; // ground shape
	sf::RectangleShape m_base; // base shape
	sf::RectangleShape m_powerBar; // power bar shape
	sf::RectangleShape m_powerBarBackground; // background of power bar, no functionality 
	sf::RectangleShape m_expBar; // player's xp bar shape
	sf::RectangleShape m_expBarBackground; // background of xp bar, no functionality 
	sf::RectangleShape m_classicModeButton; // button shape representing classic mode in main menu
	sf::RectangleShape m_customModeButton; // button shape representing custom mode in main menu
	sf::CircleShape m_explosion; // explosion circle shape

	bool m_exitGame; // control exiting game
	bool m_scoreAwarded{ false }; // checks if score and xp was already awarded

	const float MAX_POWER = 450.0f; // max power of power bar and max altitude of laser
	float m_currentPower = 0.0f; // current power of power bar and altitude of laser
	float m_powerInc = 1.0f; // power bar increment value
	float m_altitude = 0.0f; // lasers altitude
	float m_explosionRadius = 0.0f; // radius of laser explosion


	// laser line variables
	sf::VertexArray m_laser{ sf::Lines }; // vertex array of lines for laser
	
	// start position of laser at base
	sf::Vector2f m_laserStartPoint{ 0.0f, 0.0f }; // start position of laser
	
	sf::Vector2f m_laserDestination{ 0.0f, 0.0f }; // destination of laser based on mouse click
	sf::Vector2f m_laserEndPoint{ 0.0f, 0.0f }; // tip of laser, extended in update() by velocity
	sf::Vector2f m_directionNormalised{ 0.0f, 0.0f }; // direction the laser will travel
	sf::Vector2f m_laserVelocity{ 0.0f, 0.0f }; // laser speed in a direction
	float m_laserSpeed = 1.0f; // speed of laser's animation
	

	// asteroid line variables
	sf::VertexArray m_asteroid{ sf::Lines }; // vertex array of lines for asteroid

	sf::Vector2f m_asteroidStartPoint{ 0.0f, 0.0f }; // start position of asteroid
	sf::Vector2f m_asteroidEndPoint{ 0.0f, 0.0f }; // end position of asteroid

	sf::Vector2f m_asteroidDestination{ 0.0f , 0.0f }; // actual destination of asteroid pre-animation
	sf::Vector2f m_asteroidDirectionNormalised{ 0.0f, 0.0f }; // direction the asteroid will travel
	sf::Vector2f m_asteroidVelocity{ 0.0f, 0.0f }; // asteroid speed in a direction
	float m_asteroidSpeed = 0.4f; // speed of asteroid's animation
	float m_explosionCollisionDistance = 0.0f; // distance between asteroid end point and explosion radius
	float  m_asteroidInterval = 0.0f; // random interval between each asteroid launch
	float m_asteroidIntervalCounter = 0.0f; // counter for random interval


	// state machines
	enum m_laserState { standby, firing, explosion }; // all possible states of laser
	m_laserState m_currentLaserState = standby; // current laster state

	enum m_asteroidState {launch, flight, collision}; // all possible states of asteroid
	m_asteroidState m_currentAsteroidState = launch; // current asteroid state

	enum m_gameState { mainMenu, classicMode, customMode, gameOver }; // all possible states of game
	m_gameState m_currentGameState = mainMenu; // current asteroid state

};

#endif // !GAME

