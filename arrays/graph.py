import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lxfixed = [100, 200, 300, 400, 500, 600, 700, 800, 900, 
        1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
        100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000,
        1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000,10000000]
Lxsmoothfixed = np.linspace(100,10000000,9990000)
L175=[175 for x in Lxsmoothfixed]
Lyfixed=[]

Lxgrow = [100, 200, 300, 400, 500, 600, 700, 800, 900, 
        1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,100000]
Lxsmoothgrow=np.linspace(100,100000,99900)
Lygrow=[]
Lapprox = [1.38*x**(1.02)/1000 for x in Lxsmoothgrow]

dataFixed = np.loadtxt("item_search.txt", dtype=int)
dataGrow = np.loadtxt("item_search_size.txt", dtype = int)

for i in range(46):
    Lyfixed.append(dataFixed[i])
for i in range(28):
    Lygrow.append(dataGrow[i]/1000)

figure, axis = plt.subplots(1,2)

#fixed array size
axis[0].grid()
axis[0].set_title("Runtime of multiple item searches per search", fontsize='large',fontweight='bold')
axis[0].set_xlabel('number of searches in array with 100 items', fontweight='bold')
axis[0].set_ylabel('runtime per search in nanoseconds', fontweight='bold')
axis[0].plot(Lxfixed,Lyfixed, color="orange", marker="1")
axis[0].plot(Lxsmoothfixed, L175, "--",color="blue", label='175 ns')
axis[0].set_xscale("log")
axis[0].legend()

#grow size
axis[1].grid()
axis[1].set_title("Runtime of 100 000 searches per search", fontsize='large',fontweight='bold')
axis[1].set_xlabel('size of the array', fontweight='bold')
axis[1].set_ylabel('runtime per search in microseconds', fontweight='bold')
axis[1].scatter(Lxgrow,Lygrow, color="purple", marker="x")
axis[1].plot(Lxsmoothgrow, Lapprox, "--",color="orange", label='$f(x)=1.38x^{1.02}$')
axis[1].set_xscale("log")
axis[1].legend()

plt.savefig("item_search.pdf")
plt.show()