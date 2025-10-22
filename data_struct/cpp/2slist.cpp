#include <iostream>
using namespace std;

typedef int NodeDatatype;
struct Node 
{
  NodeDatatype _data;
  Node* _next;
  Node(NodeDatatype x):_data(x), _next(nullptr){}
};

class Slist 
{
public:
  Slist():_head(nullptr){}
  
  void pushback(NodeDatatype x)
  {
    if(_head == nullptr)
    {
      _head = new Node(x);
    }
    else 
    {
      Node* cur = _head;
      while(cur->_next != nullptr)
      {
        cur = cur->_next;
      }
      
      cur->_next = new Node(x);
    }
  }

  void print()
  {
    Node* cur = _head;
    while(cur != nullptr)
    {
      cout<< cur->_data << " ";
      cur = cur->_next;
    }
    cout<<endl;
  }




private: 
  Node* _head;
};



int main()
{

  Slist s;
  int arr[] = {1,3,4,5,6,7,8,23,4324,445,56,7,6,76,676};
  for(auto& e : arr)
  {
    s.pushback(e);
  }
  s.print();











  return 0;
}
