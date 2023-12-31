#ifndef APP_HPP
#define APP_HPP

#include "TextBox.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Ip.hpp"
#include <iostream>

int vfind(std::vector<std::string> v, std::string str)
{
	for (int i = 0; i < v.size(); i++)
		if (v[i] == str) return i;
	return -1;
}

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
		setUpLabel(ipLabel, "ip:", { 0, 10.f });
		//ip inputbox
		ipInput.setPos({ ipLabel.getSize().x + gap * 0.3f, 10.f });
		ipInput.setMultiLines(false);
		//mask label
		setUpLabel(maskLabel, "mask:", { ipInput.getSize().x + ipInput.getPos().x + gap, 10.f});
		//mask inputbox
		maskInput.setPos({ maskLabel.getSize().x + maskLabel.getPos().x + gap * 0.3f, 10.f});
		maskInput.setMultiLines(false);

		//hosts input label
		setUpLabel(subnetsLabel, "Hosts", { 0.f, 110.f});
		//hosts inputbox (line: int, [optional]string)
		hosts.setMultiLines(true);
		hosts.setPos({ 0.f, 130.f });
		hosts.setSize({ 200.f, 200.f });
		
		//router subnet link label
		setUpLabel(subnetsRouterLinksLabel, "Routers", {hosts.getSize().x + hosts.getPos().x + gap + 10, 110.f});
		//router link to subnet (line: [router name]string, [subnetname]string)
		routers.setMultiLines(true);
		routers.setPos({ hosts.getSize().x + hosts.getPos().x + gap, 130.f });
		routers.setSize({ 200.f, 200.f });
		
		//router links label
		setUpLabel(routerLinksLabel, "Links", {routers.getSize().x + routers.getPos().x + gap + 10, 110.f});
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

	Label ipLabel, maskLabel, subnetsLabel, subnetsRouterLinksLabel, routerLinksLabel;
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
	
	void setUpLabel(Label& label, sf::String text, sf::Vector2f pos)
	{
		label.setText(text);
		label.setFont("font.ttf");
		label.setChSize(18);
		label.setColor(sf::Color::Black);
		label.setPos(pos);
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
		subnetsLabel.draw(win);
		subnetsRouterLinksLabel.draw(win);
		routerLinksLabel.draw(win);
		
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
		{
			errorPopup("Not valid ip address");
			return;
		}
		
		if (ip.getCdir() == 0)
		{
			if (!ip.loadMask(maskInput.getText()[0]))
			{
				errorPopup("Not valid mask");
				return;
			}
		}

		std::vector<int> nhosts;
		std::vector<std::string> subnetList, routerList;
		std::vector<sf::String> tboxvalue = hosts.getText();
		std::vector<std::vector<std::string>> tempLinks;
		std::string totalResult = "";
		//getting subnet names and host number + 2 (broadcast and network address)
		for (int i = 0; i < tboxvalue.size(); i++)
		{
			std::vector<std::string> temp = split(tboxvalue[i], ' ');
			if (temp.size() != 2)
			{
				if (tboxvalue[i].getSize() == 0) continue;
				errorPopup("Subnet needs a name: " + toString(i));
				return;
			}

			int n = vfind(subnetList, temp[1]);
			if (n == -1)
			{
				//subnet name doesn't exist
				n = toInt(temp[0]) + 2;
				if (n < 0)
				{
					errorPopup("Negative numbers not accepted");
					return;
				}
				subnetList.push_back(temp[1]);
				nhosts.push_back(n);
			}
			else
			{
				//subnet name exist -> add to existing subnet the number of hosts
				if (toInt(temp[0]) < 0) return;
				nhosts[n] += toInt(temp[0]);
			}
		}
		
		//getting routers names
		tboxvalue = routers.getText();
		tempLinks = std::vector<std::vector<std::string>>(nhosts.size());
		for (int i = 0; i < tboxvalue.size(); i++)
		{
			std::vector<std::string> temp = split(tboxvalue[i], ' ');
			if (temp.size() != 2)
			{
				if (tboxvalue[i].getSize() == 0) continue;
				errorPopup("Not valid line in router textbox: " + toString(i));
				return;
			}
			//checking if subnet name exist
			int n = vfind(subnetList, temp[1]);
			if(n != -1)
			{
				//checking if router name exist
				if (vfind(routerList, temp[0]) == -1) routerList.push_back(temp[0]);
				//add a gateway address
				nhosts[n]++;
				tempLinks[n].push_back(temp[0]);
				continue;
			}
			errorPopup("Not subnet with that name exist: " + toString(i));
			return;
		}
		
		//getting the subnets
		ip.loadHosts(nhosts, subnetList, tempLinks);
		std::vector<network> result = ip.subnet(1);
		if (result.size() == 0)
		{
			errorPopup("Couldn't calculate the subnets, check your input");
			return;
		}

		//printing subnetting result
		totalResult += "subnets:\n";
		for (int i = 0; i < result.size(); i++)
		{
			totalResult += result[i].name + ":\n";
			totalResult += "  network address: " + result[i].networkAddress + "\n";
			totalResult += "  gateways:\n";
			for (int j = 0; j < result[i].gatewayAdresses.size(); j++)
				totalResult += "    " + result[i].gatewayNames[j] + " : " + result[i].gatewayAdresses[j] + "\n";
			totalResult += "  broadcast address: " + result[i].broadcastAddress + "\n";
		}
		
		//getting links
		nhosts = std::vector<int>(routerList.size(), 3);
		tempLinks = std::vector<std::vector<std::string>>(routerList.size());
		tboxvalue = links.getText();
		for (int i = 0; i < tboxvalue.size(); i++)
		{
			std::vector<std::string> temp = split(tboxvalue[i], ' ');
			if (temp.size() != 2)
			{
				if (tboxvalue[i].getSize() == 0) continue;
				errorPopup("Not valid line in links textbox: " + toString(i));
				return;
			}
			//check existing router name
			int n = vfind(routerList, temp[0]);
			if (n == -1)
			{
				routerList.push_back(temp[0]);
				tempLinks.push_back({temp[1]});
				nhosts.push_back(4);
				continue;
			}
			//add the link
			tempLinks[n].push_back(temp[1]);
			nhosts[n]++;
		}
		//clearing routers with no connections
		for (int i = 0; i < nhosts.size(); i++)
		{
			if (nhosts[i] == 3)
			{
				nhosts.erase(nhosts.begin() + i);
				i--;
			}
		}
		//getting the routers result
		if (nhosts.size() > 0)
		{
			ip.loadHosts(nhosts, routerList, tempLinks);
			result = ip.subnet(1);
			if (result.size() == 0) 
			{
				errorPopup("Couldn't calculate router links, check your input");
				return;
			}

			//printing routers result
			totalResult += "routers:\n";
			for (int i = 0; i < result.size(); i++)
			{
				totalResult += result[i].name + ":\n";
				totalResult += "  network address: " + result[i].networkAddress + "\n";
				totalResult += "  router address: " + result[i].address + "\n";
				for (int j = 0; j < result[i].gatewayAdresses.size(); j++)
					totalResult += "  " + result[i].gatewayNames[j] +" : " + result[i].gatewayAdresses[j] + "\n";
				totalResult += "  broadcast address: " + result[i].broadcastAddress + "\n";
			}
		}
		popup(totalResult);

	}

	void errorPopup(std::string error)
	{
		Label label;
		setUpLabel(label, error, {0.f, 0.f});
		label.setChSize(20);
		sf::RenderWindow window(sf::VideoMode(label.getSize().x + 20, label.getSize().y + 10), "Error", sf::Style::Titlebar | sf::Style::Close);
		window.setFramerateLimit(50);
		sf::Event event;

		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) window.close();
			}
			window.clear(sf::Color::White);

			label.draw(&window);

			window.display();
		}
	}

	void popup(std::string str)
	{
		sf::RenderWindow window(sf::VideoMode(500, 500), "Result", sf::Style::Titlebar | sf::Style::Close);
		window.setFramerateLimit(50);
		sf::Event event;
		TextBox textbox;
		setUpTextBox(textbox, window);
		textbox.setMultiLines(true);
		textbox.write(str);
		textbox.setSize({500.f, 500.f});
		textbox.setEditable(false);

		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				textbox.listen(event);
				if (event.type == sf::Event::Closed) window.close();
			}
			window.clear(sf::Color::White);

			textbox.draw();

			window.display();
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