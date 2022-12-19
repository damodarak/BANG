#include <vector>
#include "polymorph.cpp"

int main(int argc, char* argv[]) {
    PolymorficList list;

    list.add(std::make_unique<IntValue>(50));
    list.add(std::make_unique<IntValue>(120));
    list.add(std::make_unique<StringValue>("Hello, there!"));
    list.print();

    PolymorficList l1 = list;


    ValuePointer vl = std::make_unique<IntValue>(1);
    //ValuePointer vp1 = std::make_unique<IntValue>(*vl);//error
    ValuePointer vp2 = vl->clone();

    return 0;
}