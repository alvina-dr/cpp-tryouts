#include "Game.h"

using namespace sf;

//void Game::initVariables()
//{
//	this->window = nullptr;
//}
//
//void Game::initWindow()
//{
//	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "GAME", sf::Style::Titlebar | sf::Style::Close);
//}
//
//Game::Game() {
//	this->initVariables();
//	this->initWindow();
//
//	
//}
//
//Game::~Game() {
//	delete this->window;
//}
//
//void Game::update() {
//
//
//	this->pollEvents();
//}
//
//
//void Game::render() {
//
//	this->t.loadFromFile("Resources/sprite.png");
//	Sprite s(t);
//	s.setTextureRect(IntRect(0, 0, 18, 18));
//
//	this->window->clear(sf::Color::Black);
//	this->window->draw(s);
//	this->window->display();
//
//}
//
//void Game::pollEvents()
//{
//
//	while (this->window->pollEvent(this->ev)) {
//
//		switch (this->ev.type)
//		{
//		case sf::Event::Closed:
//			this->window->close();
//			break;
//		case sf::Event::KeyPressed:
//			if (ev.key.code == sf::Keyboard::Left) {
//				dx = -1;
//
//			}
//			else if (ev.key.code == sf::Keyboard::Right) {
//				dx = 1;
//			}
//			break;
//		default:
//			break;
//		}
//		s ->x += dx;
//	}
//
//}
//
//const bool Game::running() const
//{
//	return this->window->isOpen();
//}
