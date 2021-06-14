ltest: ltest.o lintpp.o lsint.o lint.o lint12.o lint1.o lstor.o lblock.o lfail.o lsrnd.o lsfio.o lsgcd.o lslcm.o
	cc -o ltest ltest.o lintpp.o lsint.o lint.o lint12.o lint1.o lstor.o lblock.o lfail.o lsrnd.o \
	lsfio.o lsgcd.o lslcm.o \
	-lstdc++ -lm

ltest.o: ltest.cpp
	cc -c ltest.cpp

lintpp.o: lintpp.cpp
	cc -c lintpp.cpp

lsint.o: lsint.c
	cc -c lsint.c

lint.o: lint.c
	cc -c lint.c

lint12.o: lint12.c
	cc -c lint12.c

lint1.o: lint1.c
	cc -c lint1.c

lstor.o: lstor.c
	cc -c lstor.c

lblock.o: lblock.c
	cc -c lblock.c

lfail.o: lfail.c
	cc -c lfail.c

lsrnd.o: lsrnd.c
	cc -c lsrnd.c

lsfio.o: lsfio.c
	cc -c lsfio.c

lsgcd.o: lsgcd.c
	cc -c lsgcd.c

lslcm.o: lslcm.c
	cc -c lslcm.c
