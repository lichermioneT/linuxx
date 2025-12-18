#include <iostream>
using namespace std;

class person
{
public:
    person(int age, int height):_age(age),_height(height)
    {}

    void print()
    {
        cout<< "age " << _age <<endl;
        cout<< "height " << _height <<endl;
    }
private:
    int _age;
    int _height;
};



int main()
{

    person p1(111,444);
    p1.print();









    return  0;
}