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

int *array, id, *broj, *procesi, *zadnji;

void kriticni_odsjecak(int i){
  int j;
  array[i] = 1;
  broj[i] = *zadnji + 1;
  *zadnji = broj[i];
  array[i] = 0;

  for(j = 0; j < *procesi; j++){
    while(array[j] != 0){

    }
    while(broj[j] != 0 && (broj[j] < broj[i] || (broj[j] == broj[i]) && j < i)){

    }
  }

}


void izadji_iz_kriticnog_odsjecka(int i){
  broj[i] = 0;
}

void funkcija(){
  printf("\x1B[31mNEXT PROCCESS...\033[0m\n");
}

void oslobodi_memoriju(int sig){
  shmdt(array);
  shmdt(broj);
  shmdt(procesi);
  shmdt(zadnji);
  shmctl(id,IPC_RMID,NULL);
  exit(0);
}

int main(int argc, char **argv) {
  int procesa;
  procesa = atoi(argv[1]);

  if(procesa > 10){
      cout << "Limited process count 1-10\n";
      exit(-1);
    }


  int polje_trazim[procesa];
  array = polje_trazim;
  int polje_broj[procesa];
  broj = polje_broj;
  procesi = &procesa;

  id = shmget(IPC_PRIVATE,sizeof(int)*procesa,0600);
  if(id == -1){
    cout << "ERROR, NO SHARED MEMORY!" << endl;
    exit(1);
  }


  array = (int*) shmat(id,NULL,0);
  broj = (int*) shmat(id,NULL,0);
  procesi = (int*) (shmat(id,NULL,0));
  zadnji = (int*) (shmat(id,NULL,0));
  *zadnji = 0;

  sigset(SIGINT, oslobodi_memoriju);  //u slučaju prekida briši memoriju


  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    broj[i] = i;
    if (fork() == 0) {
      // child code
      cout << "I'm child proccess with pid: " << getpid() << endl;
      funkcija();

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
      exit(0);
    }
  }

  sleep(1);
  for(int i = 0; i < procesa; i++){
    cout << "Waited on proccess " << i << endl;
    wait(NULL);
  }

  oslobodi_memoriju(0);

  return 0;
}
