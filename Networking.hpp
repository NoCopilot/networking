#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Mask
{
	public:
		Mask(std::string);
		static bool checkMask(std::string);
		
		//sets
		bool setMask(std::string);
		void setMask(int);
		void setMask(char);
		
		//gets
		std::string getMask();
		int getCdir();
	private:
		std::string mask;
		int cdir;
};

class Ip
{
	public:
		//class functions
		static bool checkIp(std::string);
		static std::string ipToDecimal(std::string);
		std::string ipToBit();
		
		Ip(std::string);
		std::string applyMask(Mask);
		
		//sets
		bool setIp(std::string);
		
		//gets
		char getIpClass();
		std::string getIp();
	private:
		//variables
		std::string ip;
		char ip_class;
};

struct Subnet
{
    std::string name, networkAddress, broadcastAddress;
    std::vector<std::string> gatewayAdresses, gatewayNames;
    int cdir;
};

struct SubnetInfo
{
    std::string name;
    int hosts;
    std::vector<std::string> gateways;
};


std::vector<Subnet> subnet(Ip, std::vector<SubnetInfo>, bool);

//UTIL FUNCTIONS

//conversion
int stringToInt(std::string);
std::string intToString(int);
std::string intToBit(int, int = -1);
int bitToInt(std::string);

//power
int powOf2(int);
int power(int, int);

//strings
std::vector<std::string> split(std::string, char);
std::vector<std::string> split(std::string, int);

#endif
