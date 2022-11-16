#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <vector>

using namespace std;

class Film {
public:
	Film(const string& _film, const string& _reziser, const int& _rok) : film(_film), reziser(_reziser), rok(_rok) {};

	string film;
	string reziser;
	int rok;

	bool operator<(const Film& fm) const
	{
		if (rok < fm.rok)
		{
			return true;
		}
		else if (fm.rok == rok)
		{
			return film < fm.film;
		}
		else
		{
			return false;
		}
	}
};

using Mnozina = set<Film>;

class Databaze {
public:
	void add(const string& film, const string& reziser, const int& rok)
	{
		Film f(film, reziser, rok);
		databaze.insert(Film (film, reziser, rok));
	}
	void list()
	{
		for (auto&& fl : databaze)
			cout << fl.film << " " << fl.reziser << " " << fl.rok << endl;
	}
private:
	Mnozina databaze;
};

int main(int argc, char** argv) {
	Databaze db;
	string film;
	string rez;
	string srok;
	int rok;
	for (;;)
	{
		cin >> film;
		cin >> rez;
		cin >> srok;
		if (cin.fail())
			break;
		rok = stoi(srok);
		db.add(film, rez, rok);
	}
	db.list();
}