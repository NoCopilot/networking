#include "Ip.hpp"

int main()
{
	Ip ip;
	if (!ip.setUp("192.168.10.10/24"))
	{
		std::cout << "fail\n";
		goto end;
	}
	ip.load({ 240, 199, 50, 32 });
	ip.subnet(1); //1 -> vlsm | 0 -> fissa

	end:{
		std::cout << "end";
		return std::getchar();
	}
}