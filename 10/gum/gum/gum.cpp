#include <iostream>
#include <vector>
#include <string>

using namespace std;
template<typename T> class Pole {
public:
    class iterator {
    public:
        iterator() : vec(0), index(0) {}
        iterator(const iterator& it) : vec(it.vec), index(it.index) {}
        iterator(Pole<T>& pole, size_t i) : vec(&pole), index(i) {}

        T& operator* () {
            return (*vec)[index];
        }

        bool operator != (const iterator& it) {
            return index != it.index;
        }

        iterator operator++ () {
            index++;
            return *this;
        }

        iterator operator++ (int) {
            index++;
            return *this;
        }

        iterator operator-- () {
            index--;
            return *this;
        }

        iterator operator-- (int) {
            index--;
            return *this;
        }

        bool operator < (const iterator& it) {
            return index < it.index;
        }

    private:
        Pole<T>* vec;
        size_t index;
    };

    iterator begin() {
        return iterator(*this, 0);
    }
    iterator end() {
        return iterator(*this, chunk_size * vector_container_size + no_elements_in_container);
    }

    iterator from_back_begin() {
        return iterator(*this, chunk_size * vector_container_size + no_elements_in_container - 1);
    }
    iterator from_back_end() {
        return iterator(*this, 0);
    }

    Pole<T>(size_t chunk = 5) : chunk_size(chunk), vector_container_size(0), no_elements_in_container(0) {
        vec.push_back(make_unique<T[]>(chunk_size));
    }

    void push_back(const T& x) {
        if (chunk_size == no_elements_in_container) {
            resize(chunk_size);
        }
        vec[vector_container_size][no_elements_in_container] = x;
        no_elements_in_container++;
    }

    T& operator[](size_t i) {
        return vec[i / chunk_size][i % chunk_size];
    }

    T& at(size_t i) {
        if (!check(i)) {
            return vec[i / chunk_size][i % chunk_size];
        }
    }
private:
    size_t chunk_size;
    size_t vector_container_size;
    size_t no_elements_in_container;
    vector<unique_ptr<T[]>> vec;

    bool check(size_t i) {
        //implementing using exception handling maybe better
        if (i > ((vector_container_size)*chunk_size + no_elements_in_container)) {
            cout << "EEEEEEEEE" << endl;
            return true;
        }
        return false;
    }

    void resize(size_t i) {
        vec.push_back(make_unique<T[]>(i));
        vector_container_size++;
        no_elements_in_container = 0;
    }
};

int main()
{
    Pole<int> p;
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);
    p.push_back(5);
    p.push_back(6);
    cout << p[1] << endl;
    //cout << p[12] << endl;
    cout << p.at(1) << endl;
    cout << p.at(11) << endl;
    Pole<int>::iterator it;
    for (it = p.begin(); it < p.end(); it++) {
        cout << *it << endl;
    }
    for (it = p.from_back_begin(); it != p.from_back_end(); it--) {
        cout << *it << endl;
    }
}