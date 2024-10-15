
# uncomment the following line if you are defining USE_STD_MUTEX in porting.h
#CPPFLAGS += -std=c++11
 
EDConsole: EDConsole.o EDDevice.o EDDeviceTCP.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

EDConsole.o:  EDConsole.cpp porting.h exceptions.h EDDeviceTCP.h EDDevice.h
EDDevice.o:  EDDevice.cpp EDDevice.h exceptions.h
EDDeviceTCP.o:  EDDeviceTCP.cpp EDDeviceTCP.h EDDevice.h porting.h exceptions.h

dist-tgz: ../../EDConsole-src.tar.gz

../../EDConsole-src.tar.gz: README.txt LICENSE VERSION Makefile $(wildcard *.cpp *.h)
	tar czf $@ $^