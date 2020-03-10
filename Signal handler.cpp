#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

int trenutni_prioritet = 0;
int prekidi[4];

void funkcija(int sig){
  int prioritet;

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
    prekidi[prioritet]=trenutni_prioritet;
    trenutni_prioritet=prioritet;
    sigrelse(sig);
    for(int i=0;i<=5;i++){
      cout << "Prekid " << i << "/5" <<  "\trazina: " << prioritet << endl;
      sleep(1);
    }
    cout << "zavrsila obrada prekida" << prioritet << endl;
    sighold(sig);
    trenutni_prioritet=prekidi[prioritet];
  }

int main(){

  sigset(SIGINT, funkcija);
  sigset(SIGQUIT, funkcija);
  sigset(SIGTSTP, funkcija);

  cout << "Main program running" << endl << endl;

  for(int i=1;i<=20;i++){
    cout << "Main program " << i << "/20" << endl;
    sleep(1);
  }

  cout << "Krajnji prioritet je " << trenutni_prioritet << endl;
  
  return 0;
  }
