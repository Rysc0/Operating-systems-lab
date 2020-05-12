#include <iostream>
#include <semaphore.h>
#include <ctime>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <cstdlib>
using namespace std;

int m_procesa; 
int n_brojeva;
int shared_memory_id;

struct memorija{
  int ULAZ,IZLAZ;
  int BUFFER[1000];
  sem_t semafor1;
  sem_t semafor2;
}

*ptr = new memorija;

void brisi_memoriju(int sig){
  shmdt(ptr);
  shmctl(shared_memory_id,IPC_RMID,NULL);
  exit(0);
}

void unisti_semafore(){
  sem_destroy(&ptr->semafor1);
  sem_destroy(&ptr->semafor2);
}

void generator(){
   //dok (nije generirano n zadataka){
  do{
    ptr->BUFFER[ptr->ULAZ] = rand()%100000000;
    cout << "Generiran broj: " << ptr->BUFFER[ptr->ULAZ] << endl;
    sem_post(&ptr->semafor1);
    sem_wait(&ptr->semafor2);
    n_brojeva--;
  }while(n_brojeva>0);
}

void calculator(int number){

  long int zbroj = 0;
  do{
    sem_wait(&ptr->semafor1);
    int procitani_broj = ptr->BUFFER[ptr->IZLAZ];
    cout << "Proces " << number << " je preuzeo broj/zadatak " << ptr->BUFFER[ptr->IZLAZ] << endl;
    sem_post(&ptr->semafor2);
    for(int i = 0; i < procitani_broj; i++){
      zbroj = zbroj + i;
    }
    cout << "Proces " << number << "\t" << "Broj " << procitani_broj << "\t" << "Zbroj = " << zbroj << endl;
    zbroj = 0;
  }while (n_brojeva>=0);
      
}







// glavni program (parametri) 
int main(int argc, char *argv[]){

if(argc != 3){
  cout << "ERROR, must have 3 arguments!" << endl;
  exit(-1);
}    

srand(time(NULL));

m_procesa = atoi(argv[1]);
n_brojeva = atoi(argv[2]);
  
cout << "Proces generator je započeo s radom!" << endl;
cout << "Treba generirati " << n_brojeva << "brojeva/zadataka!" << endl;

// napravi dijeljenu memoriju
shared_memory_id = shmget(IPC_PRIVATE,sizeof(ptr),0600); 
if(shared_memory_id == -1){
    cout << "ERROR; NO SHARED MEMORY!" << endl;
    exit(1);
}
sigset(SIGINT,brisi_memoriju);

// ubaci buffer i semafore u dijeljenu memoriju
ptr = (memorija*) shmat(shared_memory_id,NULL,0);

// inicijalizacija semafora
int sem_id1 = sem_init(&ptr->semafor1,1,0);
if(sem_id1 != 0){
  cout << "nema semafora 1" << endl;
}
int sem_id2 = sem_init(&ptr->semafor2,1,0);
if(sem_id2 != 0){
  cout << "nema semafora 2 " << endl;
}

ptr->ULAZ = 0;
ptr->IZLAZ = 0;



// pokreni proces generator
if(fork()==0){
  generator();
  exit(0);
}

//pokreni m procesa racunaj;   
for(int i = 0; i < m_procesa; i++){
  if(fork() == 0){
    cout << "Proces " << i << " je startao!" << endl;
    calculator(i);      
    exit(0);
  }
}



while(n_brojeva>=0){
  n_brojeva--;
}
sleep(1);

// brise dijeljenu memoriju
brisi_memoriju(SIGINT);
unisti_semafore();

return 0;
}