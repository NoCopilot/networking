#include <iostream>
#include <string>
#include <sstream>

std::string and_bit(std::string, std::string);
std::string getMask(int);
std::string getMask(std::string);
std::string toBit(std::string);
bool checkIp(std::string);
bool checkMask(std::string);
int toInt(std::string);

int main()
{
	std::string ip = "192.168.100.10/22";
	if(!checkIp(ip))
	{
		return 0;
	}
	
	
	std::cout<<getMask(22);
	
	return 0;
}

std::string and_bit(std::string ip, std::string mask)
{
	if(!checkIp(ip)) return "";
	
	int cdir_pos = ip.rfind('/');
	if(cdir_pos != -1)
	{
		mask = toInt(ip.substr(cdir_pos + 1));
		ip = ip.substr(0, cdir_pos);
	}
	else if(!checkMask(mask)) return "";
	
}

std::string getMask(int n)
{
	if(n > 32) return "";
	if(n < 1) return "";
	
	std::string to_return = "";
	for(int i=0; i<32; i++)
	{
		if(n > 0)
		{
			to_return += "1";
			n--;
		}
		else to_return += "0";
		
		if((i+1) % 8 == 0)
		{
			if(i == 31) break;
			to_return += ".";
		}
	}
	
	return to_return;
}

std::string toBit(std::string str)
{
	return "";
}

bool checkIp(std::string ip)
{
	int j = 0
	for(int i=0; i<ip.size(); ++i)
	{
		
	
		return false;
	}
	
	return true;
}

bool checkMask(std::string mask)
{
	
	
	return true;
}

int toInt(std::string str)
{
	return -1;
}
