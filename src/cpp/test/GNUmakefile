######################################################
#
# GNUmakefile to build the GEAR library examples
#
# @author F. Gaede, DESY
# @version $Id: GNUmakefile,v 1.2 2006-04-13 10:21:01 lippe Exp $
######################################################


# check for private compiler definition
ifdef MY_CXX
  CXX = $(MY_CXX)
else
  CXX = g++
endif

AR = ar


INSTALL = $(GEARBASE)/lib
BINDIR = $(GEARBASE)/bin
INCLUDE =  $(GEARINCLUDE) 
CPPFLAGS = $(GEARCPPFLAGS) $(INCLUDE) 


LIBS = -L$(INSTALL) -L$(SIOINSTALL)/lib  -lgearxml -lgear

BINS := $(patsubst %.cc,$(BINDIR)/%,$(wildcard *.cc))


bin: $(BINS)

clean:
	rm -f $(BINS)



$(BINDIR)/testgear: testgear.cc $(INSTALL)/libgear.a
	$(CXX) -o $(BINDIR)/testgear $(CPPFLAGS) testgear.cc  $(LIBS) 

$(BINDIR)/mergeXML: mergeXML.cc $(INSTALL)/libgear.a
	$(CXX) -o $(BINDIR)/mergeXML $(CPPFLAGS) mergeXML.cc  $(LIBS)

