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

int *array, id;
/*
void kriticni_odsjecak(i){

}*/

void funkcija(){
  printf("\x1B[31mNEXT PROCCESS...\033[0m\n");
}

int main(int argc, char **argv) {

  int procesa = atoi(argv[1]);

  int polje[10];
  array = polje;

  id = shmget(IPC_PRIVATE,sizeof(int)*10,0600);
  if(id == -1){
    cout << "ERROR, NO SHARED MEMORY!" << endl;
    exit(1);
  }

  array = (int*) shmat(id,NULL,0);

  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    if (fork() == 0) {
      // child code
      cout << "I'm child proccess with pid: " << getpid() << endl;
      funkcija();

      for(int k = 1; k <=5; k++){
        //uđi_u_kritični_odsječak(k);
        for(int m = 1; m <=5; m++){
          //ispisi(i,k,m);
          cout << "Proces " << i << " K.O. br: " << k << " (" << m << "/5)" << endl;
        }
        // izađi iz kritičnog odsječka
      }
      exit(0);
    }
  }

  sleep(2);
  for(int i = 0; i < procesa; i++){
    cout << "Waited on proccess " << i << endl;
    wait(NULL);
  }

  shmdt(array);
  shmctl(id,IPC_RMID,NULL);



  return 0;
}
