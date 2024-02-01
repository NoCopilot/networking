#include "Networking.hpp"

//ip class

bool Ip::checkIp(std::string _ip)
{
	std::vector<std::string> blocks;
	bool check;
	
	if(_ip.size() == 32)
	{
		//checking if the ip is in bit format
		check = true;
		blocks = split(_ip, 8);
	}
	else
	{
		//or in normal format
		check = false;
		blocks = split(_ip, '.');
	}
	
	if(blocks.size() != 4) return false;
	int n;
	for(std::string& block : blocks)
	{
		n = (check ? bitToInt(block) : stringToInt(block));
		if(n < 0 || n > 255) return false;
	}
	return true;
}

std::string Ip::ipToDecimal(std::string _ip)
{
	std::vector<std::string> bytes = split(_ip, 8);
	std::string res("");
	int n;
	
    for(std::string& byte : bytes)
	{
		n = bitToInt(byte);
		if(n < 0 || n > 255) return "";
		res += intToString(n) + ".";
	}
	res.erase(res.size()-1);
	
	return res;
}

std::string Ip::ipToBit()
{
	std::vector<std::string> blocks = split(ip, '.');
	std::string res("");
	int n;
	
    for(std::string& block : blocks)
	{
		n = stringToInt(block);
		if(n < 0 || n > 255) return "";
		res += intToBit(n, 8);
	}
	
	return res;
}


Ip::Ip(std::string _ip)
{
    setIp(_ip);
}

std::string Ip::applyMask(Mask _mask)
{
	return "";
}


//sets
bool Ip::setIp(std::string _ip)
{
    if(checkIp(_ip)) ip = _ip;
    else
    {
        ip = "";
        return false;
    }

    if(ip.length() == 32) ip = ipToDecimal(ip);
    return true;
}


//gets
char Ip::getIpClass()
{
	return ip_class;
}

std::string Ip::getIp()
{
	return ip;
}

//mask class

Mask::Mask(std::string _mask)
{
    setMask(_mask);
}

bool Mask::checkMask(std::string _mask)
{

    return true;
}


//sets
bool Mask::setMask(std::string _mask)
{
    if(checkMask(_mask)) mask = _mask;
    else
    {
        mask = "";
        return false;
    }
    return true;
}

void Mask::setMask(int n)
{
    if(n < 0 || n > 32) return;
    mask = "";
    while(n >= 1) mask += "1";
    while(mask.length() < 32) mask += "0";
    mask = Ip(mask).ipToBit();
}

void Mask::setMask(char _class)
{
    if(_class < 'a' || _class > 'c')
    {
        mask = "";
        return;
    }
    setMask((_class-'a' + 1) * 8);
}


//gets
std::string Mask::getMask()
{
    return mask;
}

int Mask::getCdir()
{
    return cdir;
}


//util functions

//conversion
int stringToInt(std::string str)
{
	int n = -1;
	std::stringstream ss;
	ss << str;
	ss >> n;
	return n;
}

std::string intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}
std::string intToBit(int n, int size)
{
	if(n < 0) return "";
	std::string str = "";
	do
	{
		str = (char)(n % 2 + 48) + str;
		n /= 2;
	}while (n != 0);

	if(size == -1) return str;

	if(str.size() > size) return "";

	for(size_t i = size - str.size(); i > 0; i--)
		str = "0" + str;

	return str;
}
int bitToInt(std::string str)
{
	int n = 0, j = 1;
	for(int i = str.size() - 1; i >= 0; i--)
	{
		if(str[i] == '1') n += j;
		else if(str[i] != '0') return -1;
		j *= 2;
	}
	return n;
}

//power
int powOf2(int n)
{
	int i = 1, j = 0;
	while(i < n)
	{
		i *= 2;
		j++;
	}
	return j;
}
int power(int b, int e)
{
	int res = 1;
    for(int i = e; i >= 1; i--) res *= b;
    return res;
}

//strings
std::vector<std::string> split(std::string str, char del)
{
	std::vector<std::string> to_return;
	std::string temp = "";

    for(size_t i = 0; i < str.size(); i++)
	{
		if(del == str[i])
		{
			to_return.push_back(temp);
			temp = "";
			continue;
		}
		temp += str[i];
	}
	if(temp != "") to_return.push_back(temp);

	return to_return;
}
std::vector<std::string> split(std::string str, int range)
{
	std::vector<std::string> v;
	while(str.size() > range)
	{
		v.push_back(str.substr(0, range));
		str = str.substr(range);
	}
	v.push_back(str);
	return v;
}
