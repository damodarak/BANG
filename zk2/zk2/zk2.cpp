#include "zk2.h"

using namespace std;

void Graph::handle_args(const vector<string>& vec)
{
	graphData.open(vec[1]);
	pairs.open(vec[2]);
	outFile.open(vec[3]);
	outFile << "node_1,node_2,distance" << endl;
}

void Graph::process_data()
{
	string line = "";

	int node1 = -1;
	int node2 = -1;

	getline(graphData, line);//first line

	while (getline(graphData, line))
	{
		int size = line.size();
		for (int i = 0; i < size; i++)
		{
			if (line[i] == ',')
			{
				if (node1 == -1)
				{
					node1 = i;
				}
				else 
				{
					node2 = i;
					break;
				}
			}		
		}
		string n1 = line.substr(0, node1);
		string n2 = line.substr(node1 + 1, node2 - node1 - 1);
		string dis = line.substr(node2 + 1);

		if (graph.find(n1) == graph.end())
		{
			Vzdal novy;
			novy.insert(pair<string, int>(n2, stoi(dis)));
			graph.insert(pair<string, Vzdal>(n1, novy));
		}
		else 
		{
			auto it = graph.find(n1);
			it->second.insert(pair<string, int>(n2, stoi(dis)));
		}

		node1 = node2 = -1;
	}
}

string Graph::handle_pair()
{
	string s;

	if (!getline(pairs, s))
	{
		return "";
	}
	if (s == "node_1,node_2")
	{
		getline(pairs, s);
	}

	int size = s.size();
	for (int i = 0; i < size; i++)
	{
		if (s[i] == ',')
		{
			string first = s.substr(0, i);
			string second = s.substr(i + 1);
			int res = find_shortest(0, 0, INT_MAX, first, second, "none");
			string line = first + "," + second + ",";
			if (res == INT_MAX)
			{
				return line;
			}
			else
			{
				return line + to_string(res);
			}
		}
	}
	return "";//for recodex
}

int Graph::find_shortest(int cena, int visited, int best, const string& start, const string& end, const string& before)
{
	int visitedMax = graph.size();

	if (start == end)
	{
		return cena;
	}
	else if (visited == visitedMax || cena >= best)
	{
		return INT_MAX;
	}
	else
	{
		auto edges = graph.find(start);
		if (edges == graph.end())
		{
			return INT_MAX;
		}
		else
		{
			for (auto&& it = edges->second.begin(); it != edges->second.end(); it++)
			{
				if (before == it->first)
				{
					continue;
				}
				int res = find_shortest(cena + it->second, visited + 1, best,it->first, end, start);
				 
				if (res < best)
				{
					best = res;
				}
			}
			return best;
		}
	}
}