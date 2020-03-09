#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

void handler(int sig){

  sigignore(sig);
  if((sig == SIGINT)&&(sig < SIGQUIT)&&(sig < SIGTSTP)){
    sigrelse(sig);
    for(int i=1;i<=5;i++){
      cout << "SIGINT recieved\t" << i << "/5" << endl;
      if(sig < SIGINT){
        sigignore(sig);
        cout << "Manji prioritet" << endl;
      }
      sleep(1);
    }
    cout << "SIGINT finished\n" << endl;
  }
  else if ((sig == SIGTSTP)&&(sig > SIGINT)&&(sig > SIGQUIT)){
    sigrelse(sig);
    for(int i=1;i<=5;i++){
      cout << "SIGTSTP recieved\t" << i << "/5" << endl;
      sleep(1);
    }
  }
}

int main(){

  sigset(SIGINT, handler);
  sigset(SIGQUIT, handler);
  sigset(SIGTSTP, handler);

  for(int i=1;i<=10;i++){
    cout << "Main program\t" << i << "/10" << endl;
    sleep(1);
  }
  cout << "\nMain program finished" << endl;
  return 0;
  }
