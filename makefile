#compiler of c: gcc
CC = g++
#compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -fopenmp -O2

#default: NbodySimulation
#NbodySimulation: NbodySimulation.o
 #       $(CC) $(CFLAGS) -o NbodySimulation NbodySimulation.o
#NbodySimulation.o: NbodySimulation.c
 #       $(CC) $(CFLAGS) -c NbodySimulation.c
#clean:
 #       $(RM) NbodySimulation *.o *~
#defalut:dySimulation1
NbodySimulationNie: NbodySimulationNie.o
	$(CC) $(CFLAGS) -o NbodySimulationNie NbodySimulationNie.o
NbodySimulationNie.o: NbodySimulationNie.c
	$(CC) $(CFLAGS) -c NbodySimulationNie.c
clean:
	$(RM) NbodySimulationNie *.o *~

