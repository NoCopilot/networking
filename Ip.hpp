#ifndef IP_HPP
#define IP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int toInt(std::string);
int getPowOf2(int);
int power(int, int);
int bitToInt(std::string);
std::string toString(int);
std::string toBit(int, int = -1);
std::string getmask(int);
std::string bitTOdec(std::string);
std::vector<std::string> split(std::string, char);
std::vector<std::string> split(std::string, int);

struct Network
{
    std::string name, networkAddress, broadcastAddress;
	std::vector<std::string> gatewayAdresses, gatewayNames;
	int cdir;
};

struct Subnet
{
    std::string name;
    int hosts;
    std::vector<std::string> gateways;
};

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

	bool loadHosts(std::vector<int> v, std::vector<std::string> str, std::vector<std::vector<std::string>> v1)
	{
		hosts.clear();
		names = str;
		hosts = v;
		links = v1;
		return true;
	}

	void reset()
	{
		ip = "";
		mask = "";
		ipClass = -1;
		clearHosts();
	}

    std::vector<Network> subnet(std::vector<Subnet> v, bool vlsm)
    {
        std::vector<Network> res;

        if(v.size() == 0) return res;

        //sort
        for(size_t i = 0; i < v.size(); i++)
        {
            for(size_t j = i+1; j < v.size(); j++)
            {
                if(v[i].hosts < v[j].hosts)
                {
                    int temp = v[i].hosts;
                    v[i].hosts = v[j].hosts;
                    v[j].hosts = temp;
                }
            }
        }

        std::string net_id = ip.substr(0, cdir);
        std::string subnet_id;
        int host_bits = 0, subnet_bits, temp_n;
        if(!vlsm)
        {
            //calculating if network can be subnetted
            host_bits = getPowOf2(v[0].hosts + v[0].gateways.size() + 2);
            subnet_bits = 32 - cdir - host_bits;
            if(subnet_bits < 0) return res;
            if(subnet_bits == 0 && v.size() > 1) return res;
            if(v.size() > power(2, subnet_bits)) return res;
            subnet_id = ip.substr(cdir, subnet_bits);
        }
        for(size_t i = 0; i < v.size(); i++)
        {
            if(vlsm)
            {
                //calculating next network
                host_bits = getPowOf2(v[i].hosts + v[i].gateways.size() + 2);
                subnet_bits = 32 - cdir - host_bits;

                //check
                if(subnet_bits < 0) return res;
                if(subnet_bits == 0 && v.size() > 1) return res;

                subnet_id = ip.substr(cdir, subnet_bits);
            }

            temp_n = power(2, host_bits)-1;

            Network network;
            network.networkAddress = bitTOdec(net_id + subnet_id + toBit(0, host_bits));
            network.broadcastAddress = bitTOdec(net_id + subnet_id + toBit(temp_n));
            network.cdir = cdir + subnet_bits;
            network.name = v[i].name;
            for(size_t j = 0; j < v[i].gateways.size(); j++)
            {
                network.gatewayNames.insert(network.gatewayNames.begin(), v[i].gateways[j]);
                network.gatewayAdresses.insert(network.gatewayAdresses.begin(), bitTOdec(net_id + subnet_id + toBit(--temp_n)));
            }
            res.push_back(network);

            //next subnet
            subnet_id = toBit(bitToInt(subnet_id) + 1, subnet_bits);
            if(subnet_id == "") break;
            ip.replace(cdir, subnet_id.size(), subnet_id);
        }

        return res;
    }

    std::string getIp(){return ip;}
    std::string getMask(){return mask;}
    int getCdir(){return cdir;}
    int getIpClass(){return ipClass;}
    void clearHosts() { hosts.clear(); names.clear(); }
private:
	std::string ip = "", mask = "";
	int cdir = 0, ipClass = 0;
	std::vector<int> hosts;
	std::vector<std::string> names;
	std::vector<std::vector<std::string>> links;

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
        for (std::string& token : tokens)
		{
			int block = toInt(token);

            if (block < 0 || block > 255) return false;
		}

		ip = "";
        for (std::string& token : tokens)
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
		std::vector<std::vector<std::string>> sortedL;
		for (int i = 0; i < hosts.size(); i++)
		{
			int j;
			for (j = 0; j < sortedI.size(); j++)
				if (hosts[i] > sortedI[j]) break;
			
			sortedI.insert(sortedI.begin() + j, hosts[i]);
			sortedS.insert(sortedS.begin() + j, names[i]);
			sortedL.insert(sortedL.begin() + j, links[i]);
		}
		hosts = sortedI;
		names = sortedS;
		links = sortedL;
	}
};
#endif
