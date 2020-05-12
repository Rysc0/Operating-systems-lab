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


sem_t *semafor1;
sem_t *semafor2;

int *generirani_broj;
int memory_id;

bool *zadatak;

void brisi(int sig){
  shmdt(generirani_broj);
  shmdt(semafor1);
  shmdt(semafor2);
  shmdt(zadatak);
  shmctl(memory_id,IPC_RMID,NULL);
}



// glavni program (parametri) 
int main(int argc, char *argv[]){

if(argc != 3){
    cout << "ERROR, must have 3 arguments!" << endl;
    exit(-1);
  }    

  srand(time(NULL));
  cout << sizeof(int) << endl;
  int m_procesa = atoi(argv[1]);
  int n_brojeva = atoi(argv[2]);
  


// napravi dijeljenu memoriju
memory_id = shmget(IPC_PRIVATE,2*sizeof(sem_t)+sizeof(int)+sizeof(bool),0600); // veličina za 2 semafora i generirani broj
if(memory_id == -1){
    cout << "ERROR; NO SHARED MEMORY!" << endl;
    exit(-1);
  }

// ubaci varijablu i semafore u dijeljenu memoriju
generirani_broj = (int*) shmat(memory_id,NULL,0);
semafor1 = (sem_t *) shmat(memory_id,NULL,0);
semafor2 = (sem_t *) shmat(memory_id,NULL,0);
zadatak = (bool *) shmat(memory_id,NULL,0);

sigset(SIGINT,brisi);

// inicijalizacija semafora
int sem_id1 = sem_init(semafor1,1,0);
if(sem_id1 != 0){
  cout << "nema semafora" << endl;
}
int sem_id2 = sem_init(semafor2,1,1);
if(sem_id2 != 0){
  cout << "nema semafora" << endl;
}



//pokreni m procesa racunaj;   
for(int i = 0; i < m_procesa; i++){
    if(fork() == 0){
      cout << "Proces " << i << endl;
      //sum(m_procesa,i);
    // proces racunaj 
    //dok (ima zadataka){ 
      //*zadatak == false;
      //while(*zadatak == false){
        //čekaj da proces generiraj stavi sljedeći broj; //rabiti semafor    - sem_wait(semafor2); - čeka da proces generira broj
        //pročitaj dijeljenu varijablu;
        sem_wait(semafor2);
        //int procitana_vrijednost = *generirani_broj;
        cout << "Proces " << i << " -> procitana vrijednost je: " << *generirani_broj << endl;
        //izračunaj zbroj;    
        int zbroj = 0;
        for(int i = 0; i < *generirani_broj; i++){
          zbroj = zbroj + i;
        }
        //ispiši zbroj;
        cout << "Zbroj je: " << zbroj << endl;
        //javi procesu generiraj da je dijeljena varijabla pročitana; //rabiti semafor   - sem_post(semafor); - pušta proces da generira idući broj
        sem_post(semafor1);
      //}
//    }
// }
      
    exit(1);
    }
}

//pokreni proces generiraj;
if(fork() == 0){
    
    //generator(n_brojeva); 
// proces generiraj 
// {
//dok (nije generirano n zadataka){
  for(int i = 0; i < n_brojeva; i++){
  //generiraj slučajni broj;
  //spremi ga u dijeljenu varijablu;
    sem_wait(semafor1);
    //*zadatak == false;
    *generirani_broj = rand()%10;
    cout << "Generirani broj je: " << *generirani_broj << endl;
   // *zadatak == true;
    //javi procesima da je stavljeni novi slučajni broj; //rabiti semafor   - sem_post(semafor2); - pušta proces da pročita i računa
    sem_post(semafor2);
    //čekaj da neki proces pročita slučajni broj; //rabiti semafor          - sem_wait(semafor);  - čeka da proces pročita

  }    
  exit(1);
}


//čekaj na završetak svih procesa;
for(int i = 0; i < m_procesa+1; i++){
    wait(NULL);
  }




// //procesi mogu provjeravati vrijednost neke dijeljene varijable da bi provjerili da li ima još slučajnih brojeva

  // brise dijeljenu memoriju
  brisi(SIGINT);

  return 0;
}