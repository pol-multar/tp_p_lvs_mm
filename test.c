#include"stdio.h"
#include <unistd.h>

/**
 * Test qui permet d'afficher l'état initial
 * des fichiers stdin, stdout et stderr
 */
void test_lecture_2c(void){

    tracer(stdin);
    getchar();
    tracer(stdin);
    getchar();
    tracer(stdin);
}

/**
 * Test qui permet de vérifier
 * la lecture de deux caractères (mais 3 entrés) 
 */

void test_etatInit(void)
{
    tracer(stdin);
    tracer(stdout);
    tracer(stderr);
}

/**
 * Test d'écriture sur la sortie standard 
 * (qui est bufferisée par ligne)
 */
void test_ecritureStdout(void)
{
    tracer(stdout);
    putc('X',stdout);
    tracer(stdout);
    putc('Y',stdout);
    tracer(stdout);
    putc('\n',stdout);
    tracer(stdout);
}

void test_getc(void)
{
    int c;
    
    while((c=getchar())!=EOF)
	write(1,&c,1);
    /**
       while((c=getc(f))!=EOF)
       write(1,&c,1);
    */
}

void test_f_initialisation(void)
{
    tracer(stdin);
    tracer(stdout);
    tracer(stderr);
    stdio_init();
    tracer(stdin);
    tracer(stdout);
    tracer(stderr);

}
int main(){
    //test_etatInit();
    test_f_initialisation();
  
  return 0;
}
