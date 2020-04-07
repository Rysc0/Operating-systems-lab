#include<pthread.h>
#include<ctime>
#include<unistd.h>
#include<iostream>
using namespace std;

int sum = 0;
int global_variable[10];
bool usage = false;


void* sum_runner(void* arg){
  while(usage){
    sleep(1);
  }
  while(!usage){

    int *brojeva_ptr = (int*) arg;
    int brojeva = *brojeva_ptr;

    for(int i = 0; i < brojeva; i++){
      int number = rand()%100;
      global_variable[i] = number;
      cout << global_variable[i] << endl;
    }
    usage = true;

  pthread_exit(0);
  }
}

void* addition(void* arg){
  while(!usage){
    sleep(1);
  }
  while(usage){
    int *brojeva_ptr = (int*) arg;
    int brojeva = *brojeva_ptr;
    for(int i = 0; i < brojeva; i++){
      sum = sum + global_variable[i];
    }
    cout << "------------------------------" << endl;
    cout << "Zbroj je: " << sum << endl;
    cout << "------------------------------" << endl;
    sum = 0;
    usage = false;
  pthread_exit(0);
}
}

int main(int argc, char **argv) {

  srand(time(NULL));
  bool good;
  if(argc != 3){
    cout << "usage: must have exactly 3 arguments, "<< argv[0] <<" included\n";
    exit(-1);
  }
  else{
    good = 1;
  }

  int brojeva = atoi(argv[1]);
  int limit = atoi(argv[2]);


  if(good){
    if(brojeva > 10 && limit < 2){
      cout << "First argument limitations are 1-10!\nSecond argument must be greater than 2!\n";
      exit(-1);
    }
    if(brojeva > 10 || brojeva < 1){
      cout << "Accepting only numbers 1-10\n";
      exit(-1);
    }
    if(limit < 2){
      cout << "Limit must be greater than 2!\n";
      exit(-1);
    }
  }


  pthread_t pid;
  pthread_t pid1;


  for(int i = 0; i <= limit-1; i++){
    if(pthread_create(&pid,NULL,sum_runner,&brojeva)!=0){
      cout << "Error, thread not created\n" << pid << endl;
      exit(1);
    }
    sleep(1);
    if(pthread_create(&pid1,NULL,addition,&brojeva)!=0){
      cout << "Error, thread not created\n" << pid1 << endl;
      exit(1);
    }
  }

  pthread_join(pid,NULL);
  pthread_join(pid1,NULL);

  cout << "Program finished!"<< endl;
  return 0;
}
