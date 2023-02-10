//#include <fstream>
//#include <string>
//#include <iostream>
//
//using namespace std;
//
//class A
//{
//public:
//    A() : outStream(&cout) {};
//    ~A()
//    {
//        if (outFile.is_open())
//        {
//            outFile.close();
//        }
//    }
//    A(const string& fname)
//    {
//        outFile.open(fname);
//        outStream = &outFile;
//    };
//    void c()
//    {
//        *outStream << "ahoj";
//    }
//private:
//    ofstream outFile;
//    ostream* outStream;
//};
//
//class B
//{
//public:
//    B() : inStream(&cin) {};
//    ~B()
//    {
//        if (inFile.is_open())
//        {
//            inFile.close();
//        }    
//    }
//    B(const string& fname)
//    {
//        inFile.open(fname);
//        inStream = &inFile;
//    };
//    void c()
//    {
//        *inStream >> line;
//        cout << line << "konec";
//    }
//private:
//    ifstream inFile;
//    istream* inStream;
//    string line;
//};