#include<iostream>
#include "Game.h"
#include "Utilities.h"

void Game::Init()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	srand((unsigned int)time(NULL));

	m_onGameEnter = false;

	m_enterPressed = false;

	m_numberOfPallets = 2;

	m_winner = -1;

	m_window.create(sf::VideoMode(1600,900), "Pong", sf::Style::Default, settings);

	if(!m_font.loadFromFile("arial.ttf"))
		std::cout<<"could not load arial.ttf in Game::Init()";

	for(int i = 0 ; i < m_numberOfPallets ; ++ i)
	{
		m_text[i].setFont(m_font);
	}

	m_text[0].setPosition(sf::Vector2f(m_window.getSize().x/7.0f,		m_window.getSize().y/8.0f));
	m_text[1].setPosition(sf::Vector2f(m_window.getSize().x/7.0f*6.0f,	m_window.getSize().y/8.0f));

	for(int i = 0 ; i < m_numberOfPallets ; ++ i)
	{
		m_text[i].setString(std::to_string(m_pallets[i].m_score));
		m_text[i].setCharacterSize(100);
		m_text[i].setColor(sf::Color(255,255,255,150));
		m_text[i].setOrigin(m_text[i].getLocalBounds().width/2.0f,m_text[i].getLocalBounds().height/2.0f);
	}

	m_scorescreenText.setFont(m_font);
	m_scorescreenText.setString("Wins!");
	m_scorescreenText.setCharacterSize(150);
	m_scorescreenText.setColor(sf::Color(255,255,255));
	m_scorescreenText.setOrigin(m_scorescreenText.getLocalBounds().width/2.0f,m_scorescreenText.getLocalBounds().height/2.0f);
	m_scorescreenText.setPosition(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f);

	if(!m_splashscreenTexture.loadFromFile("splashscreen.png"))
		std::cout<<"could not load splashscreen.png in Game::Init()";
	m_splashscreenSprite.setTexture(m_splashscreenTexture);
	m_splashscreenSprite.setPosition(0.0f,0.0f);

	if(!m_menuTexture.loadFromFile("menu.png"))
		std::cout<<"could not load menu.png in Game::Init()";
	m_menuSprite.setTexture(m_menuTexture);
	m_menuSprite.setPosition(0.0f,0.0f);

	if(!m_hitSoundBuffer.loadFromFile("pong.wav"))
		std::cout<<"could not load pong.wav in Game::Init()";
	m_hitSound.setBuffer(m_hitSoundBuffer);
	

	ServeBall();
	
	m_pallets[0].m_texture.loadFromFile("pallet1.png");
	m_pallets[1].m_texture.loadFromFile("pallet2.png");
	m_pallets[0].Init(sf::Vector2f(100,m_window.getSize().y/2.0f),sf::Vector2f(1,0), 10, 100, sf::Color::Green, m_pallets[0].m_texture);
	m_pallets[1].Init(sf::Vector2f(1500,m_window.getSize().y/2.0f),sf::Vector2f(-1,0), 10, 100, sf::Color::Yellow, m_pallets[1].m_texture);

	m_borderWidth = 5.0f;

	m_currentState = SPLASHSCREEN;
}

void Game::Reset()
{
	for(int i = 0 ; i < m_numberOfPallets ; ++ i)
		m_pallets[i].m_score = 0;
	m_enterPressed = false;
	m_winner = -1;
	m_pallets[0].Init(sf::Vector2f(100,m_window.getSize().y/2.0f),sf::Vector2f(1,0), 10, 100, sf::Color::Green, m_pallets[0].m_texture);
	m_pallets[1].Init(sf::Vector2f(1500,m_window.getSize().y/2.0f),sf::Vector2f(-1,0), 10, 100, sf::Color::Yellow, m_pallets[1].m_texture);
}

void Game::ServeBall()
{
	//m_ball.Init(sf::Vector2f(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f), 500.0f * Utilities::CreateUnitVectorFromAngle((float)((rand()%2) ? (rand()%90+135) : (rand()%90 - 45))), 10, sf::Color::White);
	m_ball.Init(sf::Vector2f(50.0f,50.0f), 500.0f * Utilities::CreateUnitVectorFromAngle(30), 10, sf::Color::White);
}

void Game::Update(float dt)
{
	switch(m_currentState)
	{
	case SPLASHSCREEN:
		{
			m_enterPressed = false;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				m_enterPressed = true;
				m_currentState = MENU;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_window.close();
			break;
		}
	case MENU:
		{
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				m_enterPressed = false;
			if(!m_enterPressed)
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					m_enterPressed = true;
					m_currentState = GAME;
				}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_window.close();
			break;
		}
	case GAME:
		{
			if(!m_onGameEnter)
			{
				m_onGameEnter = true;
				Reset();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				m_ball.StartSlowMotion(20.0f,2.5f,1900.0f,1.0f);
			}

			//updating game objects
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_currentState = MENU;
			m_ball.Update(dt);
			for(int i = 0  ; i < m_numberOfPallets ; ++i)
				m_pallets[i].Update(dt);

			//collision pallets with borders
			for(int i =  0 ; i < m_numberOfPallets ; ++i)
			{
				if(m_pallets[i].m_position.y < m_borderWidth + m_pallets[i].m_height/2.0f)
					m_pallets[i].m_position.y = m_borderWidth + m_pallets[i].m_height/2.0f;
				if(m_pallets[i].m_position.y > (float)m_window.getSize().y - m_borderWidth - m_pallets[i].m_height/2.0f)
					m_pallets[i].m_position.y = (float)m_window.getSize().y - m_borderWidth - m_pallets[i].m_height/2.0f;
			}

			//collision ball with borders
			if(m_ball.m_position.y < m_borderWidth + m_ball.m_radius)
			{
				m_ball.m_position.y = m_borderWidth + m_ball.m_radius;
				m_ball.m_velocity.y = -m_ball.m_velocity.y;

				m_hitSound.play();
			}
			if(m_ball.m_position.y > (float)m_window.getSize().y - m_borderWidth - m_ball.m_radius)
			{
				m_ball.m_position.y = (float)m_window.getSize().y - m_borderWidth - m_ball.m_radius;
				m_ball.m_velocity.y = -m_ball.m_velocity.y;

				m_hitSound.play();
			}

			//collision ball with pallets
			if((m_ball.m_position.x < m_pallets[0].m_position.x + m_pallets[0].m_length/2.0f + m_ball.m_radius) && 
				(m_ball.m_position.y > m_pallets[0].m_position.y - m_pallets[0].m_height/2.0f) &&
				(m_ball.m_position.y < m_pallets[0].m_position.y + m_pallets[0].m_height/2.0f))
			{
				if(!(m_ball.m_position.x < m_pallets[0].m_position.x - m_pallets[0].m_length/2.0f - m_ball.m_radius))
				{
					m_ball.m_position.x = m_pallets[0].m_position.x + m_pallets[0].m_length/2.0f + m_ball.m_radius;

					float qq = Utilities::Dot(m_ball.m_position - m_pallets[0].m_position, sf::Vector2f(-m_pallets[0].m_normal.y,m_pallets[0].m_normal.x));

					float newAngle = (qq + m_pallets[0].m_height/2.0f) * 2 * m_pallets[0].m_angleFromNormal/m_pallets[0].m_height - m_pallets[0].m_angleFromNormal;

					sf::Vector2f unitDirection = Utilities::CreateUnitVectorFromAngle(newAngle + Utilities::GetAngle(m_pallets[0].m_normal));
					m_ball.m_velocity = unitDirection * Utilities::Magnitude(m_ball.m_velocity);

					m_hitSound.play();

					m_ball.m_velocity *=1.05f;
				}
			}
			if((m_ball.m_position.x > m_pallets[1].m_position.x - m_pallets[1].m_length/2.0f - m_ball.m_radius) && 
				(m_ball.m_position.y > m_pallets[1].m_position.y - m_pallets[1].m_height/2.0f) && 
				(m_ball.m_position.y < m_pallets[1].m_position.y + m_pallets[1].m_height/2.0f))
			{
				if(!(m_ball.m_position.x > m_pallets[1].m_position.x + m_pallets[1].m_length/2.0f + m_ball.m_radius))
				{
					m_ball.m_position.x = m_pallets[1].m_position.x - m_pallets[1].m_length/2.0f - m_ball.m_radius;

					float qq = Utilities::Dot(m_ball.m_position - m_pallets[1].m_position, sf::Vector2f(-m_pallets[1].m_normal.y,m_pallets[1].m_normal.x));

					float newAngle = (qq + m_pallets[1].m_height/2.0f) * 2 * m_pallets[1].m_angleFromNormal/m_pallets[1].m_height - m_pallets[1].m_angleFromNormal;

					sf::Vector2f unitDirection = Utilities::CreateUnitVectorFromAngle(newAngle + Utilities::GetAngle(m_pallets[1].m_normal));
					m_ball.m_velocity = unitDirection * Utilities::Magnitude(m_ball.m_velocity);

					m_hitSound.play();

					m_ball.m_velocity *=1.05f;
				}
			}

			bool ballIsOut = false;
			//reinit ball
			if(m_ball.m_position.x < 0)
			{
				ballIsOut = true;
				m_pallets[1].m_score++;
			}
			if(m_ball.m_position.x > m_window.getSize().x)
			{
				ballIsOut = true;
				m_pallets[0].m_score++;
			}
			if(ballIsOut)
			{
				ServeBall();
			}


			for(int i = 0  ; i < m_numberOfPallets ; ++ i)
			{
				m_text[i].setString(std::to_string(m_pallets[i].m_score));
				m_text[i].setOrigin(m_text[i].getLocalBounds().width/2.0f,m_text[i].getLocalBounds().height/2.0f);
			}

			//stopping pallets
			m_pallets[0].m_velocity.y = 0.0f;
			m_pallets[1].m_velocity.y = 0.0f;
			//moving pallets
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				m_pallets[0].m_velocity.y = -600.0f;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				m_pallets[0].m_velocity.y = 600.0f;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				m_pallets[1].m_velocity.y = -600.0f;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				m_pallets[1].m_velocity.y = 600.0f;

			//win condition
			for(int i = 0 ; i < m_numberOfPallets ; ++ i)
				if(m_pallets[i].m_score >= 5)
				{
					m_currentState = SCORESCREEN;
					m_winner = i;
				}
			break;
		}
	case SCORESCREEN:
		{
			m_onGameEnter = false;
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				m_enterPressed = false;
			if(!m_enterPressed)
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					m_enterPressed = true;
					m_currentState = MENU;
				}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_window.close();
			break;
		}
	}
}

void Game::Draw()
{
	m_window.clear(sf::Color::Black);
	switch(m_currentState)
	{
	case SPLASHSCREEN:
		{
			m_window.draw(m_splashscreenSprite);
			break;
		}
	case MENU:
		{
			m_window.draw(m_menuSprite);
			break;
		}
	case GAME:
		{
			//draw borders
			sf::RectangleShape topBorder(sf::Vector2f((float)m_window.getSize().x,m_borderWidth));
			sf::RectangleShape botBorder(sf::Vector2f((float)m_window.getSize().x,m_borderWidth));
			topBorder.setPosition(0.0f,0.0f);
			botBorder.setPosition(0.0f,m_window.getSize().y - m_borderWidth);
			topBorder.setFillColor(sf::Color::Red);
			botBorder.setFillColor(sf::Color::Red);
			m_window.draw(topBorder);
			m_window.draw(botBorder);

			
			//draw game objects
			m_ball.Draw(m_window);
			for(int i = 0  ; i < m_numberOfPallets ; ++i)
			{
				m_pallets[i].Draw(m_window);
				m_window.draw(m_text[i]);
			}
			break;
		}
	case SCORESCREEN:
		{
			m_window.clear();
			m_pallets[m_winner].m_position = (sf::Vector2f(m_window.getSize().x/2.0f,m_window.getSize().y/3.0f));
			m_pallets[m_winner].Draw(m_window);
			m_window.draw(m_scorescreenText);
			break;
		}
	}
}