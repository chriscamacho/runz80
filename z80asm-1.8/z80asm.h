/* Z80 assembler by shevek

   Copyright (C) 2002-2007 Bas Wijnen <shevek@fmf.nl>
   Copyright (C) 2005 Jan Wilmans <jw@dds.nl>

   This file is part of z80asm.

   Z80asm is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Z80asm is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Z80ASM_H
#define Z80ASM_H

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>
#include <getopt.h>
#include <unistd.h>

/* defines which are not function-specific */
#ifndef BUFLEN
#define BUFLEN 300		/* size of readbuffer for file i/o */
#endif

#ifndef MAX_INCLUDE
#define MAX_INCLUDE 200		/* stack size for include command and macros */
#endif

/* types */
/* mnemonics. THESE MUST BE IN THE SAME ORDER AS const char *mnemonic[]! */
enum mnemonic
{
  CALL, CPDR, CPIR, DJNZ, HALT, INDR, INIR, LDDR, LDIR, OTDR, OTIR, OUTD,
  OUTI, PUSH, RETI, RETN, RLCA, RRCA, DEFB, DEFW, DEFS, DEFM,
  ADC, ADD, AND, BIT, CCF, CPD, CPI, CPL, DAA, DEC, EQU, EXX, INC, IND, INI,
  LDD, LDI, NEG, NOP, OUT, POP, RES, RET, RLA, RLC, RLD, RRA, RRC, RRD, RST,
  SBC, SCF, SET, SLA, SLL, SLI, SRA, SRL, SUB, XOR, ORG,
  CP, DI, EI, EX, IM, IN, JP, JR, LD, OR, RL, RR, DB, DW, DS, DM,
  INCLUDE, INCBIN, IF, ELSE, ENDIF, END, MACRO, ENDM, SEEK
};

/* types of reference */
enum reftype
{
  TYPE_BSR,			/* bit value (0-7) for bit, set and res */
  TYPE_DS,			/* ds reference (byte count and value) */
  TYPE_RST,			/* rst reference: val & 0x38 == val */
  TYPE_ABSW,			/* absolute word (2 bytes) */
  TYPE_ABSB,			/* absolute byte */
  TYPE_RELB,			/* relative byte */
  TYPE_LABEL			/* equ expression */
};

/* filetypes that can appear on the input. object files are on the todo list */
enum filetype
{
  FILETYPE_ASM
};

/* labels (will be malloced) */
struct label
{
  struct label *next, *prev;	/* linked list */
  int value;			/* value */
  int valid;			/* if it is valid, or not yet computed */
  int busy;			/* if it is currently being computed */
  struct reference *ref;	/* mallocced memory to value for computation */
  char name[1];			/* space with name in it */
};

/* files that were given on the commandline */
struct infile
{
  const char *name;
  enum filetype type;
};

/* filenames must be remembered for references */
struct name
{
  struct name *next, *prev;
  char name[1];
};

/* the include path */
struct includedir
{
  struct includedir *next;
  char name[1];
};

/* macro stuff */
struct macro_arg
{
  struct macro_arg *next;
  unsigned pos;
  unsigned which;
};

struct macro_line
{
  struct macro_line *next;
  char *line;
  struct macro_arg *args;
};

struct macro
{
  struct macro *next;
  char *name;
  unsigned numargs;
  char **args;
  struct macro_line *lines;
};

/* elements on the context stack */
struct stack
{
  const char *name;		/* filename (for errors). may be malloced */
  struct includedir *dir;	/* directory where it comes from, if any */
  FILE *file;			/* the handle */
  int line;			/* the current line number (for errors) */
  int shouldclose;		/* if this file should be closed when done */
  struct label *labels;		/* local labels for this stack level */
  /* if file is NULL, this is a macro entry */
  struct macro *macro;
  struct macro_line *macro_line;
  char **macro_args;		/* arguments given to the macro */
};

/* these structs will be malloced for each reference */
struct reference
{
  struct reference *next, *prev;
  enum reftype type;		/* type of reference */
  long oseekpos;		/* position in outfile for data */
  long lseekpos;		/* position in listfile for data */
  char delimiter;		/* delimiter for parser */
  int addr, line;		/* address and line of reference */
  int baseaddr;			/* address at start of line of reference */
  int comma;			/* comma when reference was set */
  int count;			/* only for ds: number of items */
  int infile;			/* index in infile[], current infile */
  int done;			/* if this reference has been computed */
  int computed_value;		/* value (only valid if done = true) */
  int level;			/* maximum stack level of labels to use */
  struct includedir *dir;	/* dirname of file (for error reporting) */
  char *file;			/* filename (for error reporting) */
  char input[1];		/* variable size buffer containing formula */
};

/* global variables */
/* mnemonics, used as argument to indx() in assemble */
extern const char *mnemonics[];

/* linked lists */
extern struct reference *firstreference;
extern struct label *firstlabel, *lastlabel;
extern struct name *firstname;
extern struct includedir *firstincludedir;
extern struct macro *firstmacro;

/* files */
extern FILE *realoutputfile, *outfile, *reallistfile, *listfile, *labelfile;
extern const char *realoutputfilename;
extern const char *labelfilename;
extern struct infile *infile;
/* prefix for labels in labelfile */
extern const char *labelprefix;
/* bools to see if files are opened */
extern int havelist, label;
/* number of infiles in array */
extern int infilecount;

/* number of errors seen so far */
extern int errors;

/* current line, address and file */
extern int addr, file;
/* current number of characters in list file, for indentation */
extern int listdepth;

/* use readbyte instead of (hl) if writebyte is true */
extern int writebyte;
extern const char *readbyte;
/* variables which are filled by rd_* functions and used later,
 * like readbyte */
extern const char *readword, *indexjmp, *bitsetres;

/* 0, 0xdd or 0xfd depening on which index prefix should be given */
extern int indexed;

/* increased for every -v option on the command line */
extern int verbose;

/* read commas after indx() if comma > 1. increase for every call */
extern int comma;

/* address at start of line (for references) */
extern int baseaddr;

/* set by readword and readbyte, used for new_reference */
extern char mem_delimiter;

/* line currently being parsed */
extern char *buffer;

/* if a macro is currently being defined */
extern int define_macro;

/* file (and macro) stack */
extern int sp;
extern struct stack stack[MAX_INCLUDE];	/* maximum level of includes */

/* Produce output even with errors.  */
extern int use_force;

/* print an error message, including current line and file */
void printerr (int error, const char *fmt, ...);

/* skip over spaces in string */
const char *delspc (const char *ptr);

int rd_expr (const char **p, char delimiter, int *valid, int level,
	     int print_errors);
int rd_label (const char **p, int *exists, struct label **previous, int level,
	      int print_errors);
int rd_character (const char **p, int *valid, int print_errors);

int compute_ref (struct reference *ref, int allow_invalid);

#endif
