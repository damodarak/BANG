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
using Dvojka = std::pair<std::string, std::string>;

//class Edge {
//public:
//	Edge(int capacity, const std::string& first, const std::string& second)
//	{
//		cap = capacity;
//		v1 = first;
//		v2 = second;
//	};
//	void fill_sousedi(std::map<std::string, Vzdal>& graph)
//	{
//		auto it = graph.find(v2);
//		for (auto&& i = it->second.begin(); i != it->second.end(); i++) 
//		{
//			Edge e("a", i->first, i->second);
//			//sousedi.push_back(&ob);
//		}
//	}
//	bool operator<(const Edge& ed) const
//	{
//		return cap > ed.cap;//min heap solution
//	}
//	void modify(int newCap)
//	{
//		cap = newCap;
//	}
//	int getC()
//	{
//		return cap;
//	}
//
//	std::string v1;
//	std::string v2;
//	std::vector<Edge*> sousedi;
//private:
//	int cap;
//};

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
	//std::priority_queue<Edge> initHeap(const std::string& start);
	/*Dvojka first()
	{
		auto it = findPairs.begin();
		Dvojka dv = Dvojka(it->first, it->second);
		findPairs.erase(it);
		return dv;
	}*/
	/*int size()
	{
		return findPairs.size();
	}*/
	void print(const std::string& line)
	{
		outFile << line << std::endl;
	}
private:
	std::map<std::string, Vzdal> graph;
	//std::vector<std::pair<std::string, std::string>> findPairs;

	std::ofstream outFile;
	std::ifstream graphData;
	std::ifstream pairs;
};


#endif