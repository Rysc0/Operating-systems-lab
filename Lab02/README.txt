Main codes are:
- Two_threads.cpp
- milic_says.cpp

Two_threads.cpp is code that uses two threads to operate on global memory.
First thread is generating X random numbers (X - amount of numbers to generate
- first argument).
Second thread is calculating sum of generated numbers.
Second argument is number that's determining how many times numbers should be
generated and calculated.

milic_says.cpp is implementation of Lamport's backery algorithm using processes.
It uses one input argument to determine how many processes should be created.


Waiting for response about shared memory segments. Currently I use 2 shared
memory segments and I should use only 1. Didn't found suitable solution with 1
segment so I'll be fixing bugs and cleaning the code while waiting for response.
