#include <iostream>
#include <string>
#include <cstring>

using namespace std;

void test1()
{
  string s1;
  string s2("lic hermionex");

  string s3(s2);

  string s4(100, 'x');

  for(auto& e : s4)
  {
    cout<< e << " " <<endl;
  }

}



class mystring
{
public:
  mystring(const char* str = "")
    :_str(new char[strlen(str) + 1] )
    ,_size(strlen(str))
    ,_capacity(strlen(str) + 1)
    {

    }









private:
  char* _str;
  size_t _size;
  size_t _capacity;
};



int main()
{

//  test1();
  string  s;
  cout<< s.size() <<endl;






















  return 0;
}
