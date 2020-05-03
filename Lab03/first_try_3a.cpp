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

using namespace std;

int *broj,id,*objekt_id;
sem_t *segment;

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


// void brisi(int sig){
//   shmdt(broj);
//   shmdt(objekt_id);
//   shmctl(id,IPC_RMID,NULL);
//   exit(1);
// }

//void unisti_semafore(sem_t *segment){
  // int *ss;
  // ss = &semafor;
  // sem_t *semafor_identifikator = (sem_t*) ss;
  //sem_destroy(segment); 
//}
// semafor mora biti inicijaliziran u dijeljenoj memoriji,
// za to ne mogu koristit IPC_PRIVATE nego *sem mora bit 
// mmap unutar dijeljene memorije


int main(int argc, char *argv[]){
    
  if(argc != 3){
    cout << "ERROR, must have 3 arguments!" << endl;
    exit(-1);
  }    

  srand(time(NULL));

  int prvi_argument = atoi(argv[1]);
  int drugi_argument = atoi(argv[2]);


  // // dobivanje dijeljene memorije za upis broja 
  // id = shmget(IPC_PRIVATE,sizeof(int),0666);
  // if(id == -1){
  //   cout << "ERROR; NO SHARED MEMORY!" << endl;
  //   exit(-1);
  // }
  // sigset(SIGINT, brisi);

  // broj = (int*) shmat(id,NULL,0);  

  const char* name = "OS";
  int memory_object = shm_open(name,O_RDWR|O_CREAT,0666);
  //cout << memory_object << endl;
  //ftruncate(memory_object,SIZE); - moguće da mi ne treba  
  const int SIZE = 100;


  void *ptr;
  ptr = mmap(NULL,SIZE,PROT_WRITE | PROT_READ,MAP_SHARED,memory_object,0);
  cout << ptr << endl;
 
  sleep(10);


  // attach variable broj in shm (random number will be stored here)
  // broj = (int*) shmat(id,NULL,0);
  // attach variable objekt_id (*sem varijabla za sem_init)
  // objekt_id = (int*) shmat(id,NULL,0);

  // typecast objekt_id to sem_t - dobijaš adresu di treba spojit semafor
  //segment = (sem_t*) objekt_id;
  
  // dobivanje semafora
  // int semafor_id = sem_init(segment,1,1);
  // if(semafor_id == -1){
  //   cout << "ERROR, semafor nije kreiran!!" << endl;
  //   exit(1);
  // }

  

  

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
  
  //unisti_semafore(segment);
  brisi(SIGINT);
  
  return 0;
}