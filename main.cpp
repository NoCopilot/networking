#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::string and_bit(std::string, std::string);
std::string getMask(int);
std::string getMask(std::string);
std::string toBit(std::string);
std::vector<std::string> split(std::string, char);
bool checkIp(std::string&, std::string&);
bool checkMask(std::string&);
int toInt(std::string);
char getClass(std::string);
int main()
{
	std::string ip = "198.168.1.1/24",
				submask = "";
	
	std::string type = "fixed"; //or vlsm			
	std::vector<int> hosts;
	hosts.push_back(20);
	hosts.push_back(30);
	hosts.push_back(10);
	
	if(!checkIp(ip, submask)) return 0;
	
	char ipclass = getClass(ip);
	
	return 0;
}



char getClass(std::string ip)
{
	int n = 0;
	while(ip[n] == '1') n++;
	
	return 'a' + n;
}

std::string and_bit(std::string ip, std::string mask)


{


	if(ip.size() != mask.size()) return "";
	std::string result = "";

	for(int i=0; i < (int)ip.size(); i++)

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


	if(n < 1 || n > 31) return "";

	


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

	
	for(int i=0; i<(int)str.size(); i++)

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




std::string toBit(int n)


{


	if(n < 0) return "";
	std::string str = "";
	do
	{
		str = (char)(n%2 + 48) + str;
		n /= 2;
	}while(n != 0);
	
	for(int i = 8 - str.size(); i > 0; i--)
		str = "0" + str;
	
	return str;


}




bool checkIp(std::string& ip, std::string& mask)


{


	//divisione in bloccchi dell'ip e controllo size

	std::vector<std::string> tokens = split(ip, '.');

	
	//controllo size
	if(tokens.size() != 4) return false;

	
	//controllo cdir
	std::vector<std::string> cdir = split(tokens[3], '/');
	tokens[3] = cdir[0];
	if(tokens.size() > 1) mask = getMask(toInt(cdir[1]));
	if(mask == "") return false;
	
	//controllo blocchi

	for(std::string token : tokens)

	{

		int block = toInt(token);

		if(block < 0 || block > 255)
			return false;

	}

	
	ip = "";
	for(std::string token : tokens)
	{
		ip += toBit(toInt(token)) + ".";
	}
	ip.erase(ip.size()-1);
	
	return true;


}





bool checkMask(std::string& mask)


{


	

std::vector<std::string> tokens = split(mask, '.');

	if(tokens.size() != 4)

 return false;
	
	std::string str = "";
	for(std::string token : tokens)
		str += toBit(toInt(token));
	
	if((int)str.size() != 32) return false;
	
	char current = '1';
	for(char ch : str)
	{
		if(ch != '1' && current == '1') current = '0';
		if(current == '0' && ch != current) return false;
	}
	
	mask = "";
	for(int i=0; i < (int)str.size(); i++)
	{
		mask += str.substr(i, 8) + ".";
		i += 7;
	}
	mask.erase(mask.size()-1);
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
