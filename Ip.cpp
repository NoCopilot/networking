#include "Ip.hpp"

void sort(std::vector<int>& v, bool b)
{
	std::vector<int> sorted;
	for (int i = 0; i < v.size(); i++)
	{
		int j;
		for (j = 0; j < sorted.size(); j++)
		{
			if(b)
			{
				if (v[i] > sorted[j])
					break;
			}
			else 
			{
				if (v[i] < sorted[j])
					break;
			}
		}
		sorted.insert(sorted.begin() + j, v[i]);
	}
	v = sorted;
}