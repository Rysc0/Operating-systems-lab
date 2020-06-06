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



int shared_memory_id;
int moze_uci_n;
int br_svake_vrste;

struct podaci{
  pthread_mutex_t lock;
  int uvjet[2]; // pthread_cont_t uvjet
  int br[2] = { 0,0 };
  bool vrsta;
  int ceka[2] = { 0,0 };
  int SITI = 0;
};
podaci podatak;
void *ptr;
// ptr = &podatak;

void udji(bool vrsta){
  pthread_mutex_lock(&podatak.lock);
  // ispis trenutnog stanja u restoranu
  podatak.ceka[podatak.vrsta]++;
  while((podatak.br[1-podatak.vrsta]>0) || ((podatak.SITI>=moze_uci_n) && (podatak.ceka[1-podatak.vrsta]>0))){
    pthread_cond_wait(&podatak.uvjet[podatak.vrsta],&podatak.lock);
  }
  podatak.br[podatak.vrsta]++;
  podatak.ceka[podatak.vrsta]--;
  if(podatak.ceka[1-podatak.vrsta]>0){
    podatak.SITI++;
  }
  // ispis trenutnog stanja u restoranu
  pthread_mutex_unlock(&podatak.lock);
}

void izadji(bool vrsta){
  pthread_mutex_lock(&podatak.lock);
  podatak.br[podatak.vrsta]--;
  if(podatak.br[podatak.vrsta]==0){
    podatak.SITI = 0;
    pthread_cond_broadcast(&podatak.uvjet[1-podatak.vrsta]);
  }
  pthread_mutex_unlock(&podatak.lock);
}


void brisi(int sig){
  shmdt(ptr);
  shmctl(shared_memory_id, IPC_RMID, NULL);
  exit(1);
}


int main(int argc, char *argv[]){
  
srand(time(NULL));
moze_uci_n = atoi(argv[1]);
br_svake_vrste = atoi(argv[2]);

cout << moze_uci_n << "\t" << br_svake_vrste << endl;
ptr = &podatak;

shared_memory_id = shmget(IPC_PRIVATE, sizeof(podatak), 0600);
if(shared_memory_id == -1){
  cout << "ERROR, NO SHARED MEMORY!" << endl;
  exit(1);
}
sigset(SIGINT, brisi);

// popravi 
ptr = (memorija *) shmat(shared_memory_id, NULL, 0);

// generiranje procesa
for(int i = 0; i < br_svake_vrste; i++){
  // napravi linux programera
  if(podatak.vrsta == true){
    if(fork()==0){
      funkcija();
    }
    podatak.vrsta == false;
  }
  // napravi microsoft programera
  else if(podatak.vrsta == false){
    if(fork()==0){
      funkcija();
    }
    podatak.vrsta == true;
  }
}

// funkcija(){
//   usleep(rand()%100);
//   programiraj;
//   udji(vrsta);
//   usleep(rand()%100);
//   jedi;
//   izadji(vrsta);
// }





  return 0;
}