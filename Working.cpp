#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

int prioritet;

void funkcija(int sig){
  int trenutni_prioritet = prioritet;

  switch(sig){
    case (SIGINT):
      prioritet = 1;
      break;
    case (SIGQUIT):
      prioritet = 2;
      break;
    case (SIGTSTP):
      prioritet = 3;
      break;
  }
  cout << "Prioritet je " << prioritet << " a trenutni prioritet je " << trenutni_prioritet << endl;
  if(prioritet >= trenutni_prioritet){
    sigrelse(sig);
    for(int i=0;i<=5;i++){
      cout << "Prekid " << i << "/5" << endl;
      sleep(1);
    }
  }
  else sigignore(sig);
}


int main(){
  int n;

  sigset(SIGINT, funkcija);
  sigset(SIGQUIT, funkcija);
  sigset(SIGTSTP, funkcija);

  cout << "Main program running" << endl << endl;

  for(int i=1;i<=20;i++){
    cout << "Main program " << i << "/20" << endl;
    sleep(1);
  }
  /*do{
    cout << "unesi broj: ";
    cin >> n;
    funkcija(n);
  }while(n!=0);
*/
  return 0;
  }
