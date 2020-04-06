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

int *array, id, *broj, procesa, zadnji = 0;

int max(){
  int max = broj[0];
  for(int i = 0; i < procesa; i++){
    if(max < broj[i]){
      max = broj[i];
    }
  }
  //cout << "max je: " << max << endl;
  return max;
}

void kriticni_odsjecak(int i){

  int j;
  array[i] = 1;
  broj[i] = max() + 1;
  array[i] = 0;

  for(j = 0; j < procesa; j++){
    while(array[j] != 0){

    }
    while(broj[j] != 0 && (broj[j] < broj[i] || (broj[j] == broj[i] && j < i))){

    }
  }

}


void izadji_iz_kriticnog_odsjecka(int i){
  broj[i] = 0;
}


void brisi(int sig){
  shmdt(array);
  shmdt(broj);
  shmctl(id,IPC_RMID,NULL);
  exit(1);
}


int main(int argc, char *argv[]) {

  if(argc != 2){
    cout << "Must enter two arguments! " << argv[0] << " included!" << endl;
    exit(-1);
  }
  procesa = atoi(argv[1]);


  if(procesa < 1 || procesa > 10){
      cout << "Limited process count 1-10\n";
      exit(-1);
    }


/*
  int polje_trazim[procesa];
  array = polje_trazim;
  int polje_broj[procesa];
  broj = polje_trazim;

*/

  id = shmget(IPC_PRIVATE,2*procesa*sizeof(int),0600);
  if(id == -1){
    cout << "ERROR, NO SHARED MEMORY!" << endl;
    exit(1);
  }
  cout << "\033[1;40;93mSHMID = " << id << "\033[0m" << endl;


  array = (int*) shmat(id,NULL,0);
  broj = (int*) shmat(id,NULL,0) + sizeof(int);


  sigset(SIGINT, brisi);  //u slučaju prekida briši memoriju


  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    broj[i] = i;
    if (fork() == 0) {
      // child code
      cout << "\033[3;40;92mChild with PID: " << getpid() << " created!\033[0m" << endl;

      sleep(1);
      for(int k = 1; k <=5; k++){
        kriticni_odsjecak(i);

        for(int m = 1; m <=5; m++){
          //ispisi(i,k,m);
          cout << "Proces " << i+1 << " K.O. br: " << k << " (" << m << "/5)" << endl;
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
  //  cout << "Waited on proccess " << i << endl;
    wait(NULL);
  }

  brisi(SIGINT);



  return 0;
}
