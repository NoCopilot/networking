#ifndef IP_HPP
#define IP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int toInt(std::string);
int getPowOf2(int);
std::string toString(int);
std::string toBit(int, int);
std::string getmask(int);
std::string bitTOdec(std::string);
std::vector<std::string> split(std::string, char);
std::vector<std::string> split(std::string, int);

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

	bool loadHosts(std::vector<int> v, std::vector<std::string> str)
	{
		hosts.clear();
		names = str;
		hosts.reserve(v.size());
		for (int n : v) hosts.push_back(n + 2);
		return true;
	}

	void reset()
	{
		ip = "";
		mask = "";
		ipClass = -1;
		clearHosts();
	}

	std::vector<std::string> subnet(bool type = 0)
	{
		std::vector<std::string> result;
		//fixed -> type = 0
		//vlsm  -> type = 1
		int hostbit = 0, subnetbit;
		std::string netid = ip.substr(0, cdir);
		if (!type)
		{
			for (int n : hosts) if (n > hostbit) hostbit = n;
			hostbit = getPowOf2(hostbit);
			subnetbit = 32 - cdir - hostbit;
			if (subnetbit < 1) return result;
		} else sort();
		for (int i = (int)hosts.size() - 1; i >= 0 ; i--)
		{
			if (type) //if vlsm, always find the subnet and host bits for every net
			{
				hostbit = getPowOf2(hosts[i]);
				subnetbit = 32 - cdir - hostbit;
			}
			if (hostbit < 1)
			{
				result.clear();
				return result;
			}
			if (subnetbit >= (32 - cdir) || subnetbit < 1)
			{
				result.clear();
				return result;
			}
			//print net address
			result.push_back("rete " + names[hosts.size() - i - 1] + ": " + bitTOdec(ip) + " - ");
			
			//print broadcast address
			std::string str = ip.substr(cdir + static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(subnetbit));
			for (int j = 0; j < str.size(); j++) str[j] = '1';
			result[hosts.size() - i - 1] += bitTOdec(ip.substr(0, cdir + subnetbit) + str);
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
			if (!check)
			{
				result.clear();
				return result;
			}
			add(str, 1);
			ip = ip.substr(0, cdir) + str + ip.substr(cdir + str.size());
		}
		return result;
	}

	inline std::string getIp(){return ip;}
	inline std::string getMask(){return mask;}
	inline int getCdir(){return cdir;}
	inline int getIpClass(){return ipClass;}
	inline void clearHosts() { hosts.clear(); names.clear(); }
private:
	std::string ip = "", mask = "";
	int cdir = 0, ipClass = 0;
	std::vector<int> hosts;
	std::vector<std::string> names;

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
			mask = getmask(cdir);
			//cdir invalido
			if (mask == "") return false;
		}
		else cdir = 0;
		

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
		if (mask == "") return true;
		if (cdir < (ipClass * 8)) return false;
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

		if (ipClass * 8 < cdir) return false;

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

	void getClass()
	{
		int n = 0;
		while (ip[n] == '1') n++;

		ipClass = ++n;
	}

	inline void add(std::string& str, int i)
	{
		str = toBit(toInt(bitTOdec(str)) + i, (int)str.size());
	}

	void sort()
	{
		std::vector<int> sortedI;
		std::vector<std::string> sortedS;
		for (int i = 0; i < hosts.size(); i++)
		{
			int j;
			for (j = 0; j < sortedI.size(); j++)
			{
				if (hosts[i] < sortedI[j]) break;
			}
			sortedI.insert(sortedI.begin() + j, hosts[i]);
			sortedS.insert(sortedS.begin() + j, names[i]);
		}
		hosts = sortedI;
		names = sortedS;
	}
};
#endif