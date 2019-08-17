#include "Pallet.h"

Pallet::Pallet()
{
	m_position = sf::Vector2f(900,600);
	m_velocity = sf::Vector2f(0.0f,0.0f);
	m_angleFromNormal = 80;
	m_length = 60;
	m_height = 10;
	m_color = sf::Color::Red;
	m_score = 0;
}

void Pallet::Init(sf::Vector2f position, sf::Vector2f normal, float length, float height, sf::Color color, sf::Texture texture)
{
	m_position = position;
	m_normal = normal;
	m_length = length;
	m_height = height;
	m_color = color;
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width/2.0f,m_sprite.getLocalBounds().height/2.0f);
	m_sprite.setPosition(m_position);
}

void Pallet::Update(float dt)
{
	m_position += m_velocity * dt;	
}

void Pallet::Draw(sf::RenderTarget& renderTarget)
{
	m_sprite.setPosition(m_position);
	renderTarget.draw(m_sprite);
}