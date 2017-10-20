mydiff: fbasicdiff.o fmyers.o ignoreopt.o dir.o sbs.o 
	cc fbasicdiff.o fmyers.o ignoreopt.o dir.o sbs.o -o mydiff
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
clean: 
	rm *.o
