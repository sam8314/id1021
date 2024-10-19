import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab


Lx=np.linspace(1,9675, 9675)
Lytime20000=[]
Lycheck20000=[]
Lytime50000=[]
Lycheck50000=[]
L34=[34 for x in Lx]


dataBench20000 = np.loadtxt("bench_prob_look20000.txt", dtype=int)
dataCheck20000 = np.loadtxt("bench_prob_check20000.txt", dtype=int)

dataBench50000 = np.loadtxt("bench_prob_look50000.txt", dtype=int)
dataCheck50000 = np.loadtxt("bench_prob_check50000.txt", dtype=int)

for i in range(9675):
    Lytime20000.append(dataBench20000[i])
    Lycheck20000.append(dataCheck20000[i])

    Lytime50000.append(dataBench50000[i])
    Lycheck50000.append(dataCheck50000[i])

fig, ax1 = plt.subplots()
plt.grid()

color = 'tab:orange'
ax1.set_xlabel('number of tests')
ax1.set_ylabel('additional checks', color = color,fontweight='bold')
ax1.scatter(Lx, Lycheck20000, color=color, marker="2", label="20000")
ax1.scatter(Lx, Lycheck50000, color="yellow", marker="2", label="50000")
ax1.legend(loc = 'upper left')
ax1.tick_params(axis ='y', labelcolor = color)

# Adding Twin Axes to plot using dataset_2
ax2 = ax1.twinx()

color = 'tab:purple'
ax2.set_ylabel('runtime in ns', color = color,fontweight='bold')
ax2.plot(Lx, Lytime20000, color="purple", label="20000")
ax2.plot(Lx, Lytime50000, color="blue", label="50000")
#ax2.set_xscale("log")
ax2.legend()
ax2.tick_params(axis ='y', labelcolor = color)

# Adding title
plt.title('search runtime and number of additional checks using probing for two different array sizes (extract)', fontweight ="bold")

plt.show()
