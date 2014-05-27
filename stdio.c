#include "stdio.h"
#include <unistd.h>
#include <string.h>

struct _iobuf _IOB[10]=
  {{0,NULL,NULL,0,1,0},//stdin
   {...

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
/*
int main (){
  tracer(stdin);
  tracer(stdout);
  tracer(stderr);
  return 0;
  }*/
