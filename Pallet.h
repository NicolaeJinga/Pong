#ifndef __PALLET_H__
#define __PALLET_H__

#include "SFML/Graphics.hpp"

class Pallet
{
public:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_length;
	float m_height;
	sf::Color m_color;
	sf::Vector2f m_normal;
	float m_angleFromNormal;
	int m_score;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	Pallet();

	void Init(sf::Vector2f position, sf::Vector2f normal, float length, float height, sf::Color color, sf::Texture texture);
	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
};


#endif // __PALLET_H__