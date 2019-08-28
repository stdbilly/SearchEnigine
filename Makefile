INC_DIR1:= ../../include/ ../../include/cppjieba/ ../../include/limonp
SRC_DIR1:= ./
SRCS1:=$(wildcard *.cc) $(wildcard ../Configuration.cc)   
OBJS1:= $(patsubst %.cc, %.o, $(SRCS1))
LIBS:= -lpthread

CXX:=g++

CXXFLAGS1:= -w -g  $(addprefix -I, $(INC_DIR1)) $(LIBS) 

EXE1:=../../bin/offline.exe

$(EXE1):$(OBJS1)
	$(CXX) -o $(EXE1) $(OBJS1) $(CXXFLAGS1)

clean:
	rm -rf $(EXE1)
	rm -rf $(OBJS1)
