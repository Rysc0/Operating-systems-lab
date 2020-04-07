/*Ostvariti sustav paralelnih procesa/dretava. Struktura procesa/dretava dana je
sljedećim pseudokodom:*/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>        /*exit() */
#include <unistd.h>      /*fork()*/
#include <sys/wait.h>  /* wait()*/
using namespace std;


int id, procesa, zadnji = 0;
void *ptr_1, *ptr_2;

typedef struct zajednicke_varijable{
  int *array;
  int *broj;
}var;
var memorija;

void kriticni_odsjecak(int i){
  //int *ptr = procesa;
  int j;
  memorija.array[i] = 1;
  memorija.broj[i] = zadnji + 1;
  zadnji = memorija.broj[i];
  memorija.array[i] = 0;

  for(j = 0; j < procesa; j++){
    while(memorija.array[j] != 0){

    }
    while(memorija.broj[j] != 0 && (memorija.broj[j] < memorija.broj[i] || (memorija.broj[j] == memorija.broj[i]) && j < i)){

    }
  }

}


void izadji_iz_kriticnog_odsjecka(int i){
  memorija.broj[i] = 0;
}


void brisi(int sig){
  shmdt(memorija.array);
  shmdt(memorija.broj);
  shmctl(id,IPC_RMID,NULL);
  exit(1);
}


int main(int argc, char **argv) {

  procesa = atoi(argv[1]);

  if(procesa > 10){
      cout << "Limited process count 1-10\n";
      exit(-1);
    }

  int polje_trazim[procesa];
  memorija.array = polje_trazim;
  int polje_broj[procesa];
  memorija.broj = polje_broj;
  //ptr_1 = var.array;
  //ptr_2 = var.broj;

  id = shmget(IPC_PRIVATE,sizeof(zajednicke_varijable),0600);
  if(id == -1){
    cout << "ERROR, NO SHARED MEMORY!" << endl;
    exit(1);
  }
  cout << "SHMID_1 = " << id << endl;

  memorija.array = (int*) shmat(id,NULL,0);
  memorija.broj = (int*) shmat(id,NULL,0);
/*
  procesa
  zadnji      treba ih uključit u dijeljenu memoriju
*/


  sigset(SIGINT, brisi);//u slučaju prekida briši memoriju


  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    memorija.broj[i] = i;
    if (fork() == 0) {
      // child code
      for(int k = 1; k <=5; k++){
        kriticni_odsjecak(i);

        for(int m = 1; m <=5; m++){
          //ispisi(i,k,m);
          cout << "Proces " << i << " K.O. br: " << k << " (" << m << "/5)" << endl;
          sleep(1);
        }
        // izađi iz kritičnog odsječka
        izadji_iz_kriticnog_odsjecka(i);
      }
      exit(1);
    }
  }

  sleep(1);
  for(int i = 0; i < procesa; i++){
    //cout << "Waited on proccess " << i << endl;
    wait(NULL);
  }

  brisi(SIGINT);
  /*shmdt(array);
  shmdt(broj);
  shmctl(id[0],IPC_RMID,NULL);
  shmctl(id[1],IPC_RMID,NULL);
*/


  return 0;
}
