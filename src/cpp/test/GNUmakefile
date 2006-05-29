######################################################
#
# GNUmakefile to build the GEAR library examples
#
# @author F. Gaede, DESY
# @version $Id: GNUmakefile,v 1.4 2006-05-29 13:03:18 musat Exp $
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


LIBS = -L$(INSTALL) -L$(SIOINSTALL)/lib  -lgearxml 

ifdef GEAR_USE_CGA
include $(GEARBASE)/src/cpp/gearcga/src/GNUmakefile.cga
LIBS += -lgearcga
endif

LIBS += -lgear

ifdef GEAR_USE_CGA
LIBS += $(CGALIBS)
CPPFLAGS += -DCGA
endif

#----- testong tpcproto ---------
ifdef GEAR_USE_AIDA
  CPPFLAGS += -D GEAR_USE_AIDA `aida-config --include`
  LIBS += `aida-config --lib`
  LIBS += `root-config --glibs`
endif
#--------------------------------


BINS := $(patsubst %.cc,$(BINDIR)/%,$(wildcard *.cc))


bin: $(BINS)

clean:
	rm -f $(BINS)



$(BINDIR)/testgear: testgear.cc $(INSTALL)/libgear.a
	$(CXX) -o $(BINDIR)/testgear $(CPPFLAGS) testgear.cc  $(LIBS) 

$(BINDIR)/mergeXML: mergeXML.cc $(INSTALL)/libgear.a
	$(CXX) -o $(BINDIR)/mergeXML $(CPPFLAGS) mergeXML.cc  $(LIBS)




$(BINDIR)/testtpcproto: testtpcproto.cc $(INSTALL)/libgear.a
	$(CXX) -o $(BINDIR)/testtpcproto $(CPPFLAGS) testtpcproto.cc  $(LIBS) 
