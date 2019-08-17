#include "Ball.h"

Ball::Ball()
{
	m_position = sf::Vector2f(800,450);
	m_velocity = sf::Vector2f(5,20);
	m_radius = 20.0f;
	m_color = sf::Color::Red;
}

void Ball::Init(sf::Vector2f position, sf::Vector2f velocity, float radius, sf::Color color)
{
	m_position = position;
	m_velocity = velocity;
	m_radius = radius;
	m_color = color;
	m_texture.loadFromFile("ball.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width/2.0f,m_sprite.getLocalBounds().height/2.0f);
	m_sprite.setPosition(m_position);

	if(!m_slowMoInBuffer.loadFromFile("SlowMoIn.ogg"))
		std::cout<<"could not load slowMoIn.ogg in Game::Init()";
	m_slowMoInSound.setBuffer(m_slowMoInBuffer);
	if(!m_slowMoOutBuffer.loadFromFile("SlowMoOut.ogg"))
		std::cout<<"could not load slowMoOut.ogg in Game::Init()";
	m_slowMoOutSound.setBuffer(m_slowMoOutBuffer);
	
	m_slowMoState = ESlowMoStates::NONE;
	m_initialVelocity = m_velocity;
}

void Ball::StartSlowMotion(float slowMoLowestSpeed, float durationIn, float slowMoHighestSpeed, float durationOut)
{
	if(m_slowMoState==ESlowMoStates::NONE)
	{
		m_slowMoTimeIn = durationIn;
		m_slowMoTimeOut = durationOut;
		m_slowMoLowestSpeed = slowMoLowestSpeed;
		m_slowMoHighestSpeed = slowMoHighestSpeed;
		m_slowMoDt = durationIn;
		m_slowMoState = ESlowMoStates::IN;
		m_slowMoInSound.play();
		m_initialVelocity = m_velocity;
		std::cout<<"isSlowedIn"<<std::endl;
	}
}

void Ball::Update(float dt)
{
	m_position += m_velocity * dt;
	if(m_slowMoState == ESlowMoStates::IN)
	{
		if(m_slowMoDt>=0.0f)
		{
			m_slowMoDt-=dt;
			if(m_slowMoDt<0.0f)
			{
				m_slowMoState = ESlowMoStates::OUT;
				m_slowMoOutSound.play();
				std::cout<<"isSlowedOut"<<std::endl;
				m_slowMoDt = m_slowMoTimeOut;
			}
		}
		m_velocity = Utilities::Normalized(m_velocity)*
			((Utilities::Magnitude(m_initialVelocity)-m_slowMoLowestSpeed)*
			pow(m_slowMoDt/m_slowMoTimeIn,1)+m_slowMoLowestSpeed);
	}
	if(m_slowMoState == ESlowMoStates::OUT)
	{
		if(m_slowMoDt>=0.0f)
		{
			m_slowMoDt-=dt;
			if(m_slowMoDt<0.0f)
			{
				m_slowMoState = ESlowMoStates::NONE;
				std::cout<<"SlowMoDone"<<std::endl;
			}
		}
		m_velocity = Utilities::Normalized(m_velocity)*
			((m_slowMoHighestSpeed-m_slowMoLowestSpeed)*
			(1-pow(m_slowMoDt/m_slowMoTimeOut,1))+m_slowMoLowestSpeed);
	}
}

void Ball::Draw(sf::RenderTarget& renderTarget)
{
	m_sprite.setPosition(m_position);
	renderTarget.draw(m_sprite);
}