# Wedit Makefile for project GLClock
SRCDIR=c:\lcc\projects\glclock
CFLAGS=-Ic:\lcc\include  -g2 
CC=c:\lcc\bin\lcc.exe
LINKER=c:\lcc\bin\lcclnk.exe
OBJS=\
	main.obj

LIBS=opengl32.lib glu32.lib glut32.lib

glclock.exe:	$(OBJS) Makefile
	$(LINKER)  -subsystem windows -o $(SRCDIR)\lcc\glclock.exe $(OBJS) $(LIBS)

# Build MAIN.C
MAIN_C=\
	$(SRCDIR)\mystuff.h\

main.obj: $(MAIN_C) $(SRCDIR)\main.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\main.c

link:
	$(LINKER)  -subsystem windows -o $(SRCDIR)\lcc\glclock.exe $(OBJS) $(LIBS)

