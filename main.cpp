#include<iostream>
#include "Game.h"
#include<time.h>

using namespace sf;


struct Point { float x, y; } player[1], platform[10];
float dx = 0;
float dy = 0;

int main() 
{
	srand(time(0));
	//VARIABLES
	RenderWindow* window = new RenderWindow(VideoMode(800, 600), "GAME", Style::Titlebar | Style::Close);
	window->setFramerateLimit(60);
	Texture texPlayer, texPlatform, texBackground;
	texPlayer.loadFromFile("Resources/sprite.png");
	texPlatform.loadFromFile("Resources/tileset-terrain.png");
	texBackground.loadFromFile("Resources/background.png");
	Sprite playerS(texPlayer), platformS(texPlatform), platformStartS(texPlatform), platformEndS(texPlatform), backgroundS(texBackground);
	float ground = 373;
	playerS.setTextureRect(IntRect(0, 0, 18, 18));
	playerS.setScale(3, 3);
	platformS.setTextureRect(IntRect(32, 0, 32, 32));
	platformS.setScale(2, 2);
	platformStartS.setTextureRect(IntRect(6*32, 32, 32, 32));
	platformStartS.setScale(2, 2);
	platformEndS.setTextureRect(IntRect(7*32, 32, 32, 32));
	platformEndS.setScale(2, 2);
	backgroundS.setTextureRect(IntRect(0, -150, 500, 300));
	backgroundS.setPosition(window->getSize().x / 2 - 250, window->getSize().y / 2 - 150);
	player[0].x = window->getSize().x / 2;
	player[0].y = ground+1;
	playerS.setPosition(player[0].x, player[0].y);
	playerS.setOrigin(playerS.getTextureRect().width / 2, playerS.getTextureRect().height / 2);
	bool onGround = true, rightPressed = false, leftPressed = false;
	int platformNumber = 10;

	//TEXT
	Text debugText;
	Font font;
	font.loadFromFile("Resources/Inter-Regular.ttf");
	debugText.setFont(font);
	debugText.setFillColor(Color::White);
	debugText.setCharacterSize(16);
	debugText.setPosition(window->getSize().x / 2, 15.f );

	for (int i = 0; i < platformNumber; i++) {
		platform[i].x = 83 + 64*i;
		platform[i].y = 400;
	}

	while (window->isOpen()) {
		Event ev;

		//debug text
		std::string debugString = std::to_string(player[0].x) + ", " + std::to_string(player[0].y);
		debugText.setString(debugString);
		while (window->pollEvent(ev)) {
			switch (ev.type)
			{
			case Event::Closed:
				window->close();
				break;
			case Event::KeyPressed:
				//horizontal
				if (ev.key.code == Keyboard::Left) {
					if (rightPressed) rightPressed = false;
					leftPressed = true;
					dx = -1;
					playerS.setScale(-abs(playerS.getScale().x), playerS.getScale().y);
				}
				else if (ev.key.code == Keyboard::Right) {
					if (leftPressed) leftPressed = false;
					rightPressed = true;
					dx = 1;
					playerS.setScale(abs(playerS.getScale().x), playerS.getScale().y);
				}

				if (ev.key.code == Keyboard::Space) {
					//JUMP
					if (onGround) {
						onGround = false;
					}
				}
				break;
			case Event::KeyReleased:
				if (ev.key.code == Keyboard::Left && leftPressed && rightPressed == false) {
					leftPressed = false;
					rightPressed = false;
					dx = 0;
				}
				if (ev.key.code == Keyboard::Right && rightPressed && leftPressed == false) {
					leftPressed = false;
					rightPressed = false;
					dx = 0;
				}			
			default:
				break;
			}
		}

		if (player[0].y < ground - 50) { //chute
			dy += .4;
		}
		else if (dy > 0 && player[0].y > ground) {
			onGround = true;
			dy = 0;
		}
		else if (onGround == false) { //saut
			dy -= 1.2f;
		}

		player[0].y += dy * 2;
		player[0].x += dx * 5;

		Color color(51, 50, 61, 1);
		window->clear(color);
		window->draw(backgroundS);

		playerS.setPosition(player[0].x, player[0].y);
		for (int i = 0; i < platformNumber; i++) {
			if (i == 0) {
				platformStartS.setPosition(platform[i].x, platform[i].y);
				window->draw(platformStartS);
			}
			else if (i == platformNumber-1) {
				platformEndS.setPosition(platform[i].x, platform[i].y);
				window->draw(platformEndS);
			}
			else {
				platformS.setPosition(platform[i].x, platform[i].y);
				window->draw(platformS);
			}
		}
		window->draw(playerS);
		window->draw(debugText);
		window->display();
	}

	return 0;
}