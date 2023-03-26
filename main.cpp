#include<iostream>
#include "Grid.h"
#include<time.h>

using namespace sf;

//declarations
struct Tile { float x = 0, y = 0; Sprite sprite; }  platform[32*32];
Image levelDesign;
Texture texPlayer, texTileset, texBackground, levelTexture;
Sprite playerS, platformM, platformS, platformE, backgroundS, leftWall, 
rightWall, rightPlatformWall, leftPlatformWall, roofM, leftRoofWall, rightRoofWall, leftRoofWall2, rightRoofWall2;
Font font;
RenderWindow* window;
View view;
int platformNumber = 32*32, cameraMaxOffset = 50;
float ground = 373;
bool onGround = false, jumpAscending = false, gravityOn = false;
Vector2f velocity = Vector2f(0, 0);
RectangleShape playerCollider, playerObject;
const float movementSpeed = 0.7f;
float dt;
Clock dt_clock;
FloatRect nextPos;
float jumpStartPosition;

Vector3i GetPixelRGB(Image image, int x, int y) {
	int red = image.getPixel(x, y).r;
	int green = image.getPixel(x, y).g;
	int blue = image.getPixel(x, y).b;
	return Vector3i(red, green, blue);
}

void DebugLog(std::string _log) {
	std::cout << _log << std::endl;
}

void Start() {
	levelDesign.loadFromFile("Resources/level1.png");
	texPlayer.loadFromFile("Resources/sprite.png");
	texTileset.loadFromFile("Resources/tileset-terrain.png");
	texBackground.loadFromFile("Resources/background.png");
	font.loadFromFile("Resources/Inter-Regular.ttf");

	platformM.setTexture(texTileset);
	platformM.setTextureRect(IntRect(32, 0, 32, 32));
	platformM.setScale(2, 2);

	platformS.setTexture(texTileset);
	platformS.setTextureRect(IntRect(6 * 32, 32, 32, 32));
	platformS.setScale(2, 2);

	platformE.setTexture(texTileset);
	platformE.setTextureRect(IntRect(7 * 32, 32, 32, 32));
	platformE.setScale(2, 2);

	leftWall.setTexture(texTileset);
	leftWall.setTextureRect(IntRect(64, 32, 32, 32));
	leftWall.setScale(2, 2);

	rightWall.setTexture(texTileset);
	rightWall.setTextureRect(IntRect(0, 32, 32, 32));
	rightWall.setScale(2, 2);

	rightPlatformWall.setTexture(texTileset);
	rightPlatformWall.setTextureRect(IntRect(64, 0, 32, 32));
	rightPlatformWall.setScale(2, 2);

	leftPlatformWall.setTexture(texTileset);
	leftPlatformWall.setTextureRect(IntRect(0, 0, 32, 32));
	leftPlatformWall.setScale(2, 2);

	leftRoofWall.setTexture(texTileset);
	leftRoofWall.setTextureRect(IntRect(6*32, 0, 32, 32));
	leftRoofWall.setScale(2, 2);

	rightRoofWall.setTexture(texTileset);
	rightRoofWall.setTextureRect(IntRect(7*32, 0, 32, 32));
	rightRoofWall.setScale(2, 2);

	leftRoofWall2.setTexture(texTileset);
	leftRoofWall2.setTextureRect(IntRect(64, 64, 32, 32));
	leftRoofWall2.setScale(2, 2);

	rightRoofWall2.setTexture(texTileset);
	rightRoofWall2.setTextureRect(IntRect(0, 64, 32, 32));
	rightRoofWall2.setScale(2, 2);

	roofM.setTexture(texTileset);
	roofM.setTextureRect(IntRect(32, 64, 32, 32));
	roofM.setScale(2, 2);

	backgroundS.setTexture(texBackground);
	backgroundS.setTextureRect(IntRect(0, -150, 1650, 300));

	srand(time(0));
	window = new RenderWindow(VideoMode(800, 600), "GAME", Style::Titlebar | Style::Close);
	window->setFramerateLimit(60);
	view.setSize(Vector2f(800, 600));

	int levelSize = levelDesign.getSize().x * levelDesign.getSize().y;

	backgroundS.setPosition(100, window->getSize().y / 2 - 170);

	playerCollider.setFillColor(Color::Transparent);
	playerCollider.setOutlineColor(Color::Green);
	playerCollider.setOutlineThickness(2.0f);
	playerCollider.setSize(Vector2f(64.0f, 64.0f));
	playerCollider.setOrigin(playerObject.getSize().x / 2, playerObject.getSize().y / 2);
	playerCollider.setPosition(300, 300);

	playerObject.setTexture(&texPlayer);
	playerObject.setTextureRect(IntRect(0, 0, 18, 18));
	playerObject.setSize(Vector2f(64.0f, 64.0f));
	playerObject.setOrigin(playerObject.getSize().x/2, playerObject.getSize().y / 2);
	playerObject.setPosition(300, 300);

	//int tileIndex = 0;
	for (int y = 0; y < levelDesign.getSize().x; y++) {
		for (int x = 0; x < levelDesign.getSize().y; x++) {
			platform[x + levelDesign.getSize().x * y].x = 64.0f * x;
			platform[x + levelDesign.getSize().x * y].y = 64.0f * y;
			if (levelDesign.getPixel(x, y) == Color(0, 0, 0, 255)) { //black
				platform[x + levelDesign.getSize().x * y].sprite = platformM;
			}
			else if (levelDesign.getPixel(x, y) == Color(255, 255, 255, 255)) { //white
				platform[x + levelDesign.getSize().x * y].sprite = roofM;
			}
			else if (levelDesign.getPixel(x, y) == Color(255, 0, 0, 255)) { //red
				platform[x + levelDesign.getSize().x * y].sprite = platformS;
			}
			else if (levelDesign.getPixel(x, y) == Color(0, 0, 255, 255)) { //blue
				platform[x + levelDesign.getSize().x * y].sprite = platformE;
			}
			else if (levelDesign.getPixel(x, y) == Color(0, 255, 0, 255)) { //green
				platform[x + levelDesign.getSize().x * y].sprite = leftWall;
			}
			else if (levelDesign.getPixel(x, y) == Color(0, 255, 255, 255)) { //cyan
				platform[x + levelDesign.getSize().x * y].sprite = rightWall;
			} 	
			else if (levelDesign.getPixel(x, y) == Color(255, 0, 255, 255)) { //magenta
				platform[x + levelDesign.getSize().x * y].sprite = rightPlatformWall;
			}
			else if (levelDesign.getPixel(x, y) == Color(255, 255, 0, 255)) { //yellow
				platform[x + levelDesign.getSize().x * y].sprite = leftPlatformWall;
			}
			else if (levelDesign.getPixel(x, y) == Color(150, 0, 0, 255)) { //dark red
				platform[x + levelDesign.getSize().x * y].sprite = leftRoofWall;
			}
			else if (levelDesign.getPixel(x, y) == Color(0, 0, 150, 255)) { //dark blue
				platform[x + levelDesign.getSize().x * y].sprite = rightRoofWall;
			}
			else if (levelDesign.getPixel(x, y) == Color(0, 150, 0, 255)) { //dark green
				platform[x + levelDesign.getSize().x * y].sprite = leftRoofWall2;
			}
			else if (levelDesign.getPixel(x, y) == Color(150, 150, 0, 255)) { //dark yellow
				platform[x + levelDesign.getSize().x * y].sprite = rightRoofWall2;
			}
		}
	}
}

void Update() {
	Event ev;
	dt = dt_clock.restart().asMilliseconds();
	while (window->pollEvent(ev)) {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			playerObject.setScale(-abs(playerObject.getScale().x), playerObject.getScale().y);
			velocity.x = -movementSpeed * dt;
		} else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			playerObject.setScale(abs(playerObject.getScale().x), playerObject.getScale().y);
			velocity.x = movementSpeed * dt;
		}
		else {
			velocity.x = 0;
		}
		switch (ev.type)
		{
		case Event::Closed:
			window->close();
			break;
		case Event::KeyPressed:
			if (ev.key.code == Keyboard::Up) {
				//JUMP
				if (onGround) {
					onGround = false;
					jumpAscending = true;
					jumpStartPosition = playerObject.getPosition().y;
					velocity.y = -1.0f * dt;
				}
			}
			if (ev.key.code == Keyboard::G) {
				gravityOn = true;
			}
			break;
		default:
			break;
		}
	}

	if (!onGround && jumpAscending && playerObject.getPosition().y > jumpStartPosition - 170) {
		velocity.y += .05f * dt;
	}
	else if (playerObject.getPosition().y <= jumpStartPosition - 170) {
		jumpAscending = false;

		if (gravityOn) velocity.y = .7f * dt;
	}
	else {
		if(gravityOn) velocity.y = .7f * dt;
	}

	FloatRect playerBounds = playerObject.getGlobalBounds();
	nextPos = playerBounds;
	nextPos.left += velocity.x;
	nextPos.top += velocity.y;
	playerCollider.setPosition(nextPos.left, nextPos.top);


	Color color(51, 50, 61, 1);
	window->clear(color);
	window->draw(backgroundS);

	for (int y = 0; y < levelDesign.getSize().x; y++) {
		for (int x = 0; x < levelDesign.getSize().y; x++) {
			if (platform[x + levelDesign.getSize().x * y].sprite.getTexture() != NULL) {
				platform[x + levelDesign.getSize().x * y].sprite.setPosition(platform[x + levelDesign.getSize().x * y].x, platform[x + levelDesign.getSize().x * y].y);
				window->draw(platform[x + levelDesign.getSize().x * y].sprite);
				FloatRect platformBounds = FloatRect(Vector2f(platform[x + levelDesign.getSize().x * y].x, platform[x + levelDesign.getSize().x * y].y), Vector2f(64.0f, 64.0f));
				if (platformBounds.intersects(nextPos)) {
					//COLLISION
					if (playerBounds.top < platformBounds.top && playerBounds.top + playerBounds.height < platformBounds.top + platformBounds.height &&
						playerBounds.left < platformBounds.left + platformBounds.width && playerBounds.left + playerBounds.width > platformBounds.left) {
						velocity.y = 0;
						onGround = true;
					}
					if (playerBounds.top > platformBounds.top && playerBounds.top + playerBounds.height > platformBounds.top + platformBounds.height &&
						playerBounds.left < platformBounds.left + platformBounds.width && playerBounds.left + playerBounds.width > platformBounds.left) {
						velocity.y = 0;
					}
					if (playerBounds.left < platformBounds.left && playerBounds.left + playerBounds.width < platformBounds.left + platformBounds.width &&
						playerBounds.top < platformBounds.top + platformBounds.height && playerBounds.top + playerBounds.height > platformBounds.top) {
						velocity.x = 0;
					}
					if (playerBounds.left > platformBounds.left && playerBounds.left + playerBounds.width > platformBounds.left + platformBounds.width &&
						playerBounds.top < platformBounds.top + platformBounds.height && playerBounds.top + playerBounds.height > platformBounds.top) {
						velocity.x = 0;
					}

				}
			}

		}
	}

	playerObject.setPosition(playerObject.getPosition().x + velocity.x, playerObject.getPosition().y + velocity.y);

	window->draw(playerCollider);
	window->draw(playerObject);

	//CAMERA CONTROLLER
	if (view.getCenter().x - playerObject.getPosition().x > cameraMaxOffset) {
		view.setCenter(Vector2f(playerObject.getPosition().x + cameraMaxOffset, view.getCenter().y));
	}
	else if (view.getCenter().x - playerObject.getPosition().x < -cameraMaxOffset) {
		view.setCenter(Vector2f(playerObject.getPosition().x - cameraMaxOffset, view.getCenter().y));
	}
	if (view.getCenter().y - playerObject.getPosition().y > cameraMaxOffset) {
		view.setCenter(Vector2f(view.getCenter().x, playerObject.getPosition().y + cameraMaxOffset));
	}
	else if (view.getCenter().y - playerObject.getPosition().y < -cameraMaxOffset) {
		view.setCenter(Vector2f(view.getCenter().x, playerObject.getPosition().y - cameraMaxOffset));
	}
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window->setView(view);
	window->display();
}

int main() 
{
	Start();

	while (window->isOpen()) { //update equivalent
		Update();		
	}
}

