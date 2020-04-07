#include<iostream>
#include<algorithm>
using namespace std;

int broj[]={1,2,3,4,5,6,7,8,9,10};

int main(){

  for(int i = 0; i < 10; i++){
    broj[i] = *max_element(broj, broj+10)+1;
    cout << broj[i] << endl;
  }
  //int bumber = *max_element(broj, broj+10);
  //cout << bumber << endl;
  return 0;
  }
