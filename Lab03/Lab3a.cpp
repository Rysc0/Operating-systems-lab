#include <iostream>
#include <semaphore.h>
#include <ctime>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
using namespace std;

int *broj,id;

void sum(int prvi_arg, int indeks){
  int zbroj;
  for(int i = 1; i < broj[0]; i++){
    zbroj += i;
  }
 cout << "Proces " << indeks << "\t" << "Zbroj je: " << zbroj << endl;
}


void generator(int drugi_arg){
  for(int i = 0; i < drugi_arg; i++){
    broj[0] = rand()%10;
    cout << "Generiran broj " << broj[0] << endl;
  }
}


void brisi(int sig){
  shmdt(broj);
  shmctl(id,IPC_RMID,NULL);
  exit(1);
}



int main(int argc, char *argv[]){
    
  if(argc != 3){
    cout << "ERROR, must have 3 arguments!" << endl;
    exit(-1);
  }    

  srand(time(NULL));

  int prvi_argument = atoi(argv[1]);
  int drugi_argument = atoi(argv[2]);

  id = shmget(IPC_PRIVATE,sizeof(int),0600);
  if(id == -1){
    cout << "ERROR; NO SHARED MEMORY!" << endl;
    exit(-1);
  }

  broj = (int*) shmat(id,NULL,0);
    
  sigset(SIGINT, brisi);

  for(int i = 0; i < prvi_argument; i++){
    if(fork() == 0){
      sum(prvi_argument,i);
      exit(1);
    }
  }

  if(fork() == 0){
    generator(drugi_argument);
    exit(1);
  }

  for(int i = 0; i < prvi_argument+1; i++){
    wait(NULL);
  }
  
  brisi(SIGINT);

  return 0;
}