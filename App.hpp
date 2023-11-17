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
		win = new sf::RenderWindow(sf::VideoMode(600, 500), "Window", sf::Style::Titlebar | sf::Style::Close);
		e = new sf::Event();

		ipLabel.setText("ip:");
		ipLabel.setFont("font.ttf");
		ipLabel.setChSize(18);
		ipLabel.setColor(sf::Color::Black);
		ipLabel.setPos({ 0, 10 });

		maskLabel.setText("mask:");
		maskLabel.setFont("font.ttf");
		maskLabel.setChSize(18);
		maskLabel.setColor(sf::Color::Black);
		maskLabel.setPos({ 0, 60 });

		setUpTextBox(hosts, *win);
		setUpTextBox(routers, *win);
		setUpTextBox(ipInput, *win);
		setUpTextBox(maskInput, *win);

		ipInput.setPos({ ipLabel.getSize().x + 10, 10.f });
		ipInput.setMultiLines(false);
		maskInput.setMultiLines(false);
		maskInput.setPos({ maskLabel.getSize().x + 10, 60.f });

		btn.setChSize(20);
		btn.setText("Calculate");
		btn.setFont("font.ttf");
		btn.setColor(sf::Color::White);
		btn.setTextColor(sf::Color::Black);
		btn.setBorder(1, sf::Color::Black);
		btn.setPos(600 - btn.getSize().x - 10, 500 - btn.getSize().y - 10);
		btn.setFunction([this]() {onClick(); });
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

	Ip ip;

	Label ipLabel, maskLabel;
	TextBox hosts, routers, ipInput, maskInput;
	Button btn;

	void setUpTextBox(TextBox& t, sf::RenderWindow& win)
	{
		t.setUp(win, 0, 0, 200, 200);
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
			if (ipInput.lostFocus()) checkIp();

			maskInput.listen(*e);
			if (maskInput.lostFocus()) checkMask();

			btn.listen(*e);

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

		btn.draw(win);

		win->display();
	}

	void onClick()
	{
		std::cout << ip.getIpClass() << "\n";
		std::cout << ip.getIp() << "\n";
		std::cout << ip.getMask() << "\n";
		std::cout << ip.getCdir() << "\n";
	}

	void checkIp()
	{
		if (!ip.loadIp(ipInput.getText()[0]))
		{
			ipInput.setOutlineColor(sf::Color::Red);
			unlock:{
				maskInput.Lock(false);
				maskInput.setTextColor(sf::Color::Black);
			}
			return;
		}
		ipInput.setOutlineColor(sf::Color::Green);
		
		if (ip.getCdir() != 0)
		{
			maskInput.Lock(true);
			maskInput.setTextColor(sf::Color(100, 100, 100));
			maskInput.setOutlineColor(sf::Color::Black);
		}
		else goto unlock;
	}

	void checkMask()
	{
		if (!ip.loadMask(maskInput.getText()[0]))
		{
			maskInput.setOutlineColor(sf::Color::Red);
			return;
		}
		maskInput.setOutlineColor(sf::Color::Green);
	}
};

#endif