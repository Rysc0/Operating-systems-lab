#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;


int counter = 0,broj;
int *brojac,*n,*red;
pthread_mutex_t lock;


void barijera(pthread_mutex_t lock,int array[]){
  pthread_mutex_lock(&lock);
  
  
  counter = counter +1;
  if(counter <= *n){
    int pthread_cond_wait(pthread_cond_t *array, pthread_mutex_t *lock);
  }
  else{
    counter = 0;
     int pthread_cond_broadcast(pthread_cond_t *array);
  }
  pthread_mutex_unlock(&lock);
}



void* thread_funkcija(void* arg){
 
  barijera(lock, red);
 
  
  pthread_exit(0);
  
}





int main(int argc, char *argv[]){
  

int prvi_argument = atoi(argv[1]);
int array[prvi_argument];
red = array;
pthread_t thread_id[prvi_argument];

n = &prvi_argument;

int pthread_cond_init(pthread_cond_t *array, const pthread_condattr_t __NULL);

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
int pthread_cond_destroy(*array);
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