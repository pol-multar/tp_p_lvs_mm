#include "stdio.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

void tracer(FILE *f);
int snprintf(char *str, size_t size, const char *format, ...);

/**
 * Tableau _IOB de la structure définie comme externe dans stdio.h
 * Les trois premières cases contiennent respectivement :
 * stdin, stdout et stderr
*/

struct _iobuf _IOB[10]={{0,NULL,NULL,0,1,0},{0,NULL,NULL,0,1,1},{0,NULL,NULL,0,1,2}};


/*!
   \fn tracer : Fonction qui permet d'afficher les valeurs des différents champs de la structure_iobuf
   \param le FILE à tracer
   \return void
*/
void tracer(FILE *f)
{
    char buffer[500];
    snprintf(buffer, 500, "Trace FILE %p\n", f);
    write(2, buffer, strlen(buffer));
    snprintf(buffer, 500,
	     "\tcnt: %d, ptr: %p, base: %p, bufsiz: %d, flag:%x, file:%d\n\n",
	     f->_cnt, f->_ptr, f->_base, f->_bufsiz, f->_flag, f->_file);
    write(2, buffer, strlen(buffer));
}

/*!
   \fn _filbuf : Fonction qui remplit un buffer à partir d'un fichier
   \param le fichier a bufferiser
   \return le caractère lu, EOF si c'est terminé
*/
int _filbuf(FILE *f){
    int c=EOF;

/*
 *On regarde d'abord si le fichier est ouvert
 */
    if (!(f->_flag & (_IOREAD | _IOWRT | _IORW))) {
        errno = EBADF;
	return c;
    }

    if(!f -> _base){	
	  /*
	   * Il n'y a pas encore de buffer alloué pour ce fichier
	   * On va donc allouer de la mémoire pour le buffer :
	   */
	  f->_base = malloc(f -> _bufsiz);
	  f->_flag &= ~_IOMYBUF;//C'est moi qui alloue le buffer
	  if(!f->_base){//L'allocation a échouée
		errno = ENOMEM;
		return c;
	  }
    } else if(((int)f -> _cnt)>0){
	  /*
	   * Il y a un caractère non lu dans le buffer.
	   * On le retourne:
	   */
	  --f->_cnt;
	  c = *f->_ptr++;
	  return c;
    }

	/*
	 * On va flush le fichier si l'on est buffurisé par ligne
	 */

	if(f->_flag & _IOLBF){
	  fflush(f);
	}

	/*
	 * On remplit le buffer avec la prochaine partie du fichier
	 */
    if(f->_bufsiz){
	  f->_cnt = read(f->_file,(char *)(f->_ptr=f->_base),f->_bufsiz);
    }else{
	  /*
	   *Le buffer n'a pas de taille valide, il ne peut pas y avoir de lecture
	   *(Une valeur de 0 est mise par la fonction sscanf())
	   */
	  f->_cnt=0;
    }
	
    if(f->_cnt<0){ /* Il y a une erreur */
	  f->_flag |= _IOERR;
    }else if(!f->_cnt){/* On est en fin de fichier */
	  f->_flag|=_IOEOF;
    }else{/* Il y a des données valides lues depuis le fichier */
	  f->_flag&=~_IOEOF;
	  f->_cnt--;
	  c=*f->_ptr++;
    }
	return (int )c;

}

/*!
   \fn _flsbuf fonction utilisée dans putc pour ajouter un caractère dans le buffer. Elle flush le buffer.
   \param c : le caractère à ajouter, f le fichier ou ajouter le caractère 
   \return la valeur int de c si l'ajout est correct, int de EOF sinon.
*/

int _flsbuf(unsigned char c,FILE* f){
  int cpt;
  char tmp[1];

  if(f->_bufsiz <=1){
	/*
	 * Le fichier ouvert est non bufferisée, le caractère
	 * est directement écrit dans le fichier.
	 */
	f->_cnt=0;
	tmp[0]=c;
	if(write(f->_file,tmp,1)!=1){ /* Erreur lors de l'écriture */
	  f->_flag |= _IOERR;
	  return (int)EOF;
	}
	return (int)c;
  }

  if(!(f->_base)){
	/*
	 * Il n'y a pas de buffer encore alloué au fichier.
	 * On va donc créer le buffer en allouant dynamiquement
	 * la mémoire.
	 */
	f->_ptr = f->_base = malloc(f->_bufsiz);
	f->_cnt = f->_bufsiz;
	if(!f->_base){				/* Erreur lors de l'allocation */
	  errno = ENOMEM;
	  f->_flag |= _IOERR;
	  return (int)EOF;
	}
  }
  if(f->_ptr == f->_base + f-> _bufsiz){
	/*
	 * Le buffer est sans doute plein.
	 * Dans ce cas on l'écrit dans le fichier.
	 */
	if(write(f->_file,(char *)f->_base,f->_bufsiz)!=f->_bufsiz)
	  {
		f->_flag |= _IOERR;
		return (int)EOF;
	  }
	f->_ptr = f->_base;
	f->_cnt = f->_bufsiz;
  }
  if(f->_flag & _IOWRT || f->_flag & _IORW){
	/*
	 * Si on a l'accès en écriture sur le fichier, 
	 * alors le caractère peut être copié dans le buffer :
	 */
	*f->_ptr++=c;
	f->_cnt--;
  }

  if ((f->_flag & _IOLBF) == (_IOLBF)) { /* A vérifier */
	/*
	 * Si le fichier est bufferisé par ligne,
	 * nous devons chercher les '\n' pour savoir
	 * si le buffer doit être écrit
	 */
	if(c=='\n') {
	  cpt =((f->_ptr) - (f->_base));
	  if(write(f->_file,(char *)f->_base,cpt)!=cpt){
		f->_flag |= _IOERR;
		return (int)EOF;
	  }
	  f->_ptr = f-> _base;
	}
	f->_cnt=0;
  }
  return (int)c;
}

/* ###########################" */

FILE *fopen(const char *path, const char *mode){
	int fd;
	FILE *fil;

	fil = (FILE *)malloc(sizeof(FILE));
	if(fil == 0)//erreur
	{
		return NULL;
	}

	switch(*mode)
	{	//On mettra r pour read w pour write et z pour read/write
		case 'r': fd = open((char *)path, O_RDONLY,0644);
		 fil->_flag |= _IOREAD;
		 break;
		case 'w': fd = open((char *)path, O_WRONLY | O_CREAT | O_TRUNC,0644);
		 fil->_flag |= _IOWRT;
		 break;
		default  : fd = open((char *)path, O_RDWR | O_CREAT | O_TRUNC,0644);
		 fil->_flag |= _IORW;		 
	}


	if (fd == -1)	{	//erreur	
		return NULL;
	}

	

	//initialisation
	fil->_ptr = NULL;
    fil->_cnt = 0;
    fil->_base = NULL;
    fil->_flag = 0;
    fil->_file = fd;
    fil->_bufsiz =BUFSIZ;
    return fil;
}



void setbuf(FILE *stream, char *buf)
{
	if(buf==NULL) // Si le buffer est mis a NULL
	{
		//Cela veut dire que rien n'est bufferisé. (IONBF)
		setvbuf(stream,NULL,_IONBF,0);
	}
	else
	{	
		setvbuf(stream,buf,_IOFBF,BUFSIZ);	
	}
}

int setvbuf(FILE *stream, char *buf, int mode, int size)
{
	
	
	if(mode == _IOFBF || mode == _IOFBF)
	{
		stream->_bufsiz=size;
		stream->_cnt=0;
		stream->_base = buf;
		stream->_ptr=stream->_base;
	}
	else if(mode == _IONBF)
	{
		// On est en mode non bufferisé donc on prend un caractère comme buffer
		size = 1;
		buf = (char *)&stream->_file;
		stream->_bufsiz=size;
		stream->_cnt=0;
		stream->_base = buf;
		stream->_ptr=stream->_base;
	}
	return 0;
}

int fflush(FILE *fb)
{
	int cmp;
	if (!(fb->_flag & (_IOREAD | _IOWRT | _IORW))) {
		return -1; //erreur, on ne peut rien faire
	}
	if (!fb) {
		fb = stdout;
	}
	if (fb->_base && fb->_ptr != fb->_base) {
        
		if((cmp = fb->_ptr - fb->_base) > 0)
		{
		 	write(fb->_file, fb->_base, cmp);		 
		}
		fb->_ptr = fb->_base;
		fb->_cnt = 0;
	}

	return 0;
}

int fclose(FILE *fp)
{
	fflush(fp);
	close(fp->_file); // close du file descriptor
	  
	free(fp->_ptr);
	free(fp->_base);
	
	free(fp);
	return 0;
}


