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


// struktura semafora + memorija
struct memorija{
  int ULAZ, IZLAZ, UKUPNO, BUFFER[5];
}

*ptr = new memorija;

int shared_memory_id;
int semafor_id;

int semafor_operation(int number,int operation){
  struct sembuf Semafor;
  Semafor.sem_num = number;
  Semafor.sem_op = operation;
  Semafor.sem_flg = 0;
  return semop(semafor_id, &Semafor, 1);
}

void producer (int producer_id, int amount_of_numbers){
  
  for(int i = 0; i < amount_of_numbers; i++){

    semafor_operation(2,-1);
    semafor_operation(1,-1);
    srand(time(NULL));
    ptr->BUFFER[ptr->ULAZ] = 1+rand()%1000;
    sleep(1);
    cout << "Proizvodac " << producer_id << " salje " << ptr->BUFFER[ptr->ULAZ] << endl;
    ptr->ULAZ = (ptr->ULAZ+1) %5;
    semafor_operation(1, 1);
    semafor_operation(3, 1);
  }

  cout << "Proizvodac " << producer_id << " je zavrsio sa slanjem " << endl;
  sleep(1);
}


void consumer (int number_of_producers, int amount_of_numbers){
  
  int zbroj = 0;
  int temp = number_of_producers * amount_of_numbers;
  for(int i = 0; i < temp; i++){

    semafor_operation(3,-1);
    sleep(1);
    cout << "Potrosac je primio: " << ptr->BUFFER[ptr->IZLAZ] << endl;
    zbroj = zbroj + (ptr->BUFFER[ptr->IZLAZ]);
    ptr->IZLAZ = (ptr->IZLAZ+1) %5;
    semafor_operation(2,1);
  }

  cout << "Potrosac - zbroj brojeva koje je primio: " << zbroj << endl;
  sleep(1);
}


void brisi(int sig){
  shmdt(ptr);
  semctl(semafor_id, 0, IPC_RMID, 0);
  shmctl(shared_memory_id, IPC_RMID, NULL);
  exit(1);
}

int main(int argc, char *argv[] ){

  if(argc != 3){
    cout << "ERROR, must have 3 arguments!" << endl;
    exit(-1);
  }    

  int number_of_producers = atoi(argv[1]); 
  int amount_of_numbers = atoi(argv[2]); 
 

  if(((number_of_producers <= 0)||(amount_of_numbers <= 0))||((number_of_producers <= 0)&&(amount_of_numbers <= 0))){
    cout << "Input arguments not in scope!" << endl;
    exit(-1);
  }


  semafor_id = semget(IPC_PRIVATE, sizeof(int), 0600);

  if(semafor_id == -1){
    cout << "ERROR, NO SEMAPHORES!" << endl;
    exit(1);
  }

  semctl(semafor_id, 3, SETVAL, 0);
  semctl(semafor_id, 1, SETVAL, 1);
  semctl(semafor_id, 2, SETVAL, 5);

  shared_memory_id = shmget(IPC_PRIVATE, sizeof(ptr), 0600);
  
  if(shared_memory_id == -1){
    cout << "ERROR, NO SHARED MEMORY!" << endl;
    exit(1);
  }

  sigset(SIGINT, brisi);
 

  ptr = (memorija *) shmat(shared_memory_id, NULL, 0);

  ptr->ULAZ = 0;
  ptr->IZLAZ = 0;


  cout << "PROIZVOĐAČA = " << number_of_producers << endl;
  cout << "BROJEVA ZA PROIZVESTI = " << amount_of_numbers << endl;

  for(int i = 0; i < number_of_producers; i++){
    if(fork() == 0){
      producer(i,amount_of_numbers);
      exit(1);
    }
  }

  if(fork() == 0){
    consumer(number_of_producers,amount_of_numbers); 
    exit(1);
  }

  for(int i = 0; i < number_of_producers+1; i++){
    wait(NULL);
  }


  brisi(SIGINT);

  return 0;
}
