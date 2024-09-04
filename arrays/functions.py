import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lxsmall = np.linspace(100, 150000)
Lxbig = np.linspace(126000, 1000000)

Lysearchsmall = [1.38*x**(1.02) for x in Lxsmall]
Lyduplicatesmall = [26.63*x**(0.77) for x in Lxsmall]

Lysearchbig=[1.38*x**(1.02) for x in Lxbig]
Lyduplicatebig=[26.63*x**(0.77) for x in Lxbig]

figure, axis = plt.subplots(1,2)

#fixed array size
axis[0].grid()
axis[0].set_title("Runtime at small scale", fontsize='large',fontweight='bold')
axis[0].set_xlabel('array size', fontweight='bold')
axis[0].set_ylabel('runtime in nanoseconds', fontweight="bold")
axis[0].plot(Lxsmall,Lysearchsmall, color="blue", label="item search")
axis[0].plot(Lxsmall, Lyduplicatesmall,color="red", label='duplicate search')
axis[0].legend()

#grow size
axis[1].grid()
axis[1].set_title("Runtime at big scale", fontsize='large',fontweight='bold')
axis[1].set_xlabel('array size', fontweight='bold')
axis[1].plot(Lxbig,Lysearchbig, color="blue", label="item search")
axis[1].plot(Lxbig, Lyduplicatebig,color="red", label='duplicate search')
axis[1].set_xscale("log")
axis[1].legend()

plt.savefig("function_graphs.pdf")
plt.show()