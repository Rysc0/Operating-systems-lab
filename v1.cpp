/*Napisati program koji omogućava obradu prekida s više razina/prioriteta,
bar triju, zadanih od korisnika, s tim da se prekidi koji čekaju na obradu
ne pamte, ali viša razina prioriteta prekida manju razinu. */

#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;

void obrada_prekida(int prioritet){
  cout << "OBRADA PREKIDA \t RAZINA PREKIDA = " << prioritet << "\n" << endl;
  for(int i=1;i<=5;i++){
    cout << "PREKID " << i << "/5" << endl;
    sleep(1);
  }
  cout << "PREKID GOTOV\t Razina prekida = " << prioritet << endl << endl;
  /* how to process the interrupt*/
}


void prekidna_rutina(int sig){
  // recognizing recieved signal
  int priority[] = {SIGINT,SIGQUIT,SIGTSTP};
  int tekuci_prioritet = -1;
  for(int i=0;i<3;i++){
    if(sig == priority[i]){
        if(i > tekuci_prioritet){
        priority[i] = tekuci_prioritet;
        tekuci_prioritet = i;
        sigrelse(sig);
        obrada_prekida(i)
      }
    }
  }
  /* make interrupt recognition and priority system */



int main(){

  sigset(SIGINT, prekidna_rutina);  // priority = 1
  sigset(SIGQUIT, prekidna_rutina); // priority = 2
  sigset(SIGTSTP, prekidna_rutina); // priority = 3

  cout << "PROGRAM START\n" << endl;

  for(int i=1;i<=10;i++){
    cout << "Glavni program " << i << "/10" << endl;
    sleep(1);
  }
  cout << "PROGRAM FINISHED\n" <<  endl;
  return 0;
  }
