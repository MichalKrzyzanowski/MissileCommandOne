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
	sf::Vector2f m_laserDestination{ 0, 0 };

	sf::VertexArray m_asteroid{ sf::Lines };

};

#endif // !GAME

