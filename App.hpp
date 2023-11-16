#ifndef APP_HPP
#define APP_HPP

#include "TextBox.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Ip.hpp"
#include <iostream>

class App
{
public:
	App()
	{
		win = new sf::RenderWindow(sf::VideoMode(600, 500), "Window");
		win->setPosition({win->getPosition().x, 0});
		e = new sf::Event();

		ipLabel.setText("ip:");
		ipLabel.setFont("font.ttf");
		ipLabel.setChSize(18);
		ipLabel.setColor(sf::Color::Black);
		ipLabel.setPos({ 0, 0 });

		maskLabel.setText("mask:");
		maskLabel.setFont("font.ttf");
		maskLabel.setChSize(18);
		maskLabel.setColor(sf::Color::Black);
		maskLabel.setPos({ 0, 80 });

		setUpTextBox(hosts, *win);
		setUpTextBox(routers, *win);
		setUpTextBox(ipInput, *win);
		setUpTextBox(maskInput, *win);

		ipInput.setPos({ ipLabel.getSize().x + 10, 0.f });
		ipInput.setMultiLines(false);
		maskInput.setMultiLines(false);
		maskInput.setPos({ maskLabel.getSize().x + 10, 80.f });
	}

	void begin()
	{
		while (win->isOpen())
		{
			listen();
			draw();
		}
	}
private:
	sf::RenderWindow* win;
	sf::Event* e;

	Label ipLabel, maskLabel;
	TextBox hosts, routers, ipInput, maskInput;
	Button btn;

	void setUpTextBox(TextBox& t, sf::RenderWindow& win)
	{
		t.setUp(win, 0, 0, 150, 200);
		t.setFont("font.ttf");
		t.setChSize(15);
		t.setOutlineColor(sf::Color::Black);
		t.setTextColor(sf::Color::Black);
		t.setBarColor(sf::Color::Black);
		t.setScrollBarSize(8);
	}

	void listen()
	{
		while (win->pollEvent(*e))
		{
			ipInput.listen(*e);
			maskInput.listen(*e);
			if (e->type == sf::Event::Closed) win->close();
		}
	}

	void draw()
	{
		win->clear(sf::Color::White);

		ipLabel.draw(win);
		maskLabel.draw(win);

		ipInput.draw();
		maskInput.draw();

		win->display();
	}
};

#endif