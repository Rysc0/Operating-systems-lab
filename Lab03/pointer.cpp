#include <iostream>

using namespace std;


int main(){

    int a = 10;
    int *ptr;
    int b = *ptr;
    cout << &a << "\t" << a << endl;
    cout << b << endl;
    cout << ptr << endl;
    

    return 0;
}