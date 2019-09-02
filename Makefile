INC_DIR:= include/ 
SRC_DIR:= src/
SRCS:=$(wildcard src/*.cc) $(wildcard src/net/*.cc) $(wildcard src/threadpool/*.cc) 
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -llog4cpp -lpthread -lhiredis

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I , $(INC_DIR)) $(LIBS) 

EXE:=bin/SearchEngine.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
