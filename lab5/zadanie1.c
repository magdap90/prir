#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include<math.h>
#include<time.h>
#include "err.h"
#include "err.c"

float func(float x)
{
    return (4*x-6*x+5);
}
 float met_trap(float xp, float xk, int n)
 {
     float dx, calka = 0;
        dx= (xk - xp)/n;

        for( int i =1; i<n; i++)
        {
            calka += func(xp+i*dx);
        }
        calka +=(func(xp)+func(xk))/2;
        calka *=dx;
        printf("calka %f \n", calka);
 }
int proces3()
{
   srand(getpid()+getppid());
    int n=0;
    n= (rand()%4000+100);
}

float proces()
 {
     srand(getpid()+getppid());
     float x = 5.0;
     float a = (float)rand()/(float)(RAND_MAX/x);
     float b = (float)rand()/(float)(RAND_MAX/x);
     int n = (rand()%1000+100);
     if(a<b){
      met_trap(a,b,n);
     }
     else{
        printf("a>b niestety nie mozna policzyc calki \n");
     }

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
 switch (pid = fork()) {                                                                                                 case -1: /* blad */
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
