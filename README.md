# Memory-Management-System

Purpose:

The purpose of this lab is to learn and implement memory management system along with multithreading in linux and using pthreads for the same.

Note:
The program is configured to take maximum 5 threads. 
To show the results, 3 threads is used in the lab report.


Name of Source code files:
1) mms.cpp
2) memory_block.h

Use make to build the source code - mms.cpp
 
To run the executable files:
1) ./mms.o 3 1 	for fit type - first fit
2) ./mms.o 3 2	for fit type - best fit
3) ./mms.o 3 3	for fit type - worst fit

------------------GENERAL FORMAT---------------------------
1) ./mms.o <number of threads> <fit type>
number of threads = maximum 5
fit type = 1 for first fit
	2 for best fit
	3 for worst fit
