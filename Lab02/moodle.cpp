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

int *ptr,*ptr1,*Nprocesa;

void f(int polje[], int size){
  cout << "Ide polje" << endl;
  for(int i=0;i<5;i++){
    ptr[i] = 0;
    cout << ptr[i] << endl;
  }
  cout << "Idu PID-ovi" << endl;
  for(int i = 0; i<size;i++){
    cout << polje[i] << endl;
  }
}

void funkcija(){
  printf("\x1B[31mNEXT PROCCESS...\033[0m\n");
}

int main(int argc, char **argv) {

  int procesa = atoi(argv[1]);
  int array[procesa];
  int pid[procesa];
  ptr = array;
  ptr1 = pid;
  //Nprocesa = procesa;
  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    if (fork() == 0) {
      // child code
      pid[i] = getpid();
      cout << pid[i] << endl;
      cout << "I'm child proccess with pid: " << getpid() << endl;
      //for(int i = 0; i < procesa; i++){
        funkcija();
        //sleep(1);
        for(int k = 1; k <=5; k++){
          //uđi_u_kritični_odsječak(k);
          for(int m = 1; m <=5; m++){
            //ispisi(i,k,m);
            cout << "Proces " << i << " K.O. br: " << k << " (" << m << "/5)" << endl;
          }
          // izađi iz kritičnog odsječka
        }
      //}
      exit(0);
    }
  }
  /*cout << "Resavam PIDove: " << endl;
  for(int i = 0; i < procesa; i++){
    cout << pid[i] << endl;
  //}
*/
  sleep(2);
  for(int i = 0; i < procesa; i++){
    cout << "Waited on proccess " << i << endl;
    wait(NULL);
  }
  // f(pid,procesa);




  return 0;
}
