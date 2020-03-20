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
#include<iostream>
using namespace std;
// the sum computed by the background thread
int sum = 0;
int global_variable[10];

// thread function to generate random numbers from 0 to arg
void* sum_runner(void* arg){
  // typecasting void arg to int
  int *brojeva_ptr = (int*) arg;
  int brojeva = *brojeva_ptr;
  // generating random numbers
  for(int i = 0; i < brojeva; i++){
    int number = rand()%5;
    global_variable[i] = number;
    cout << global_variable[i] << endl;
  }

  pthread_exit(0);
  // 0 je prenesena u pthread_join(arg,0)

}

void* addition(void* arg){

    for(int i = 0; i < 5; i++){

      sum = sum + global_variable[i];
    }
    cout << "Zbroj je: " << sum << endl;


}

int main(int argc, char **argv) {

  srand(time(NULL));
  int brojeva = atoi(argv[1]);
  int limit = atoi(argv[2]);

  if(argc < 2 || argc > 3){
    cout << "usage " << argv[0] << endl;
    exit(-1);
  }
  else{
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

  for(int z = 0; z < limit-1; z++){
    for(int i = 0; i < limit-1; i++){ // argv[2] is M (how many times threads need to run)
      pthread_create(&pid,NULL,sum_runner,&brojeva);
      /* &brojeva je argument koji se prosljeđuje u thread, tj onaj argument sa terminala*/
    }
    pthread_create(&pid1,NULL,addition,NULL); // change return value later
  
    // do other stuff here
  }

  // wait until thread is done
  pthread_join(pid,NULL);
  pthread_join(pid1,NULL);

  cout << "Sum is " << sum << endl;
  return 0;
}
