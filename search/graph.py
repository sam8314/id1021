import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lx = [100, 200, 300, 400, 500, 600, 700, 800, 900, 
        1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
        100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000,1000000]
Lxsmooth = np.linspace(100,1000000,999000)
LyMin=[]
LyMean=[]
LyMax=[]
Lyapprox=[4*x**(1.005) for x in Lxsmooth]

dataMin = np.loadtxt("unsorted_min.txt", dtype=int)
dataMean = np.loadtxt("unsorted_mean.txt", dtype=int)
dataMax = np.loadtxt("unsorted_max.txt", dtype=int)

for i in range(37):
    LyMin.append(dataMin[i])
    LyMean.append(dataMean[i])
    LyMax.append(dataMax[i])

plt.figure()
plt.grid()
plt.suptitle("1000 search runtime in an unsorted array", fontsize='large',fontweight='bold')
plt.title("10 measurements are made per array size to find minimum, maximum and average time", fontsize="small")
plt.xlabel('array size', fontweight='bold')
plt.ylabel('total runtime in nanoseconds', fontweight='bold')
plt.plot(Lx,LyMin, ":", color="greenyellow", marker="2", label="best case")
plt.plot(Lx,LyMean, color="darkred", marker="2", label="average searchtime")
plt.plot(Lx,LyMax, ":", color="gold", marker="2", label="worst case")
plt.plot(Lxsmooth, Lyapprox, "--", color="olive", label='$f(x)=4\cdot x^{1.005}$')
plt.xscale("log")
plt.yscale("log")
plt.legend()

plt.savefig("unsorted_search.pdf")
plt.show()