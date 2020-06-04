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

// sem_t binarni_semafor;
int counter;
int broj[100];
pthread_mutex_t lock;

void* thread_funkcija(void* arg){
  pthread_mutex_lock(&lock);

  cout << "Ja sam thread " << counter << endl;
  cout << "Unesi broj: ";
  cin >> broj[counter];
  counter = counter +1;

  pthread_mutex_unlock(&lock);
  cout << "Ispis: " << broj[counter] << endl;
  pthread_exit(0);
  // return NULL;
}


int main(int argc, char *argv[]){


int prvi_argument = atoi(argv[1]);
pthread_t thread_id[prvi_argument];

if (pthread_mutex_init(&lock, NULL) != 0) { 
  cout << "\n mutex init has failed\n" << endl; 
  return 1; 
} 


for(int i = 0; i < prvi_argument; i++){
  if(pthread_create(&thread_id[i],NULL,thread_funkcija,&i)!=0){
    cout << "Error, thread not created\n" << thread_id << endl;
    exit(1);
  }  
}

for(int i = 0; i < prvi_argument; i++){
  pthread_join(thread_id[i],NULL);
}


pthread_mutex_destroy(&lock);

  return 0;
}





// int pthread_mutex_init(pthread_mutex_t *ključ, const pthread_mutexattr_t *attr);
// int pthread_mutex_lock(pthread_mutex_t *ključ);
// int pthread_mutex_unlock(pthread_mutex_t *ključ);
// int pthread_mutex_destroy(pthread_mutex_t *ključ);




// int pthread_cond_init(pthread_cond_t *uvjet, const pthread_condattr_t *attr);
// int pthread_cond_wait(pthread_cond_t *uvjet, pthread_mutex_t *ključ);
// int pthread_cond_signal(pthread_cond_t *uvjet);
// int pthread_cond_broadcast(pthread_cond_t *uvjet);
// int pthread_cond_destroy(pthread_cond_t *uvjet); 