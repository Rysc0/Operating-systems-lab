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
  pthread_cond_t uvjet[2]; // pthread_cont_t uvjet
  int br[2] = { 0,0 };
  bool vrsta;
  int ceka[2] = { 0,0 };
  int SITI = 0;
};
podaci podatak;
void *pod = new podaci;

pthread_mutexattr_t mon_atr;
pthread_condattr_t uv_atr;

void ispis (bool vrsta)
{
	cout<<"Red Linux: ";
    for(int i=0;i<M;i++) 
	{
		if (i<podaci->ceka[0])
			cout<<"L";
		else
			cout<<"-";
    }
    cout<<" Red MS: ";
    for(int i=0;i<M;i++) 
	{
        if (i<podaci->ceka[1])
           	cout<<"M";
        else
          	cout<<"-";
    }
		
    cout<<" Restoran: ";
    if(vrsta==0 || podaci->br[0]>0) {
        for(int i=0;i<podaci->br[0];i++) {
            cout<<"L";
        }
    }
    if(vrsta==1 || podaci->br[1]>0) {
        for(int i=0;i<podaci->br[1];i++) {
            cout<<"M";
        }
    }
    cout<<" --> ";
    
    if(vrsta==0) {
        cout<<"L";
    }
    else 
	{
        cout<<"M";
    }
}

void udji(bool vrsta){
  pthread_mutex_lock(&podatak.lock);
  // ispis trenutnog stanja u restoranu
  ispis(vrsta);
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
  ispis(vrsta);
  pthread_mutex_unlock(&podatak.lock);
}

void izadji(bool vrsta){
  pthread_mutex_lock(&podatak.lock);
  ispis(vrsta);
  podatak.br[podatak.vrsta]--;
  if(podatak.br[podatak.vrsta]==0){
    podatak.SITI = 0;
    pthread_cond_broadcast(&podatak.uvjet[1-podatak.vrsta]);
  }
  ispis(vrsta);
  pthread_mutex_unlock(&podatak.lock);
}


void brisi(int sig){
  shmdt(pod);
  shmctl(shared_memory_id, IPC_RMID, NULL);
  exit(1);
}


void funkcija(){
  usleep(rand()%100);
  udji(podatak.vrsta);
  usleep(rand()%100);
  izadji(podatak.vrsta);
  exit(1);
}

int main(int argc, char *argv[]){
  
srand(time(NULL));
moze_uci_n = atoi(argv[1]);
br_svake_vrste = atoi(argv[2]);

cout << moze_uci_n << "\t" << br_svake_vrste << endl;
//pod = &podatak;

shared_memory_id = shmget(IPC_PRIVATE, sizeof(podatak), 0600);
if(shared_memory_id == -1){
  cout << "ERROR, NO SHARED MEMORY!" << endl;
  exit(1);
}
sigset(SIGINT, brisi);

// popravi 
pod = shmat(shared_memory_id, NULL, 0);
pthread_mutexattr_init(&mon_atr);
pthread_mutexattr_setpshared(&mon_atr, PTHREAD_PROCESS_SHARED);
pthread_mutex_init(&podaci->mon, &mon_atr);
pthread_condattr_init(&uv_atr);
pthread_condattr_setpshared(&uv_atr, PTHREAD_PROCESS_SHARED);
// generiranje procesa
for(int i = 0; i < br_svake_vrste*2; i++){
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

 for(int i = 0; i < br_svake_vrste*2; i++){
  wait(NULL);
  cout << "Gotov sam...Wait 20 sec..." << endl;
  }
  
for(int i = 0; i < 10; i++){
  sleep(1);
}
// funkcija(){
//   usleep(rand()%100);
//   programiraj;
//   udji(vrsta);
//   usleep(rand()%100);
//   jedi;
//   izadji(vrsta);
// }
brisi(SIGINT);
  return 0;
}
