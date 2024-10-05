import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lx = [100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
                    100000]
Lxsmooth = np.linspace(100,100000,99900)
LyEnmin=[]
LyEnmean=[]
LyEnmax=[]

LyDemin=[]
LyDemean=[]
LyDemax=[]

Lyapprox1=[75 for n in Lxsmooth]
Lyapprox2=[25 for n in Lxsmooth]

dataEnqBenchMin = np.loadtxt("bench2_enque_min.txt", dtype=int)
dataEnqBenchMean = np.loadtxt("bench2_enque_mean.txt", dtype=int)
dataEnqBenchMax = np.loadtxt("bench2_enque_max.txt", dtype=int)
dataDeqBenchMin = np.loadtxt("bench2_deque_min.txt", dtype=int)
dataDeqBenchMean = np.loadtxt("bench2_deque_mean.txt", dtype=int)
dataDeqBenchMax = np.loadtxt("bench2_deque_max.txt", dtype=int)

for i in range(28):
    LyEnmin.append(dataEnqBenchMin[i])
    LyEnmean.append(dataEnqBenchMean[i])
    LyEnmax.append(dataEnqBenchMax[i])

    LyDemin.append(dataDeqBenchMin[i])
    LyDemax.append(dataDeqBenchMax[i])
    LyDemean.append(dataDeqBenchMean[i])

plt.figure()
plt.grid()
plt.title("for each size, 20 measurements are done to get average", fontsize='small')
plt.suptitle("Time complexity of enqueing and dequeuing a queue with head and tail", fontsize='large',fontweight='bold')
plt.xlabel('number of nodes', fontweight='bold')
plt.ylabel('total runtime in nanoseconds cropped to 200', fontweight='bold')

plt.plot(Lx,LyEnmean, color="darkgoldenrod", marker="3", label="enqueuing average")
plt.plot(Lx,LyDemean, color="mediumorchid", marker="3", label="dequeuing average")

#plt.plot(Lxsmooth, Lyapprox1, "--", color ="orangered", label="$75$")
#plt.plot(Lxsmooth, Lyapprox2, "--", color ="darkcyan", label="$25$")
plt.ylim(0,200)
plt.xscale("log")
#plt.yscale("log")
plt.legend()

plt.show() 
