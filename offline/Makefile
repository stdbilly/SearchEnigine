INC_DIR:= include/ 
SRC_DIR:= src/
SRCS:=$(wildcard src/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:=-lpthread -lboost_regex 

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I , $(INC_DIR)) $(LIBS) 

EXE:=bin/offline.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
