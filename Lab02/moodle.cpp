#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<stdlib.h>
#include<ctime>
#include<unistd.h>
#include<iostream>

int ZajednickaVarijabla;

void *Pisac(void *x)
{
   ZajednickaVarijabla = *((int*)x);
}

void *Citac(void *x)
{
   int i;

   do {
      i = ZajednickaVarijabla;
      printf("Procitano %d\n", i);
      sleep(1);
   } while (i == 0);

   printf("Procitano je: %d\n", i);
}

int main()
{
   int i;
   pthread_t thr_id[2];

   ZajednickaVarijabla = 0;
   i=123;
   
   /* pokretanje dretava */
   if (pthread_create(&thr_id[0], NULL, Citac, NULL) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
   }
   sleep(5);
   if (pthread_create(&thr_id[1], NULL, Pisac, &i) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
   }

   pthread_join(thr_id[0], NULL);
   pthread_join(thr_id[1], NULL);

   return 0;
}
