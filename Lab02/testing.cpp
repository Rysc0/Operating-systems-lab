#include<iostream>
#include<unistd.h>
#include<ctime>
#include<cstdlib>
using namespace std;
bool mozes = false;
int main(){

  srand(time(NULL));
  int a[5];
  for(int i = 0; i < 5; i++){
    while(!mozes) {
      sleep(1);
      a[i] = rand()%1000;
      cout << "Random broj je: " << a[i] << endl;
      mozes = true;
    }


    while(mozes){
      cout << "2. funkcija\n";
      mozes = false;
    }
  }
  return 0;
  }
