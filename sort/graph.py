import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lx = [100, 200, 300, 400, 500, 600, 700, 800, 900,
        1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000]
Lxsmooth = np.linspace(100,90000,89900)
LyMin=[]
LyMean=[]
LyMax=[]
LyBest=[]
LyapproxN2=[x**(2) for x in Lxsmooth]
LyapproxN=[x for x in Lxsmooth]


dataMin = np.loadtxt("insertion_min.txt", dtype=int)
dataMean = np.loadtxt("insertion_mean.txt", dtype=int)
dataMax = np.loadtxt("insertion_max.txt", dtype=int)
dataBest = np.loadtxt("insertion_best.txt", dtype=int)

for i in range(27):
    LyMin.append(dataMin[i])
    LyMean.append(dataMean[i])
    LyMax.append(dataMax[i])
    LyBest.append(dataBest[i])

plt.figure()
plt.grid()
plt.suptitle("Time complexity of insertion sort algorithm in C", fontsize='large',fontweight='bold')
plt.title("5 measurements are made per array size to find minimum, maximum and average time", fontsize="small")
plt.xlabel('array size', fontweight='bold')
plt.ylabel('total runtime in nanoseconds', fontweight='bold')
plt.plot(Lx,LyMin, ":", color="mediumorchid", marker="2", label="minimum")
plt.plot(Lx,LyMean, color="sienna", marker="2", label="average searchtime")
plt.plot(Lx,LyMax, ":", color="darkturquoise", marker="2", label="maximum")
plt.plot(Lxsmooth, LyapproxN2, "--", color="red", label='$f(x)=x^{2}$')
plt.plot(Lx, LyBest, color="purple", marker="2", label='in sorted array')
plt.plot(Lxsmooth, LyapproxN, "--", color="steelblue", label='$f(x)=x$')
plt.xscale("log")
plt.yscale("log")
plt.legend()

plt.savefig("insertion_sort.pdf")
plt.show()