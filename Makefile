GCC=g++
ARGS=-Wall

build: myVim

myVim: MyVim.cpp Command.cpp Stack.cpp Queue.cpp
	$(GCC) $^ -o myVim $(ARGS) 

clean:
	rm -rf *.o myVim
