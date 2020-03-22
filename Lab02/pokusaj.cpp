/*Napisati program koji stvara dvije nove dretve. Prva dretva generira N slučajnih brojeva (N je ulazni argument
programa kod njegovog pokretanja, maksimalna vrijednost neka bude ograničena na 10), ispisuje brojeve i sprema ih
u zajedničku globalnu memoriju. Druga dretva čita brojeve iz zajedničke globalne memorije, računa njihov zbroj i
ispisuje izračunan zbroj. Ponoviti navedene korake još M puta pri čemu je M drugi ulazni argument programa kod
njegovog pokretanja.

Napomene:

Budući da obje dretve pristupaju istoj zajedničkoj memoriji potrebno je osigurati da se istoj pristupa naizmjenice.
U tu svrhu se može iskoristiti dodatna varijabla u zajedničkoj memoriji čiju vrijednost obje dretve ispituju. Ako
dretva nema pravo pristupiti zajedničkoj memoriji tada MOŽE BITI U RADNOM ČEKANJU.
*/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include<unistd.h>
#include<iostream>
using namespace std;
// the sum computed by the background thread
int sum = 0;
int global_variable[10];
bool usage = false;

// thread function to generate random numbers from 0 to arg
void* sum_runner(void* arg){
  while(usage){
    sleep(1);
  }
  while(!usage){
    // typecasting void arg to int
    int *brojeva_ptr = (int*) arg;
    int brojeva = *brojeva_ptr;
    // generating random numbers
    for(int i = 0; i < brojeva; i++){
      int number = rand()%100;
      global_variable[i] = number;
      cout << global_variable[i] << endl;
    }
    usage = true;

  pthread_exit(0); // THREAD EXITS CUZ IT CAN'T ACCESS MEMORY SO IT DOESN'T DO ANYTHING
}
  // 0 je prenesena u pthread_join(arg,0)

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

  int brojeva = atoi(argv[1]); // koliko brojeva treba generirati
  int limit = atoi(argv[2]); // koliko puta treba generirati


  if(good){
    // limit argument value to 10
    if(brojeva > 10 && limit < 2){
      cout << "First argument limitations are 1-10!\nSecond argument must be greater than 2!\n";
      exit(-1);
    }
    if(brojeva > 10){
      cout << "Number is too big. Accepting numbers 1-10\n";
      exit(-1);
    }
    // avoiding segmentation fault
    if(limit < 2){
      cout << "Limit must be greater than 2!\n";
      exit(-1);
    }

  }

  // thread id
  pthread_t pid;
  pthread_t pid1;

  // limit is second argument (how many times to run)
  for(int i = 0; i <= limit-1; i++){
    if(pthread_create(&pid,NULL,sum_runner,&brojeva)!=0){
      cout << "Error, thread not created\n" << pid << endl;
      exit(1);
    }
    sleep(1);
    if(pthread_create(&pid1,NULL,addition,&brojeva)!=0){
      cout << "Error, thread not created\n" << pid1 << endl;
      exit(1);
    } // change return value later
    /* &brojeva je argument koji se prosljeđuje u thread, tj onaj argument sa terminala*/
    }

    // do other stuff here
//  }

  // wait until thread is done
  pthread_join(pid,NULL);
  pthread_join(pid1,NULL);

  //cout << "Sum is " << sum << endl;
  return 0;
}

  /* TODO: Figure out how to properly loop threads and how to make sum work every time. */
