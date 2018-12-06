// author Peter Lowe
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
	void update(sf::Time t_deltaTime);
	void processMouseEvents(sf::Event t_mouseEvent);
	void animateLaser();
	void animateExplosion();
	void animatePowerBar();
	void asteroidProperties();
	void animateAsteroid();
	void collisionDetection();
	void levelUp();
	void render();

	void setupGameOverText();
	void setupTextProperties(sf::Text & t_text, sf::Vector2f t_position, std::string t_string, int t_characterSize);
	void setupText();
	void setupSprite();
	void setupSceneProperties(sf::RectangleShape & t_rectangle, sf::Vector2f t_position, sf::Vector2f t_size);
	void setupScene();

	// variables
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_gameOverText; // text used for message on screen
	sf::Text m_scoreText; // score Text
	sf::Text m_scoreMultiplier; // multiplier of score gained
	sf::Text m_playerLvlText;
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	int m_score = 0;
	int m_playerLvl = 1;
	float m_xp = 0.0f; // experience points earned by shooting down asteroids
	float m_playerXpGain = 50.0f;

	const float MAX_XP = 100.0f;

	sf::RectangleShape m_ground;
	sf::RectangleShape m_base;
	sf::RectangleShape m_powerBar;
	sf::RectangleShape m_powerBarBackground;
	sf::RectangleShape m_expBar;
	sf::RectangleShape m_expBarBackground;
	sf::CircleShape m_explosion;

	bool m_exitGame; // control exiting game
	bool m_scoreAwarded{ false }; // checks if score and xp was already awarded

	const float MAX_POWER = 450.0f; // max power of power bar and max altitude of laser
	float m_currentPower = 0.0f; // current power of power bar and altitude of laser
	float m_powerInc = 10.0f; // power bar increment value
	float m_altitude = 0.0f; // lasers altitude
	float m_explosionRadius = 0.0f; // radius of laser explosion


	// laser line variables
	sf::VertexArray m_laser{ sf::Lines };
	
	// start position of laser at base
	sf::Vector2f m_laserStartPoint{ 0.0f, 0.0f };
	
	sf::Vector2f m_laserDestination{ 0.0f, 0.0f }; // destination of laser based on mouse click
	sf::Vector2f m_laserEndPoint{ 0.0f, 0.0f }; // tip of laser, extended in update() by velocity
	sf::Vector2f m_directionNormalised{ 0.0f, 0.0f }; // direction the laser will travel
	sf::Vector2f m_laserVelocity{ 0.0f, 0.0f }; // laser velocity
	float m_laserSpeed = 2.0f; // laser speed
	

	// asteroid line variables
	sf::VertexArray m_asteroid{ sf::Lines };

	sf::Vector2f m_asteroidStartPoint{ 0.0f, 0.0f };
	sf::Vector2f m_asteroidEndPoint{ 0.0f, 0.0f };

	sf::Vector2f m_asteroidDestination{ 0.0f , 0.0f };
	sf::Vector2f m_asteroidDirectionNormalised{ 0.0f, 0.0f };
	sf::Vector2f m_asteroidVelocity{ 0.0f, 0.0f };
	float m_asteroidSpeed = 0.5f;
	float m_explosionCollisionDistance = 0.0f; // distance between asteroid end point and explosion radius
	float  m_asteroidInterval = 0.0f; // random interval between each asteroid launch
	float m_asteroidIntervalCounter = 0.0f; // counter for random interval


	// state machines
	enum m_laserState { standby, firing, explosion };
	m_laserState m_currentLaserState = standby;

	enum m_asteroidState {launch, flight, collision};
	m_asteroidState m_currentAsteroidState = launch;

	enum m_gameState { gameRunning, gameOver, };
	m_gameState m_currentGameState = gameRunning;

};

#endif // !GAME

