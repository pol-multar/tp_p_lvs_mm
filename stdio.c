#include "stdio.h"
#include <unistd.h>
#include <string.h>

//struct _iobuf _IOB[10]=
//  {{0,NULL,NULL,0,1,0},//stdin
//   {...

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

}


FILE *fopen(const char *path, const char *mode)
{
	FILE *file;
	file = malloc(sizeof(FILE));
	if(	
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
	
	stream->_flag = _bufsiz | _base | _ptr | _cnt;
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

static int fflush()
{
	if((cmp = stream->ptr - stream->base) > 0)
	{
		 written = write(stdout, stream->_base, cmp);		 
	}
	stream->ptr = stream->base;
	stream->cnt = 0;
}

/*
int main (){
  tracer(stdin);
  tracer(stdout);
  tracer(stderr);
  return 0;
  }*/
