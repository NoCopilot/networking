#ifndef IP_HPP
#define IP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

void sort(std::vector<int>& v, bool b = false);

class Ip
{
public:
	bool loadIp(std::string IP)
	{
		ip = IP;
		mask = "";
		cdir = 0;
		return checkIp();
	}

	bool loadMask(std::string MASK)
	{
		mask = MASK;
		cdir = 0;
		return checkMask();
	}

	bool loadHosts(std::vector<int> v)
	{
		hosts.clear();
		hosts.reserve(v.size());
		for (int n : v) hosts.push_back(n + 2);
		return true;
	}

	void subnet(bool type = 0)
	{
		if (((ipClass + 1) * 8) < cdir) return;
		//fixed -> type = 0
		//vlsm  -> type = 1
		int hostbit = 0, subnetbit;
		std::string netid = ip.substr(0, cdir);
		if (!type)
		{
			for (int n : hosts) if (n > hostbit) hostbit = n;
			hostbit = getPowOf2(hostbit);
			subnetbit = 32 - cdir - hostbit;
			if (subnetbit < 1) return;
		}
		
		sort(hosts);
		for (int i = (int)hosts.size() - 1; i >= 0 ; i--)
		{
			if (type) //if vlsm, always find the subnet and host bits for every net
			{
				hostbit = getPowOf2(hosts[i]);
				subnetbit = 32 - cdir - hostbit;
			}
			if (hostbit < 1) return;
			if (subnetbit >= (32 - cdir) || subnetbit < 1) return;
			//print net address
			std::cout << "rete " << (char)('A' + (hosts.size() - i - 1)) << ": " << bitTOdec(ip) + " - ";
			
			//print broadcast address
			std::string str = ip.substr(cdir + subnetbit);
			for (int j = 0; j < str.size(); j++) str[j] = '1';
			std::cout << bitTOdec(ip.substr(0, cdir + subnetbit) + str) << "\n";

			//next net
			str = ip.substr(cdir, subnetbit);
			bool check = false;
			for (char ch : str)
			{
				if (ch != '1')
				{
					check = true; break;
				}
			}
			if (!check) return;
			add(str, 1);
			ip = ip.substr(0, cdir) + str + ip.substr(cdir + str.size());
		}
	}

	inline std::string getIp(){return ip;}
	inline std::string getMask(){return mask;}
	inline int getCdir(){return cdir;}
	inline int getIpClass(){return ipClass;}
private:
	std::string ip = "", mask = "";
	int cdir = 0, ipClass = 0;
	std::vector<int> hosts;

	bool checkIp()
	{
		//divisione in bloccchi dell'ip e controllo size
		std::vector<std::string> tokens = split(ip, '.');

		//controllo size
		if (tokens.size() != 4) return false;

		//controllo cdir
		std::vector<std::string> v = split(tokens[3], '/');
		tokens[3] = v[0];
		if (v.size() > 1) {
			cdir = toInt(v[1]);
			mask = getMask(cdir);
			//cdir invalido
			if (mask == "") return false;
		}

		//controllo blocchi
		for (std::string token : tokens)
		{
			int block = toInt(token);

			if (block < 0 || block > 255)
				return false;
		}

		ip = "";
		for (std::string token : tokens)
		{
			ip += toBit(toInt(token), 8);
		}

		getClass();
		return bitAnd();
	}

	bool checkMask()
	{
		std::vector<std::string> tokens = split(mask, '.');
		if (tokens.size() != 4) return false;

		std::string str = "";
		for (std::string token : tokens)
			str += toBit(toInt(token), 8);

		if ((int)str.size() != 32) return false;

		char current = '1';
		cdir = 0;
		for (char ch : str)
		{
			if (ch != '1' && current == '1') current = '0';
			if (current == '0' && ch != current) return false;
			if (current == '1') cdir++;
		}

		mask = str;
		return true;
	}

	bool bitAnd()
	{
		if (ip.size() != mask.size()) return false;
		std::string result = "";

		for (int i = 0; i < (int)ip.size(); i++)
			result += (ip[i] != mask[i] ? '0' : ip[i]);
		
		ip = result;
		return true;
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

	void getClass()
	{
		int n = 0;
		while (ip[n] == '1') n++;

		ipClass = n;
	}

	int toInt(std::string str)
	{
		int n = -1;
		std::stringstream ss;
		ss << str;
		ss >> n;
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

	std::string getMask(int n)
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

	inline void add(std::string& str, int i)
	{
		str = toBit(toInt(bitTOdec(str)) + i, (int)str.size());
	}
};
#endif