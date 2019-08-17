#ifndef __BALL_H__
#define __BALL_H__

#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Utilities.h"

class Ball
{
public:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_radius;
	sf::Color m_color;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::SoundBuffer m_slowMoInBuffer;
	sf::Sound m_slowMoInSound;
	sf::SoundBuffer m_slowMoOutBuffer;
	sf::Sound m_slowMoOutSound;

	enum ESlowMoStates
	{
		NONE = 0,
		IN,
		OUT
	};

	ESlowMoStates m_slowMoState;
	float m_slowMoDt;
	float m_slowMoTimeIn;
	float m_slowMoTimeOut;
	sf::Vector2f m_initialVelocity;
	float m_slowMoLowestSpeed;
	float m_slowMoHighestSpeed;

	Ball();

	void Init(sf::Vector2f position, sf::Vector2f velocity, float radius, sf::Color color);
	void StartSlowMotion(float slowMoLowestSpeed, float durationIn, float slowMoHighestSpeed, float durationOut);
	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
};


#endif // __BALL_H__