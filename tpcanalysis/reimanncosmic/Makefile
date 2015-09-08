CC=gcc
CXX=g++ 
RM=rm -f
PREFIX=/cache/scr/spirit/spiritroot_repo/
CPPFLAGS= -g -Wall `root-config --cflags` -I${PREFIX}format -I${PREFIX}reco -I${PREFIX}reco/RiemannTracking
INC=`locate *ST*.hh|grep /cache/` 
LDFLAGS=`root-config --ldflags`
LDLIBS=`root-config --libs` -L/cache/scr/spirit/spiritroot_repo/build/lib -lSTFormat -lrt -lSTReco
EXEC=field
SRCS=fieldcorrection.cc 
OBJS=$(subst .cc,.o,$(SRCS))

all: $(EXEC)

$(EXEC): 
	$(CXX)-o $(EXEC) $(SRCS) $(CPPFLAGS) $(LDFLAGS)  $(LDLIBS) 

clean:
	$(RM) $(OBJS)
	$(RM) field
dist-clean: clean
	$(RM) field