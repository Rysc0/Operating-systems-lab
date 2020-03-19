#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include<iostream>
using namespace std;
// the sum computed by the background thread
int sum = 0;

// thread function to generate sum of 0 to n
void* sum_runner(void* arg){
  int *brojeva_ptr = (int *) arg;
  int brojeva = *brojeva_ptr;

  for(int i=0;i<=brojeva;i++){
    sum += i;
  }
  // TODO: What do to with the answer
  // sum is a global variable so other threads can access
  pthread_exit(0);
  // 0 je prenesena u pthread_join(arg,0)

}

int main(int argc, char **argv) {

  if(argc < 2){
    cout << "usage " << argv[0] << endl;
    exit(-1);
  }

  int brojeva = atoi(argv[1]);

  // thread id
  pthread_t pid;

  // create attributes
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_create(&pid,&attr,sum_runner,&brojeva);
  /* &brojeva je argument koji se prosljeđuje u thread, tj onaj argument sa terminala*/


  // do other stuff here

  // wait until thread is done
  pthread_join(pid,NULL);

  cout << "Sum is " << sum << endl;
  return 0;
}
