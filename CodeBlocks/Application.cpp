#include "Application.hpp"

#include <iostream>

using namespace std;
using namespace sf;

Application::Application() :
    _window_size(largeurFenetre, hauteurFenetre),
    _window(),
    _game(&_window_size)
{
	_window.setKeyRepeatEnabled(false);
    _window.create(VideoMode(_window_size.x, _window_size.y), "Tetris de Jimmy, Gabriel, Alexandre et Quentin", Style::Close | Style::Titlebar);

    imageFond.loadFromFile("fond.png");
    _background.setTexture(imageFond);
    _background.setPosition(sf::Vector2f(0,0));
}

Application::~Application()
{
}

void Application::run()
{
	while (_window.isOpen())
	{
		processEvents();
		update();
		render();
	}
}

void Application::processEvents()
{
    Event event;
	while (_window.pollEvent(event))
	{
		switch(event.type)
		{
            case Event::Closed:
                _window.close();
            break;
            default:
                _game.onEvent(event);
            break;
		} //SWITCH
	}
}

void Application::update()
{
    _game.update();
}

void Application::render()
{
    _window.clear(sf::Color::White);
    _window.draw(_background);
    _window.draw(_game);
	_window.display();
}