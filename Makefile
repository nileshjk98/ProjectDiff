all: project patch
project: fbasicdiff.o fmyers.o ignoreopt.o dir.o sbs.o context.o 
	cc -Wall fbasicdiff.o fmyers.o ignoreopt.o dir.o sbs.o context.o -o project
fbasicdiff.o: fbasicdiff.c diff.h
	cc -c fbasicdiff.c
fmyers.o: fmyers.c diff.h
	cc -c fmyers.c
ignoreopt.o: ignoreopt.c diff.h
	cc -c ignoreopt.c
dir.o: dir.c diff.h
	cc -c dir.c
sbs.o: sbs.c diff.h
	cc -c sbs.c
context.o: context.c diff.h
	cc -c context.c
patch: patch.c
	cc -Wall patch.c -o patch
clean: 
	rm *.o
