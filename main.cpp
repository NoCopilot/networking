#include "TextBox.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Ip.hpp"
#include <iostream>

void setUpTextBox(TextBox& t, sf::RenderWindow& win)
{
	t.setUp(win, 0, 0, 150, 200);
	t.setFont("font.ttf");
	t.setChSize(15);
	t.setOutlineColor(sf::Color::Black);
	t.setTextColor(sf::Color::Black);
	t.setBarColor(sf::Color::Black);
}

int main()
{
	sf::RenderWindow win(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 0.8f, sf::VideoMode::getDesktopMode().height * 0.8f), "Window");
	sf::Event e;
	
	Label ipLabel("ip Address", 18, sf::Color::Black, "font.ttf"), 
		maskLabel("subnetmask", 18, sf::Color::Black, "font.ttf");
	ipLabel.setPos({0, 0});
	maskLabel.setPos({0, 80});
	
	TextBox hosts, routers, ipInput, maskInput;
	
	Button btn;
	
	setUpTextBox(hosts, win);
	setUpTextBox(routers, win);
	setUpTextBox(ipInput, win);
	setUpTextBox(maskInput, win);
	
	ipInput.setPos({ipLabel.getSize().x + 10, 0.f});
	ipInput.setMultiLines(false);
	maskInput.setMultiLines(false);
	maskInput.setPos({maskLabel.getSize().x + 10, 80.f});
	
	
	
	while(win.isOpen())
	{
		while(win.pollEvent(e))
		{
			ipInput.listen(e);
			maskInput.listen(e);
			if(e.type == sf::Event::Closed) win.close();
		}
		win.clear(sf::Color::White);
		
		ipLabel.draw(win);
		maskLabel.draw(win);
		
		ipInput.draw();
		maskInput.draw();
		
		win.display();
	}
	
	return 0;
}
