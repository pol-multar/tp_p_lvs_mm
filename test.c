#include"stdio.h"
#include <unistd.h>

int main(){
  int c;

  while((c=getchar())!=EOF)
    write(1,&c,1);
  /**
  while((c=getc(f))!=EOF)
    write(1,&c,1);
  */
  return 0;
}
