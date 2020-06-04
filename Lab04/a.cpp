#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;
 
 
int counter = 0, broj[10];
int n;
pthread_cond_t uv;
pthread_mutex_t lock;
 
 
 
 
void barijera(){
  pthread_mutex_lock(&lock);
  counter = counter +1;
  if (counter < n){
    pthread_cond_wait(&uv, &lock);
  }
  else{
      counter = 0;
      pthread_cond_broadcast(&uv);
  }
  pthread_mutex_unlock(&lock);
}
 
 
 
 
void* thread_funkcija(void* arg){
  int br = *(int*)arg;
  pthread_mutex_lock(&lock);
  cout << "Dretva: " << br << endl;
  cin >> broj[br];
  pthread_mutex_unlock(&lock);
  barijera();
  cout << "Dretva: " << br << " iznosi " << broj[br] << endl;
 
  pthread_exit(0);
 
}
 
 
 
 
 
int main(int argc, char *argv[]){
 
    int prvi_argument = atoi(argv[1]);
    int array[prvi_argument];
    pthread_t thread_id[prvi_argument];
     
    n = prvi_argument;
     
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
   
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&uv);
    return 0;
}