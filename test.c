#include"stdio.h"
#include <unistd.h>

void tracer(FILE *f);
int scanf(const char *format, ...);
void _IOB_init(void); //A UTILISER A CHAQUE DEBUT DE PROGRAMME

void copy(int src, int dst){
  char ch;
  while(read(src,&ch,1)){
	write(dst,&ch,1);
  }
  printf("Fini\n");
}

void copy_2(FILE *fin)
{
  int c;
  while ((c = getc(fin)) != EOF){
    putchar(c);
  }
}

void test_cat(char *fichiers[])	/* Ne fonctionne pas correctement */
{
  if(! *fichiers){
	/* On affiche le fichier standard d'entree */
	printf("Je rentre dans le if\n");
	copy_2(stdin);
  }
  else{
	printf("Je rentre dans le else");
	/* Analyse de la ligne */
	for( ; *fichiers;fichiers++){
	  FILE *f;
	  if((f=fopen(*fichiers,"r"))==NULL){
		fprintf(stderr,"Je ne peux pas ouvrir %s\n",*fichiers);
	  }
	  else{
		copy_2(f);
		fclose(f);
	  }
	}
  }
}

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

void aff_menu(void){
  /* Ne pas oublier de rajouter l'option aussi dans menu */
   printf("\n*** Menu de test des fonctions implémentée dans stdio.c **\n\n");
   printf("Tapez le chiffre correspondant au test que vous voulez exécuter\n\n");
   printf("   0 -> Quitter le programme\n");
   printf("   1 -> Afficher l'état initial de stdin, stdout et stderr\n");
   printf("   2 -> Test qui permet de vérifier la lecture de 2 caractères\n");
   printf("   3 -> Test d'écriture sur la sortie standard\n");
   printf("   4 -> cat(stdin -> stdout)\n");
 }

void menu(int *boucle){
  /* Ne pas oublier de modifier aussi l'affichage lorsque vous rajoutez une option */
  int i;
  aff_menu();
  scanf("%d",&i);
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
	case 4:
	  //test_cat(NULL);
	  //copy(stdin->_file,stdout->_file);
	  printf("\n\n L'utilisation de ce test ne permet pas de revenir au menu, à la fin il faudra utiliser Ctrl-c\n\n");
	  printf("Vous pouvez commencer à taper :\n");
	  copy_2(stdin);
	  break;
	default:
	  printf("\n\nCe que vous avez rentré n'est attribué à aucun test pour le moment\n");
	  break;
	
	}
}

int main(){
  int boucle=1;
  
  _IOB_init();
  printf("%d\n",EOF);
  while (boucle){
	menu(&boucle);
  }
  
  return 0;
}
