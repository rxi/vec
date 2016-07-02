
SRCS = ./vec_wrap.h \
       ./vec_wrap.c \
       ../src/vec.c \
       ../src/vec.h

all: Properties

Properties: Main.hs vec_wrap.o vec.o
	ghc -O2 vec_wrap.o vec.o --make Main.hs -o $@

Main.hs: Properties.hsc $(SRCS)
	hsc2hs -o $@ $< -I ../src

vec.o: ../src/vec.c ../src/vec.h
	$(CC) -O2 -c ../src/vec.c -o $@

vec_wrap.o: vec_wrap.c vec_wrap.h
	$(CC) -O2 -c vec_wrap.c -o $@

clean:
	rm -f vec.o vec_wrap.o Main.hs Properties
