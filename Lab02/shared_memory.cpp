#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <iostream>
#include <ctime>
#include <cstdlib>        /*exit() */
#include <unistd.h>      /*fork()*/
#include <sys/wait.h>  /* wait()*/
using namespace std;

int *array, id;

int main(){

  int polje[10];
  array = polje;
  srand(time(NULL));

  id = shmget(IPC_PRIVATE,sizeof(int)*10,0600);
  if(id == -1){
    cout << "ERROR, NO SHARED MEMORY!" << endl;
    exit(1);
  }
  //int broj = (int*) array;
  array = (int*) shmat(id,NULL,0);

  if(fork() == 0){
    cout << "input values..." << endl;
    for(int i = 0; i < 10; i++){
      array[i] = rand()%100;
      cout << i << ". upis = " << array[i] << endl;
    }
  }
//  else cout << "First proces colapsed" << endl;

  //if(fork() == 0){
    //sleep(5);
    //cout << "Array..." << endl;
    cout << "Output values..." << endl;
    for(int i = 0; i < 10; i++){
      cout << i << "\t" << array[i] << endl;
    }
//  }

  //else cout << "second proces colapsed" << endl;

  wait(NULL);



  shmdt(array);
  shmctl(id,IPC_RMID,NULL);



  return 0;
  }
