#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

void obrada_prekida(int j){
  cout << "Start --- obrada prekida" << endl;
  for(int i=1;i<=10;i++){
    cout << "Prekid " << i << "/10" << endl;
    sleep(1);
  }
  sleep(j);
  cout << "End --- obrada gotova" << endl;
  cout << j << endl;
}

void prekidna_rutina(int sig){
  int i;

  cout << "Prekidna rutina aktivirana!" << endl;
  switch (sig) {
    case SIGINT:
        i = 1;
        obrada_prekida(i);
        break;
    case SIGQUIT:
      i = 2;
      obrada_prekida(i);
      break;
    case SIGTSTP:
      i = 3;
      obrada_prekida(i);
      break;
    default:
      return;
  }
  cout << "Razina prekida: " << i << endl;
}

int main(void){
  sigset(SIGINT, prekidna_rutina);
  sigset(SIGQUIT, prekidna_rutina);
  sigset(SIGTSTP, prekidna_rutina);

  cout << "Osnovni program --- START" << endl;
  for(int i=1;i<=20;i++){
    cout << i << "/20" << endl;
    sleep(1);
  }
  cout << "Osnovni program --- END" << endl;
  return 0;
  }
