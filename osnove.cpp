#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

void funkcija(){

}

void prioritet(int sig, int priority){
  // setting priority
  if(sig > priority){
    priority = sig;

    cout << "Prioritet je: " << priority << endl;
    //return priority;
    for(int i=1;i<=5;i++){
      cout << i << "/5" << endl;
      sleep(1);

    }
    cout << endl;
  }
  else {
    sigignore(sig);
    cout << "Manji prioritet!" << endl;
  }
}

void prekid(int sig){
  volatile int priority = 0;
  prioritet(sig,priority);
  sigignore(sig);
/*  if(sig > prioritet(sig,priority)){
    cout << "Prekid" << endl;
    for(int i = 1;i <=10; i++){
      cout << i << "/10" << endl;
      sleep(1);
    }
    cout << endl;
  }
  else{
    sigignore(sig);
    cout << "Nema" << endl;
  }*/

}


int main(){

  sigset(SIGTSTP,prekid);
  sigset(SIGINT, prekid);
  sigset(SIGQUIT, prekid);

  cout << "Glavni program\n" << endl;
  for(int i=1;i<=20;i++){
    cout << i << "/20" << endl;

    sleep(1);

  }

  return 0;
  }
