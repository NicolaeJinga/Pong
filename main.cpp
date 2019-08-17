#include <SFML/Graphics.hpp>
#include<iostream>
#include "Game.h"

int main()
{
	Game game;
	game.Init();
	sf::Clock clock;
	float dt = 0.0f;
	float lag = 0.0f;
	float dtFixed = 1/60.f;
	while(game.m_window.isOpen())
	{
		sf::Event event;
		while(game.m_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				game.m_window.close();
		}
		dt = clock.restart().asSeconds();
		lag += dt;


		game.Update(dt);
// 		while (lag >= dtFixed)
// 		{
// 			game.Update(dtFixed);
// 			lag -= dtFixed;
// 		}
		//std::cout<<"FPS: "<<1/dt<<" lag:"<<lag<<std::endl;


		game.Draw();
		game.m_window.display();

	}
	return 0;

}