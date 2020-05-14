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


// semafori + buffer
struct memorija{
  int ULAZ, IZLAZ, BUFFER[100];
  sem_t semafor, semafor2;
  
}

*ptr = new memorija;

int shared_memory_id;

void generator(int n_brojeva){
  
  for(int i = 0; i < n_brojeva; i++){
    ptr->BUFFER[ptr->ULAZ] = rand()%100000000;
    cout << "Generirani broj: " << ptr->BUFFER[ptr->ULAZ] << endl;
    sem_post(&ptr->semafor);
    sem_wait(&ptr->semafor2);
  }  
}


void calculator(int number,int n_brojeva){

  long int zbroj = 0; 

  do{
    sem_wait(&ptr->semafor);
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


void brisi_memoriju(int sig){
  shmdt(ptr);
  shmctl(shared_memory_id,IPC_RMID,NULL);
  exit(0);
}

void unisti_semafore(){
  sem_destroy(&ptr->semafor);
  sem_destroy(&ptr->semafor2);
}




int main(int argc, char *argv[]){

if(argc != 3){
  cout << "ERROR, must have 3 arguments! Arg 2 & 3 must be >0" << endl;
  exit(-1);
}    

srand(time(NULL));

int m_procesa = atoi(argv[1]);
int n_brojeva = atoi(argv[2]);

if(((m_procesa<=0)||(n_brojeva<=0))||((m_procesa<=0)&&(n_brojeva<=0))){
    cout << "Input arguments not in scope!" << endl;
    exit(-1);
  }
  

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
int sem_id1 = sem_init(&ptr->semafor,1,0);
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
  cout << "START ----> GENERATOR ----> " << n_brojeva << " TASKS NEEDED!" << endl;
  generator(n_brojeva);
  exit(1);
}

//pokreni m procesa racunaj;   
for(int i = 0; i < m_procesa; i++){
  if(fork() == 0){
    cout << "START ----> PROCCESS " << i << endl;
    calculator(i,n_brojeva);      
    exit(1);
  }
}



while(n_brojeva>=0){
  n_brojeva--;
}
sleep(1);



// brise dijeljenu memoriju i semafore
brisi_memoriju(SIGINT);
unisti_semafore();

return 0;
}