/***
 *
 * Re-ecriture des fonctions de base de stdio
 *
 *           Author: Erick Gallesio [eg@unice.fr]
 *    Creation date: 27-Oct-1992 09:28
 * Last file update: 11-May-2014 23:45 (eg)
 *
 */


/**** Ce qui suit est le fichier standard stdio.h de Sun OS 4.x
      (legerement modifie) ****/

# ifndef FILE
#define	BUFSIZ	1024
#define _SBFSIZ	8
extern	struct	_iobuf {
  int		_cnt;//Compteur ? Nbr de caractere dans le buffeur
  unsigned char*  _ptr;//emplacement disponible pour mettre un caractère
  unsigned char*  _base;//Qui alloue la base ?
  int		_bufsiz;//La taille du buffer sans doute
  short		_flag;//Union des valeurs precedente ?
  char		_file;//Char est un petit entier
} _IOB[];

#define _IOFBF	 0
#define	_IOREAD	 01
#define	_IOWRT	 02
#define	_IONBF	 04
#define	_IOMYBUF 010
#define	_IOEOF	 020
#define	_IOERR	 040
#define	_IOSTRG	 0100
#define	_IOLBF	 0200
#define	_IORW	 0400
#define	NULL	 ((void*) 0)
#define	FILE	 struct _iobuf
#define	EOF	 (-1)

#define	stdin	 (&_IOB[0])//pointeur sur _IOB[0]
#define	stdout	 (&_IOB[1])
#define	stderr	 (&_IOB[2])


extern int _flsbuf(unsigned char c, FILE *f);
extern int _filbuf(FILE * f);

#ifdef lint	/* so that lint likes (void)putc(a,b) */
extern int putc();
extern int getc();
#else
#define	getc(p)		(--(p)->_cnt>=0? ((int)*(p)->_ptr++):_filbuf(p))//si >=0 je renvoi le caractere courant (*ptr++) et j'incremente le pointeur de 1
//Sinon je lance _filebuf(p), fonction qui fait read(p->_file,p->bufsize,p->_base)
//ptr pointe sur base au depart
//ne pas oublier de mettre des conditions :
//(si j'ai pas de buffer, etc...)
//et donc d'allouer p->base
#define putc(x, p)	(--(p)->_cnt >= 0 ?\
	(int)(*(p)->_ptr++ = (unsigned char)(x)) :\
	(((p)->_flag & _IOLBF) && -(p)->_cnt < (p)->_bufsiz ?\
		((*(p)->_ptr = (unsigned char)(x)) != '\n' ?\
			(int)(*(p)->_ptr++) :\
			_flsbuf(*(unsigned char *)(p)->_ptr, p)) :\
		_flsbuf((unsigned char)(x), p)))
#endif
/*
 *trois cas possibles:
 *buffeurise ou pas
 *si je buffurise en neuw ligne, est-ce que le caractere lu est new line
 */
#define	getchar()	getc(stdin)
#define	putchar(x)	putc((x),stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)
#define	clearerr(p)	(void) ((p)->_flag &= ~(_IOERR|_IOEOF))


FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
int fclose(FILE *fp);

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

FILE *tmpfile(void);

int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
char *gets(char *s);

int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int puts(const char *s);

int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);

void setbuf(FILE *stream, char *buf);//Permet de mettre son propre buffeur pour les entrées sorties
int setvbuf(FILE *stream, char *buf, int mode, int size);
# endif
