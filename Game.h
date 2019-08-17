#ifndef __GAME_H__
#define __GAME_H__

#include "SFML/Graphics.hpp"
#include "Pallet.h"
#include "Ball.h"
#include "SFML/Audio.hpp"

class Game
{
public:
	Pallet m_pallets[2];
	Ball m_ball;

	sf::Texture m_splashscreenTexture;
	sf::Sprite m_splashscreenSprite;

	sf::Texture m_menuTexture;
	sf::Sprite m_menuSprite;

	sf::Font m_font;
	sf::Text m_text[2];
	sf::Text m_scorescreenText;

	sf::SoundBuffer m_hitSoundBuffer;
	sf::Sound m_hitSound;

	


	float m_borderWidth;
	int m_numberOfPallets;

	int m_winner;

	bool m_onGameEnter;

	bool m_enterPressed;

	sf::RenderWindow m_window;

	enum GameState
	{
		SPLASHSCREEN,
		MENU,
		GAME,
		SCORESCREEN,
		COUNT
	};

	int m_currentState;

	void Init();
	void Reset();
	void ServeBall();
	void Update(float dt);
	void Draw();
};

#endif // __GAME_H__