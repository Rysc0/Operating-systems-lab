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
  else {
    sighold(sig);
    /*if(prioritet > trenutni_prioritet){
      sigignore(sig);
    }*/
  }
  // problem - sighold drzi samo jedan signal i omogucava prekid vise razine ako 2 puta dode niza razina dok
  // sigignore ignorira sve signale i nije moguce napraviti prekid manje razine nakon izvrsetka vise razine prekida
  //sigignore(sig);


  //prioritet = 0;

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

  cout << "Krajnji prioritet je " << prioritet << endl;
  /*do{
    cout << "unesi broj: ";
    cin >> n;
    funkcija(n);
  }while(n!=0);
*/

  return 0;
  }
