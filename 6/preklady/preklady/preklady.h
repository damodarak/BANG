#ifndef PREKLADY_H_
#define PREKLADY_H_

#include <set>
#include <iterator>
#include <string>
#include <map>

struct cmplen {
    bool operator()(const std::string& s1, const std::string& s2) const;
};
struct cmpnorm {
    bool operator()(const std::string& s1, const std::string& s2) const;
};

using Mnoz = std::set<std::string, cmplen>;
using Mapa = std::map<std::string, Mnoz, cmpnorm>;
using Dvojka = std::pair<Mapa::const_iterator, Mapa::const_iterator>;

class Preklady {
public:
	void add(const std::string& slovo, const std::string& preklad);
	void del(const std::string& slovo, const std::string& preklad);
	void del(const std::string& slovo);
    Mapa::const_iterator find(const std::string& slovo);
    Dvojka prefix(const std::string& pre);

    void print(const Mapa::const_iterator& it);
    void print_pre(const Dvojka& dv);
private:
	Mapa slova;
};

#endif