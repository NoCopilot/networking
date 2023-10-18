#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::string and_bit(std::string, std::string);
std::string getMask(int);
std::string getMask(std::string);
std::string toBit(std::string);
std::vector<std::string> split(std::string, char);
bool checkIp(std::string);
bool checkMask(std::string);
int toInt(std::string);

int main()
{
	return 0;
}

std::string and_bit(std::string ip, std::string mask)
{
	std::string result = "";
	for(int i=0; i<32; i++)
	{
		if(ip[i] == '.')
		{
			result += "."; continue;
		}
		result += (ip[i] != mask[i] ? '0' : ip[i]);
	}
	return result;
}

std::string getMask(int n)
{
	if(n < 0 || n > 32) return "";
	
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

std::vector<std::string> split(std::string str, char del)
{
	std::vector<std::string> to_return;
	std::string temp = "";
	for(int i=0; i<str.size(); i++)
	{
		if(del == str[i])
		{
			to_return.push_back(temp);
			temp = "";
			continue;
		}
		temp += str[i];
	}
	if(to_return.size() == 0 && temp != "") to_return.push_back(temp);
	
	return to_return;
}

std::string toBit(std::string str)
{
	
	return "";
}

bool checkIp(std::string ip)
{
	//divisione in bloccchi dell'ip e controllo size
	std::vector<std::string> tokens = split(ip, '.');
	if(tokens.size() != 4) return false;
	//controllo blocchi
	for(std::string token : tokens)
	{
		int block = toInt(token);
		if(block < 0 || block > 255) return false;
	}
	return true;
}

bool checkMask(std::string mask)
{
	
	
	return true;
}

int toInt(std::string str)
{
	int n = -1;
	std::stringstream ss;
	ss << str;
	ss >> n;
	
	return n;
}
