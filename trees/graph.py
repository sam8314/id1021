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


LyLOG=[10*np.log(n) for n in Lxsmooth]
Lybinary=[]

dataAddUnB = np.loadtxt("add_unb.txt", dtype=int)
dataAddB = np.loadtxt("add_b.txt", dtype=int)
dataLookUnB=np.loadtxt("look_unb,txt", dtype=int)
dataLookB=np.loadtxt("look_b.txt", dtype=int)


for i in range(46):
    LyAddUnB.append(dataAddUnB[i])
    LyAddB.append(dataAddB[i])
    LyLookUnB.append(dataLookUnB[i])
    LyLookB.append(dataLookB[i])

plt.figure()
plt.grid()
plt.title("Time complexity of adding and searching in balanced and unbalanced trees", fontsize='large',fontweight='bold')
plt.suptitle("for each size of tree, 10 measurements are done to extract min, max, average", fontsize="small")
plt.xlabel('number of nodes', fontweight='bold')
plt.ylabel('total runtime in nanoseconds', fontweight='bold')

plt.scatter(Lx,Lylookmin, color="#0D6986", marker="2",)
plt.plot(Lx,Lylookmean, color="#0D6986", marker="2", label="lookup")
plt.scatter(Lx,Lylookmax, color="#0D6986", marker ="2")

plt.scatter(Lx, Lyaddmin, color="#FFA15C", marker="3")
plt.plot(Lx, Lyaddmean, color="#FFA15C", marker="3", label="add")
plt.scatter(Lx, Lyaddmax, color="#FFA15C", marker="3")

#plt.plot(Lxsmooth, LyN, "--", color ="red", label="$4n$")
plt.plot(Lxsmooth, LyLOG, "--", color ="red", label="$160log(n)$")

plt.xscale("log")
plt.yscale("log")
plt.legend()

#plt.savefig("bench1.pdf")
plt.show() 
