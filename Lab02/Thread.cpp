/*Napisati program koji stvara dvije nove dretve. Prva dretva generira N slučajnih brojeva (N je ulazni argument
programa kod njegovog pokretanja, maksimalna vrijednost neka bude ograničena na 10), ispisuje brojeve i sprema ih
u zajedničku globalnu memoriju. Druga dretva čita brojeve iz zajedničke globalne memorije, računa njihov zbroj i
ispisuje izračunan zbroj. Ponoviti navedene korake još M puta pri čemu je M drugi ulazni argument programa kod
njegovog pokretanja.

Napomene:

Budući da obje dretve pristupaju istoj zajedničkoj memoriji potrebno je osigurati da se istoj pristupa naizmjenice.
U tu svrhu se može iskoristiti dodatna varijabla u zajedničkoj memoriji čiju vrijednost obje dretve ispituju. Ako
dretva nema pravo pristupiti zajedničkoj memoriji tada MOŽE BITI U RADNOM ČEKANJU.
*/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include<iostream>
using namespace std;

int global_variable[5];
int times = 5;



void *random_number(void *arg){
  for(int i = 0; i <= 5; i++){
    int number = rand()%5;
    global_variable[i] = number;
    cout << "Random number is: " << number << endl;
    cout << global_variable[i] << endl;
  }
}

void *how_many_times(void *arg){
    for(int j = 0; j <= times; j++){
      int zbroj = 0;
      for(int i = 0; i <= 5; i++){
        zbroj = zbroj + global_variable[i];
      }
      cout << "Zbroj je: " << zbroj << endl;
    }
}


int main(){

  srand(time(NULL));

  pthread_t pid;
  pthread_t pid1;
  int retval;
  int retval1;


  retval = pthread_create(&pid,NULL,random_number,NULL);
  retval1 = pthread_create(&pid1,NULL,how_many_times,NULL);

  if(retval < 0){
    cout << "Error" << endl;
    exit(1);
  }

  if(retval1 < 0){
    cout << "Error" << endl;
    exit(1);
  }

  pthread_join(pid,NULL);
  pthread_join(pid1,NULL);

  cout << "Ispis polja\n";
  for(int i=0;i<=5;i++){
    cout << global_variable[i] << endl;
  }
  return 0;
}
