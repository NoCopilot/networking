#ifndef APP_HPP
#define APP_HPP

class App
{
	public:
		App()
		{
			sf::RenderWindow win = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 0.8f, sf::VideoMode::getDesktopMode().height * 0.8f), "Window");
			sf::Event e = new sf::Event();
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
		}
}

#endif