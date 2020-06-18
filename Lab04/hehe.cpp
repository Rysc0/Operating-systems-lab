#include <iostream>
#include <semaphore.h>
#include <ctime>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

using namespace std;

struct podaci{
  pthread_mutex_t lock;
  pthread_cond_t uvjet[2];
  int br[2] = { 0,0 };
  int vrsta;
  int ceka[2] = { 0,0 };
  int SITI = 0;
} *podatak;

pthread_mutexattr_t lock_atr;
pthread_condattr_t uvjet_atr;

int shared_memory_id;
int N_programera, programera_svake_vrste;

void ispis(int vrsta){
    
  cout << "Red Linux: ";
  for(int i = 0; i < programera_svake_vrste; i++){
    if (i < podatak->ceka[0]){
	    cout << "L";
    }
    else{
      cout << "-";
    }
  }
  cout << " Red Microsoft: ";
  for(int i = 0; i < programera_svake_vrste; i++){
      if (i < podatak->ceka[1]){
       	cout << "M";
      }
      else{
       	cout << "-";
      }
  }	
  cout << " Restoran: ";
  if(vrsta == 0 || podatak->br[0] > 0){ 
    for(int i = 0; i < podatak->br[0]; i++){
      cout << "L";
    } 
  }
  if(vrsta == 1 || podatak->br[1] > 0){ 
    for(int i = 0; i < podatak->br[1]; i++){
      cout << "M";
    }
  }
  cout << " --> ";
  if(vrsta == 0){
    cout << "L";
  }
  else{
    cout << "M";
  }
}

void udji(int vrsta){
  pthread_mutex_lock(&podatak->lock);
  podatak->ceka[vrsta]++;
  while(podatak->br[1-vrsta] > 0 || (podatak->SITI >= N_programera && podatak->ceka[1-vrsta] > 0)){
    ispis(vrsta);
		cout << " u red cekanja" << endl;
    pthread_cond_wait(&podatak->uvjet[vrsta], &podatak->lock);
  }
  podatak->br[vrsta]++;
  podatak->ceka[vrsta]--;

  if(podatak->ceka[1-vrsta] > 0){
	  podatak->SITI++;
  }
  ispis(vrsta);
	cout << " u restoran" << endl;
  pthread_mutex_unlock(&podatak->lock);
}

void izadji(int vrsta){
  pthread_mutex_lock(&podatak->lock);
  podatak->br[vrsta]--;
  if(podatak->br[vrsta] == 0){
    podatak->SITI = 0;
    pthread_cond_broadcast(&podatak->uvjet[1-vrsta]);
  }      
  ispis(vrsta);
	cout << " iz restorana" << endl;
  pthread_mutex_unlock(&podatak->lock);
}

void funkcija(int vrsta){
  usleep(rand()%100);
  udji(vrsta);
  usleep(rand()%100);
  izadji(vrsta);
}

void brisi(int sig){
  pthread_mutexattr_destroy(&lock_atr);
  pthread_mutex_destroy(&podatak->lock);
  pthread_condattr_destroy(&uvjet_atr);

  for(int i = 0; i < 2; i++){ 
    pthread_cond_destroy(&podatak->uvjet[i]);
  }   

  shmdt(podatak);
  shmctl(shared_memory_id, IPC_RMID, 0);
  exit(0);
}




int main(int argc, char *argv[]){
        
        
  if(argc != 3){
    cout << "ERROR! Must have 3 arguments!" << endl;
    exit(1);
  }
        
  N_programera = atoi(argv[1]);
  programera_svake_vrste = atoi(argv[2]);
  srand(time(NULL));
        
  shared_memory_id=shmget(IPC_PRIVATE, sizeof(podaci), 0600);
  if(shared_memory_id == -1){
    cout << "ERROR! No shared memory!" << endl;
    exit(1);
  }
  sigset(SIGINT, brisi);
        
  podatak=(podaci*)shmat(shared_memory_id, NULL, 0);
	
  pthread_mutexattr_init(&lock_atr);
  pthread_mutexattr_setpshared(&lock_atr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&podatak->lock, &lock_atr);
  pthread_condattr_init(&uvjet_atr);
  pthread_condattr_setpshared(&uvjet_atr, PTHREAD_PROCESS_SHARED);
  pthread_cond_init(&podatak->uvjet[0], &uvjet_atr);
  pthread_cond_init(&podatak->uvjet[1], &uvjet_atr);
        
       
       
  int i;
  for(i = 0; i < 2*programera_svake_vrste; i++){
    switch(fork()){
      case 0: 
        podatak->vrsta = 1-podatak->vrsta; 
        funkcija(podatak->vrsta);
        exit(1);
      case -1: 
        cout << "FORK ERROR!" << endl;
        exit(1);
    }
  }
		
  while(i != 0){
    wait(NULL);
    i--;
  }
  cout << "Restoran prazan!" << endl;
  brisi(SIGINT);
  return 0;
}

