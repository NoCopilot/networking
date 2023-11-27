#include "Ip.hpp"

int toInt(std::string str)
{
	int n = -1;
	std::stringstream ss;
	ss << str;
	ss >> n;
	return n;
}

int bitToInt(std::string str)
{
	int n = 0, j = 1;
	for (int i = str.size() - 1; i >= 0; i--)
	{
		if (str[i] == '1') n += j;
		j *= 2;
	}
	return n;
}

std::string toString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

std::string toBit(int n, int s = -1)
{
	if (n < 0) return "";
	std::string str = "";
	do
	{
		str = (char)(n % 2 + 48) + str;
		n /= 2;
	} while (n != 0);

	if (s == -1) return str;

	if (str.size() > s) return "";

	for (int i = s - (int)str.size(); i > 0; i--)
		str = "0" + str;

	return str;
}

std::string getmask(int n)
{
	if (n < 1 || n > 31) return "";
	std::string to_return = "";
	for (int i = 0; i < 32; i++)
	{
		if (n > 0)
		{
			to_return += "1";
			n--;
		}
		else to_return += "0";
	}

	return to_return;
}

int getPowOf2(int n)
{
	int i = 1, j = 0;
	while (i < n)
	{
		i *= 2;
		j++;
	}
	return j;
}

std::vector<std::string> split(std::string str, char del)
{
	std::vector<std::string> to_return;
	std::string temp = "";

	for (int i = 0; i < (int)str.size(); i++)
	{
		if (del == str[i])
		{
			to_return.push_back(temp);
			temp = "";
			continue;
		}
		temp += str[i];
	}
	if (temp != "") to_return.push_back(temp);

	return to_return;
}

std::vector<std::string> split(std::string str, int every)
{
	std::vector<std::string> v;
	while (str.size() > every)
	{
		v.push_back(str.substr(0, every));
		str = str.substr(every);
	}
	v.push_back(str);
	return v;
}

std::string bitTOdec(std::string str)
{
	std::vector<std::string> v = split(str, 8);
	str = "";
	for (std::string s : v)
	{
		int j = 1, n = 0;
		for (int i = (int)s.size() - 1; i >= 0; i--)
		{
			if (s[i] == '1') n += j;
			j *= 2;
		}
		str += toString(n) + ".";
	}
	str.erase(str.size() - 1);

	return str;
}