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

	void processEvents();
	void update(sf::Time t_deltaTime);
	void processMouseEvents(sf::Event t_mouseEvent);
	void animateLaser();
	void AsteroidProperties();
	void render();

	void setupFontAndText();
	void setupSprite();
	void setUpScene(sf::RectangleShape & t_rectangle, sf::Vector2f t_position, sf::Vector2f t_size);

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	sf::RectangleShape m_ground;
	sf::RectangleShape m_base;
	sf::RectangleShape m_powerBar;

	bool m_exitGame; // control exiting game

	sf::VertexArray m_laser{ sf::Lines };
	
	// start position of laser at base
	sf::Vector2f m_laserStartPoint{ 0.0f, 0.0f };
	
	sf::Vector2f m_laserDestination{ 0.0f, 0.0f }; // destination of laser based on mouse click
	sf::Vector2f m_laserEndPoint{ 0.0f, 0.0f }; // tip of laser, extended in update() by velocity
	sf::Vector2f m_directionNormalised{ 0.0f, 0.0f }; // direction the laser will travel
	sf::Vector2f m_laserVelocity{ 0.0f, 0.0f }; // laser velocity
	float m_laserSpeed = 2.0f; // laser speed
	
	enum m_laserState{standby, firing};
	m_laserState m_currentLaserState = standby;


	sf::VertexArray m_asteroid{ sf::Lines };

	

};

#endif // !GAME

