#include "stdio.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

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
    _IOB[0]._flag=_IOREAD | _IOFBF;
    _IOB[0]._file=0;

//Pour la compilation
    return 0;

}


//Il manque encore des tests dans cette fonction
// A chaque caractere lu on va dons _filbuf
int _filbuf(FILE* f){
    int c;
    int n=read(f->_file,&c,1);
    //f->_cnt=read(f->_file,f->_base,f->_bufsize)
    //Consomme le premier caractere du buffer
    //f->ptr=f->base+1
    //f->cnt--
    
    //if(f->_cnt==0)
    //return EOF;
    //else{
    //f->_cnt--;
    //return(f->ptr++);
    //} 
    return(n)?c:EOF;
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


