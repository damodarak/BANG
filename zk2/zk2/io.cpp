//class A
//{
//public:
//    A() : outStream(&cout) {};
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