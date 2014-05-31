#include "stdio.h"

void copy(char* src, char* dest)
{
	int MAX=1000;
	char buffer[MAX];
	int n;
	FILE* fdin = fopen(src,"r");
	FILE* fdout = fopen(dest,"w");
	while((n = getc(fdin)) > 0) {
		putc(n, fdout);
	}
	fclose(fdin);
	fclose(fdout);
}
int main(int argc, char* argv[]){
	copy("copytest","copytest2");

}
