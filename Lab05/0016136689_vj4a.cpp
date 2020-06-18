#include <iostream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <pthread.h>
using namespace std;

int cnt;
int br[50];
pthread_mutex_t l;
pthread_cond_t uv;
int *pom;

void bar(){
	
  pthread_mutex_lock(&l);
  cout << "Ja sam thread " << cnt << endl;
  cout << "Unesi broj: ";
  cin >> br[cnt];
  cnt = cnt + 1;
  
   if (cnt < *pom){
    pthread_cond_wait(&uv, &l);
  		}
  		
  else{
      cnt = 0;
      pthread_cond_broadcast(&uv);
  		}
  		
  pthread_mutex_unlock(&l);
	}
	

void* thread_func(void* arg){
  

  bar();
  cout << "Thread " << cnt << "\tIspis: " << br[cnt] << endl;
  cnt++;
  
  pthread_exit(0);
 
	}


int main(int argc, char *argv[]){


int N = atoi(argv[1]);
pthread_t thread_id[N];
pom = &N;

if (pthread_mutex_init(&l, NULL) != 0) { 
  cout << "\n mutex init has failed\n" << endl; 
  return 1; 
	} 


for(int i = 0; i < N; i++){
  if(pthread_create(&thread_id[i],NULL,thread_func,&i)!=0){
    cout << "Error, thread not created\n" << thread_id << endl;
    exit(1);
  		}  
	}

for(int i = 0; i < N; i++){
	pthread_join(thread_id[i],NULL);
	}


pthread_mutex_destroy(&l);

	return 0;
}
