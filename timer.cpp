#include<iostream>
#include<time.h>
using namespace std;

int main(){
  time_t timer;
  //timer = time(NULL);
  time(&timer);
  cout << "Current time is: " << ctime(&timer) << endl;
  return 0;
  }
