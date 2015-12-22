import os

numOfBodies = [100,200,400,1000,2000,5000,10000]
numOfThreads = [1,2,4,8,10,12,14,15,16]
times = 10 
for body in numOfBodies:
    for thread in numOfThreads:
            line = "./NbodySimulationNie " + str(body) + " " + str(thread)
            for i in range(0,times):
                    if os.system(line) != 0:
                            print 'Something wrong'
                    else:
                            print 'end for body=%d,thread=%d'%(body,thread)
print 'end simulation!'


