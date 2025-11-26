#include <iostream>
#include <cstdio>
using namespace std;

int main() {
#ifdef DEBUG
    cout << " 输出debug" << endl;
    cout << "xxxx" << endl;
#endif
    return 0;
}