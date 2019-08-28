SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o,$(SRCS))
ELF:=main
CC:=g++
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	g++ -o $@ $^ -lboost_regex
clean:
	rm -rf $(OBJS) $(ELF)
