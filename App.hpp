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
		win = new sf::RenderWindow(sf::VideoMode(800, 500), "Window", sf::Style::Titlebar | sf::Style::Close);
		e = new sf::Event();

		setUpTextBox(ipInput, *win);
		setUpTextBox(maskInput, *win);
		setUpTextBox(hosts, *win);
		setUpTextBox(routers, *win);
		setUpTextBox(links, *win);

		float gap = 30.f;

		//ip label
		ipLabel.setText("ip:");
		ipLabel.setFont("font.ttf");
		ipLabel.setChSize(18);
		ipLabel.setColor(sf::Color::Black);
		ipLabel.setPos({ 0, 10.f });
		//ip inputbox
		ipInput.setPos({ ipLabel.getSize().x + gap * 0.3f, 10.f });
		ipInput.setMultiLines(false);
		//mask label
		maskLabel.setText("mask:");
		maskLabel.setFont("font.ttf");
		maskLabel.setChSize(18);
		maskLabel.setColor(sf::Color::Black);
		maskLabel.setPos({ ipInput.getSize().x + ipInput.getPos().x + gap, 10.f});
		//mask inputbox
		maskInput.setPos({ maskLabel.getSize().x + maskLabel.getPos().x + gap * 0.3f, 10.f});
		maskInput.setMultiLines(false);

		//hosts inputbox (line: int, [optional]string)
		hosts.setMultiLines(true);
		hosts.setPos({ 0.f, 130.f });
		hosts.setSize({ 200.f, 200.f });
		//router link to subnet (line: [router name]string, [subnetname]string)
		routers.setMultiLines(true);
		routers.setPos({ hosts.getSize().x + hosts.getPos().x + gap, 130.f });
		routers.setSize({ 200.f, 200.f });
		//links between routers (line: [router name]string, [router name]string)
		links.setMultiLines(true);
		links.setPos({ routers.getSize().x + routers.getPos().x + gap, 130.f });
		links.setSize({ 200.f, 200.f });

		btn.setChSize(20);
		btn.setText("Calculate");
		btn.setFont("font.ttf");
		btn.setColor(sf::Color::White);
		btn.setTextColor(sf::Color::Black);
		btn.setBorder(1, sf::Color::Black);
		btn.setPos(win->getSize().x - btn.getSize().x - 10, win->getSize().y - btn.getSize().y - 10);
		btn.setFunction([this]() {onClick(); });
	}

	void begin()
	{
		win->setFramerateLimit(50);
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
	TextBox hosts, routers, ipInput, maskInput, links;
	Button btn;

	void setUpTextBox(TextBox& t, sf::RenderWindow& win)
	{
		t.setUp(win, 0, 0, 180, 200);
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

			hosts.listen(*e);
			routers.listen(*e);
			links.listen(*e);

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
		hosts.draw();
		routers.draw();
		links.draw();

		btn.draw(win);

		win->display();
	}

	void onClick()
	{
		ip.reset();
		if (!ip.loadIp(ipInput.getText()[0]))
			return;
		if (ip.getCdir() == 0)
		{
			if (!ip.loadMask(maskInput.getText()[0]))
				return;
		}
		std::vector<int> nhosts;
		std::vector<std::string> subnetname;
		std::vector<sf::String> shosts = hosts.getText();
		char ch = 'A';
		for (int i = 0; i < shosts.size(); i++)
		{
			std::vector<std::string> temp = split(shosts[i], ' ');
			nhosts.push_back(toInt(temp[0]));

			if (temp.size() > 1) subnetname.push_back(temp[1]);
			else 
			{
				subnetname.push_back("");
				subnetname[i] += ch++;
			}
		}
		ip.loadHosts(nhosts, subnetname);
		std::vector<std::string> result = ip.subnet();
		
		if (nhosts.size() != result.size())
		{
			std::cout << result.size() << "\n";
			return;
		}
		for (std::string str : result)
		{
			std::cout << str + "\n";
		}
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