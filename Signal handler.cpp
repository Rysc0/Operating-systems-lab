#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<time.h>
using namespace std;

int trenutni_prioritet = 0;
int prekidi[3];

void current_time() {
  time_t timer;
  time(&timer);
  cout << ctime(&timer);
}

void handler(int sig) {

  int prioritet;

  switch(sig) {
    case (SIGINT):
      prioritet = 1;
      break;
    case (SIGQUIT):
      prioritet = 2;
      break;
    case (SIGTSTP):
      prioritet = 3;
      break;
  }

  cout << endl;

  if(prioritet > trenutni_prioritet) {
    prekidi[prioritet] = trenutni_prioritet;
    trenutni_prioritet = prioritet;
    sigrelse(sig);
    cout << "-------------------------------------------" << endl;
    cout << "\t";
    current_time();
    cout << "\tPOCELA OBRADA PREKIDA " << prioritet << endl << endl;
    for(int i = 1; i <= 5; i++) {
      cout << "\tPrekid " << i << "/5" <<  "\tRazina: " << prioritet << endl;
      sleep(1);
    }

    cout << "\n\tZAVRSILA OBRADA PREKIDA " << prioritet << endl;
    cout << "\t";
    current_time();
    cout << "-------------------------------------------" << endl;
    sighold(sig);
    trenutni_prioritet = prekidi[prioritet];

    cout << endl;
  }
}

int main() {

  sigset(SIGINT, handler);
  sigset(SIGQUIT, handler);
  sigset(SIGTSTP, handler);

  cout << "\nMAIN PROGRAM RUNNING" << endl << endl;

  for(int i = 1; i <= 20; i++) {
    cout << "Main program " << i << "/20" << endl;
    sleep(1);
  }
  cout << "\nPROGRAM FINISHED" << endl;
  return 0;
  }
