import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lx = [100, 200, 300, 400, 500, 600, 700, 800, 900,
        1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
        100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000,
        1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000,
        10000000]
Lxsmooth = np.linspace(100,10000000,99999)
Ly1=[]
Ly2=[]
LyN=[4*n for n in Lxsmooth]
LyLOG=[4610 for n in Lxsmooth]


dataBench1 = np.loadtxt("bench1.txt", dtype=int)
dataBench2 = np.loadtxt("bench2.txt", dtype=int)

for i in range(46):
    Ly1.append(dataBench1[i])
    Ly2.append(dataBench2[i])

plt.figure()
plt.grid()
plt.title("Time complexity of append() in linked list C", fontsize='large',fontweight='bold')
plt.xlabel('number of cells', fontweight='bold')
plt.ylabel('total runtime in nanoseconds', fontweight='bold')

plt.plot(Lx,Ly1, color="#FFA15C", marker="2", label="benchmark 1")
plt.plot(Lx,Ly2, color="#0D6986", marker="2", label="benchmark 2")
plt.plot(Lxsmooth, LyN, "--", color ="red", label="$4n$")
plt.plot(Lxsmooth, LyLOG, "--", color ="blue", label="$4610 (constant)$")
#plt.xscale("log")
plt.yscale("log")
plt.legend()

#plt.savefig("bench1.pdf")
plt.show() 
