#ifndef ZK2_H
#define ZK2_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <climits>

using Vzdal = std::map<std::string, int>;

class Graph 
{
public:
	Graph() {};
	~Graph()
	{
		if (outFile.is_open())
		{
			outFile.close();
		}
		if (graphData.is_open())
		{
			graphData.close();
		}
		if (pairs.is_open())
		{
			pairs.close();
		}
	}
	void handle_args(const std::vector<std::string>& vec);
	void process_data();
	std::string handle_pair();
	int find_shortest(int cena, int visited, int best, const std::string& start, const std::string& end, const std::string& before);
	void print(const std::string& line)
	{
		outFile << line << std::endl;
	}
private:
	std::map<std::string, Vzdal> graph;

	std::ofstream outFile;
	std::ifstream graphData;
	std::ifstream pairs;
};


#endif