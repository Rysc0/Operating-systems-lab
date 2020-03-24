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

int *ptr;

void f(){
  cout << "Ide polje" << endl;
  for(int i=0;i<5;i++){
    ptr[i] = 0;
    cout << ptr[i] << endl;
  }
}

int main(int argc, char **argv) {

  int procesa = atoi(argv[1]);
  int array[procesa];
  int polje[5];
  ptr = polje;
  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    if (fork() == 0) {
      // child code
       cout << "I'm child proccess with pid: " << getpid() << endl;
       exit(0);
    }
  }

  cout << "Waited on proccess " << endl;
  for(int i = 0; i < procesa; i++){
    wait(NULL);
  }
   f();


  return 0;
}
