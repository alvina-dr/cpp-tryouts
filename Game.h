#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

class Game
{

private:
	sf::RenderWindow* window;
	sf::Event ev;
	void initVariables();
	void initWindow();

public:
	Game();
	virtual ~Game();

	void update();
	void render();
	void pollEvents();
	const bool running() const;
	Texture t;
	int dx = 0;
	int dy = 0;
	struct Point {
		int x, y;
	} a[4], b[4];
};

