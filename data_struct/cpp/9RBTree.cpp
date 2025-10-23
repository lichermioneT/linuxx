#include <iostream>
using namespace std;

enum Colour
{
  BLACK,
  RED
};

template<class K, class V>
struct RBTreeNode
{
  RBTreeNode<K,V>*  _left;
  RBTreeNode<K,V>* _right;
  RBTreeNode<K,V>* _parent;
  Colour _col;
  pair<K,V> _kv;
  RBTreeNode(const pair<K,V>& kv)
    :_left(nullptr)
    ,_right(nullptr)
    ,_parent(nullptr)
    ,_col(RED)
    ,_kv(kv)
  {}
};


template<class K, class V>
class RBTree 
{
  typedef RBTreeNode<K,V> Node;
public:
  bool insert(const pair<K,V>& kv)
  {
    if(_root == nullptr)
    {
      _root = new Node(kv);
      _root->_col = BLACK; // 根节点是黑色的
      return true;
    }

    Node* parent = nullptr;
    Node* cur = _root;

    while(cur != nullptr)
    {
      if(cur->_kv.first < kv.first)
      {
        parent = cur;
        cur = cur->_right;
      }
      else if(cur->_kv.first > kv.first)
      {
        parent = cur;
        cur = cur->_left;
      }
      else 
      {
        return false;
      }
    }

    cur = new Node(kv);
    if(parent->_kv.first < kv.first)
    {
      parent->_right = cur;
      cur->_parent = parent;
    }
    else 
    {
      parent->_left = cur;
      cur->_parent = parent;
    }

    
    cur->_col = RED; // 新节点只能是红色的,因为黑的破坏平衡了（黑色数量相同）
   // 关键看叔叔
   // 
   // 1.parent是红的，一定不是根节点 
   // cur红 parent红 叔叔存在且是红色的
   // p和u变成黑色，g变成红。没有连续的红节点了，且子树中每条路径的黑节点数量不变。
   //   1.g是根节点了。变黑就行了
   //   2.不是跟呢？g可能是红色的
   //     g的父亲是黑色，没有影响
   //     g的父亲是红色，继续向上调整
   //
   // cur存在且为红，p和u变黑，g变红，继续往上处理
   // ps:这里只关注颜色，p,g,u,几个节点在左右没有影响的
   //
   //
   //
   // 2.cur红色，p红色，g黑色的，p不存在的
   //   cur就是新增的
   // 2.cur红色，p红色，g黑色的，p存在且是黑色的
   //   cur不是新增的，cur原来是黑色的，这里是cur变红，由第一种情况变来的
   //   旋转+变色
   //   左单旋转：g红 p黑
   //   右单旋转：
   //
   // 3.双旋转
   //   cur红色的，parent红色的，g是黑色的 u不存在，或者为黑的
   //
   //
   //
   //
  
  while(parent != nullptr && parent->_col == RED)
  {
    // 红黑树关键看叔叔
    
    Node* grandfather = parent->_parent;

    if(grandfather->_left == parent)
    {
      Node* uncle = grandfather->_right;
      // 叔叔存在且颜色为红色的
      if(uncle != nullptr && uncle->_col == RED)
      {
        parent->_col = BLACK;
        uncle->_col = BLACK;
        grandfather->_col = RED;

        // 继续往上处理
        cur = grandfather;
        parent = cur->_parent;
      }
      else  // 叔叔不存在，或者叔叔存在且为黑色
      {
        // 双旋---》变成单旋转
        if(cur == parent->_right)
        {
          RotateL2(parent);
          swap(parent, cur);
        }
        
        // 第二种情况
        //
        RotateR2(grandfather);
        grandfather->_col = RED;
        parent->_col = BLACK;
        break;
      }
    }
    else 
    {
      Node* uncle = grandfather->_left;
      if(grandfather->_right == parent)
      {
        if(uncle != nullptr  && uncle->_col == RED)
        {
          parent->_col = BLACK;
          uncle->_col = BLACK;
          grandfather->_col = RED;
          
          // 继续向上处理
          cur = grandfather;
          parent = cur->_parent;
        }
        else  // 叔叔不存在，存在且为黑色
        {
          if(cur == parent->_left)
          {
             RotateR2(parent);
             swap(parent, cur);
          }

          RotateL2(grandfather);
          grandfather->_col = RED;
          parent->_col = BLACK;
          break;
        }
      }
    }
  }
    _root->_col = BLACK;
    return true;
  }

  void Print() 
  {
    InOrder(_root);
    cout<<endl;
  } 


void PrintTree(Node* root, int depth = 0)
{
      if (root == nullptr)
                return;

      PrintTree(root->_right, depth + 1);
        for (int i = 0; i < depth; ++i)
              printf("    ");  // 缩进

       printf("%d(%s)\n", root->_data, root->_col == RED ? "R" : "B");
       PrintTree(root->_left, depth + 1);
}



private: 
void InOrder(Node* root)
  {
    if(root == nullptr) return;
    InOrder(root->_left);
    cout << root->_kv.first << "(" << (root->_col == RED ? "R" : "B") << ") ";
    InOrder(root->_right);
  }

  Node* _root = nullptr;
  void RotateL(Node* x)
  {
    Node* y = x->_right;
    if (y == nullptr)
      return;

    x->_right = y->_left;
    if (y->_left != nullptr)
    {
      y->_left->_parent = x;
    }

    y->_parent = x->_parent;
    if (x->_parent != nullptr)
    {
      if (x == x->_parent->_left)
      {
        x->_parent->_left = y;
      }
      else
      {
        x->_parent->_right = y;
      }
    }
    else
    {
      _root = y;
    }

    y->_left = x;
    x->_parent = y;
  }

  void RotateR(Node* y)
  {
    Node* x = y->_left;
    if (x == nullptr)
      return;

    y->_left = x->_right;
    if (x->_right != nullptr)
    {
      x->_right->_parent = y;
    }

    x->_parent = y->_parent;
    if (y->_parent != nullptr)
    {
      if (y == y->_parent->_left)
      {
        y->_parent->_left = x;
      }
      else
      {
        y->_parent->_right = x;
      }
    }
    else
    {
      _root = x;
    }

    x->_right = y;
    y->_parent = x;
  }

  void RotateR2(Node* grandfather)
  {
      Node* parent = grandfather->_left;
      Node* curR = parent->_right;
      Node* ppNode = grandfather->_parent;
       
      // 旋转
      grandfather->_left = parent->_right;
      if(parent->_right != nullptr)
      {
        curR->_parent = grandfather;; 
      }

      parent->_right = grandfather;
      grandfather->_parent = parent;

      parent->_parent = ppNode;
      if(ppNode != nullptr)
      {
        if(ppNode == grandfather->_left)
        {
          ppNode->_left = parent;
        }
        else 
        {
          ppNode->_right = parent;
        }
      }
      else 
      {
        _root = parent;
        _root->_parent = nullptr;
      }
  }

  void RotateL2(Node* grandfather)
  {
    Node* parent = grandfather->_right;
    Node* curL = parent->_left;
    Node* ppNode = grandfather->_parent;

    grandfather->_right = curL;
    if(curL != nullptr)
    {
      curL->_parent = grandfather; 
    }

    parent->_left = grandfather;
    grandfather->_parent = parent;

    parent->_parent = ppNode;

    if(ppNode != nullptr)
    {
      if(ppNode->_left == grandfather)
      {
        ppNode->_left = parent;
      }
      else 
      {
        ppNode->_right = parent;
      }
    }
    else 
    {
      _root = parent;
      _root->_parent = nullptr; 
    }
  }


};



int main()
{




  RBTree<int, int> tree;

  // 测试插入顺序：1,2,3,4,5,6,7 会触发旋转
  tree.insert({1, 1});
  tree.Print();  // 输出: 1(B) 

  tree.insert({2, 2});
  tree.Print();  // 输出: 1(B) 2(R) 

  tree.insert({3, 3});
  tree.Print();  // 触发调整: 2(B) 1(R) 3(R) 

  tree.insert({4, 4});
  tree.Print();  // 触发旋转: 2(B) 1(R) 3(B) 4(R) 

  tree.insert({5, 5});
  tree.Print();  // 触发调整: 3(B) 2(B) 1(R) 4(R) 5(R)  等待，实际应为平衡树

  tree.insert({6, 6});
  tree.Print();

  tree.insert({7, 7});
  tree.Print();

  // 测试重复插入
  bool res = tree.insert({4, 4});
  cout << "Insert duplicate 4: " << (res ? "true" : "false") << endl;

  // 测试更多节点以观察平衡
  tree.insert({0, 0});
  tree.Print();












  return 0;
}
