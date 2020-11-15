#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "err.h"
#include "err.c"
#include <math.h>
#include <time.h>
#include <sys/mman.h>

float lib()
{
    srand(getpid()+getppid());
    int n=0;
    n= (rand()%4000+100);
    float wynik = 0, liczba = 0, zmienna = 0;
    for(int i=0; i<=n; i++){
        liczba =(pow((- 1.0), i ) ) /( 2 * i + 1 );
        zmienna +=liczba;
    }
    wynik = 4*zmienna;
return wynik;
}
int proces()
{
   printf(" piiiii    %f \n", lib());
}

int main ()
{
 printf("Prosze podac ilosc procesow: ");
 int ilosc;
 scanf("%d", &ilosc);

 pid_t pid;
 int i;
 /* tworzy nowe procesy */
 for (i = 1; i <= ilosc; i++)
 switch (pid = fork()) {
 case -1: /* blad */
 syserr("Error in fork\n");

 case 0: /* proces potomny */
 printf(" Proces %d rozpoczyna prace \n ",i); 
 proces();
 return 0;

 }

 /* czeka na zakonczenie procesow potomnych */
 for (i = 1; i <= ilosc; i++) { 
     if (wait(0) == -1){
         syserr("Error in wait\n");
     }
 }
 return 0;

}

