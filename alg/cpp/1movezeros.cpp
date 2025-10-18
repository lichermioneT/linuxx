#include <iostream>
#include <vector>
using namespace std;

int main()
{
  
  vector<int> v = {1,0,98,3,4,6,8,0,84,0,2,3,99};

  int i = 0;
  for(auto& e : v)
  {
    if(e != 0)
    {
      std::swap(v[i++], e);
    }
  }
  
 for(auto& e : v)
 {
   cout<< e << " ";
 }
 cout<<endl;







  return 0;
}
