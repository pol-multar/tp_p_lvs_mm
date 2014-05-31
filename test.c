#include"stdio.h"
#include <unistd.h>

void tracer(FILE *f);
int scanf(const char *format, ...);

/**
 * Test qui permet d'afficher l'état initial
 * des fichiers stdin, stdout et stderr
 */
void test_etatInit(void)
{
    tracer(stdin);
    tracer(stdout);
    tracer(stderr);
}

/**
 * Test qui permet de vérifier
 * la lecture de deux caractères (mais 3 entrés) 
 */
void test_lecture_2c(void){
    tracer(stdin);
    getchar();
    tracer(stdin);
    getchar();
    tracer(stdin);
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

void menu(int *boucle){
  int i;
  printf("\n*** Menu de test des fonctions implémentée dans stdio.c **\n\n");
  printf("Tapez le chiffre correspondant au test que vous voulez exécuter\n\n");
  printf("   0 -> Quitter le programme\n");
  printf("   1 -> Afficher l'état initial de stdin, stdout et stderr\n");
  printf("   2 -> Test qui permet de vérifier la lecture de 2 caractères\n");
  printf("   3 -> Test d'écriture sur la sortie standard\n");
  scanf("%d",&i);
  //printf("%d\n",i);
  if(i==0){
	*boucle=0;
  }

  switch(i)
	{
	case 0:
	  printf("\n\n  -->  Sortie du programme\n");
	  *boucle=0;
	  break;
	case 1:
	  test_etatInit();
	  break;
	case 2:
	  test_lecture_2c();
	  break;
	case 3:
	  test_ecritureStdout();
	  break;
	default:
	  printf("\n\nCe que vous avez rentré n'est attribué à aucun test pour le moment\n");
	  break;
	
	}
}

int main(){
  int boucle=1;
  while (boucle){
	menu(&boucle);
  }
  
  return 0;
}
