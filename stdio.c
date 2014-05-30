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
   \fn tracer 
   \param le FILE à tracer
   \return 
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
   \fn Fonction qui initialise _IOB
   \param aucun
   \return 0 si tout s'est bien passé
*/


int stdio_init(void){
/**
* On alloue la mémoire et on initialise la structure
*/

/**
 * if((_IOB =_stdio_create(sizeof(struct _iobuf)))==NULL){
 *	return errno;
 *	  }
 */

/* Initialisation de stdin */
    _IOB[0]._cnt=0;
    _IOB[0]._ptr =malloc(BUFSIZ);
    _IOB[0]._base=_IOB[0]._ptr;
    _IOB[0]._bufsiz=BUFSIZ;
    _IOB[0]._file=0;
    _IOB[0]._flag=_IOREAD | _IOFBF;


/* Initialisation de stdout */
    _IOB[1]._cnt=0;
    _IOB[1]._ptr =NULL;
    _IOB[1]._base=NULL;
    _IOB[1]._bufsiz=BUFSIZ;
    _IOB[1]._flag=_IOWRT | _IOFBF;
    _IOB[1]._file=1;

/* Initialisation de stderr */
    _IOB[2]._cnt=0;
    _IOB[2]._ptr =NULL;
    _IOB[2]._base=NULL;
    _IOB[2]._bufsiz=BUFSIZ;
    _IOB[2]._flag=_IOWRT | _IOFBF;
    _IOB[2]._file=2;



/* Si l'initialisation s'est bien déroulée */
    return 0;

}


//Il manque encore des tests dans cette fonction
// A chaque caractere lu on va dons _filbuf
int _filbuf(FILE* f){
    int c=EOF;

/*
 *On regarde d'abord si le fichier est ouvert
 */
    if (!(f->_flag & (_IOREAD | _IOWRT | _IORW))) {
        errno = EBADF;
	return c;
    }

    if(!f->_base){	
/*
 * Il n'y a pas encore de buffer alloué pour ce fichier
 * On va donc alloué de la mémoire pour le buffer 
 */
	f->_base = malloc(f->_bufsiz);
	if(!f->_base){//L'allocation a échouée
	    errno=ENOMEM;
	    return c;
	}
    }else if(((int)f->_cnt)>0){
/*
 * Il y a un caractère non lu dans le buffer.
 * On le retourne
 */
	--f->_cnt;
	c = *f->_ptr++;
	return c;
    }

/*
 * On remplit le buffer avec la prochaine partie du fichier
 */

    f->_flag&=~_IOMYBUF;		/* A vérifier */
    if(f->_bufsiz){
	f->_cnt=read(f->_file,(char *)(f->_ptr=f->_base),f->_bufsiz);
    }else{
/*
 *Le buffer n'a pas de taille valide, il ne peut pas y avoir de lecture
 */
	f->_cnt=0;
    }
    if(f->_cnt<0){ /* Il y a une erreur */
	f->_flag |= _IOERR;
    }else if(!f->_cnt){/* On est en fin de fichier */
	f->_flag|=_IOEOF;
    }else{/* Il y a des données lues depuis le fichier */
	f->_flag&=~_IOEOF;
	f->_cnt--;
	c=*f->_ptr++;
    }

    return c;
}

/**
 * La fonction _flsbuf utilisée (entre autre) dans putc
 */
int _flsbuf(unsigned char c,FILE* f){
	return 0;
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
	
	
	if(mode == _IOFBF)
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


