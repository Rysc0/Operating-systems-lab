#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<cstdlib>
#include<unistd.h>
#include <sys/wait.h>

int Id; /* identifikacijski broj segmenta */
int *ZajednickaVarijabla;

void Pisac(int i)
{
   *ZajednickaVarijabla = i;
}

void Citac()
{
   int i;

   do {
      i = *ZajednickaVarijabla;
      printf("Procitano %d\n", i);
      sleep(1);
   } while (i == 0);

   printf("Procitano je: %d\n", i);
}
void brisi(int sig)
{
   /* oslobađanje zajedničke memorije */
   shmdt(ZajednickaVarijabla);
   shmctl(Id, IPC_RMID, NULL);
   exit(0);

}
int main()
{
   /* zauzimanje zajedničke memorije */
   Id = shmget(IPC_PRIVATE, sizeof(int)*100, 0600);
   /* sizeof(int) je dovoljno memorije, ali u nekim slucajevima (pinus)
      treba zatraziti vise memorije, pa je u primjeru broj pomnozen sa 100 */

   if (Id == -1)
      exit(1);  /* greška - nema zajedničke memorije */

   ZajednickaVarijabla = (int *) shmat(Id, NULL, 0);
   *ZajednickaVarijabla = 0;
   sigset(SIGINT, brisi);//u slučaju prekida briši memoriju

   /* pokretanje paralelnih procesa */
   if (fork() == 0) {
      Citac();
      exit(0);
   }
   if (fork() == 0) {
      sleep(5);
      Pisac(123);
      exit(0);
   }
   wait(NULL);
   wait(NULL);
   brisi(0);

   return 0;
}
















/*srand(time(NULL));
cout << "Argumenata: " << argc << endl;
cout << "Drugi argument: " << argv[1] << endl;
int n = atoi(argv[1]);
int array[n];
for(int i = 0; i < n; i++){
  array[i] = rand()%10;
  cout << array[i] << endl;
}*/


  /*
  bool mozes = false;
  int main(){

    srand(time(NULL));
    int a[5];
    for(int i = 0; i < 5; i++){
      while(!mozes) {
        sleep(1);
        a[i] = rand()%1000;
        cout << "Random broj je: " << a[i] << endl;
        mozes = true;
      }


      while(mozes){
        cout << "2. funkcija\n";
        mozes = false;
      }
    }*/
