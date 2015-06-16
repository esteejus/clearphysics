gastable: makegasfile.C
	g++ -c -O3 `root-config --cflags` -I$(GARFIELD_HOME)/Include makegasfile.C
	g++ `root-config --glibs` -lgfortran -lm -o makegasfile makegasfile.o $(GARFIELD_HOME)/Library/libGarfield.a
	rm makegasfile.o
