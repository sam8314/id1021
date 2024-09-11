import numpy as np
import matplotlib.pyplot as plt
from math import exp
import pylab

Lx = [
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 
    200, 300, 400, 500, 600, 700, 800, 900, 
    1000, 2000, 3000, 4000, 5000, 6000, 7000, 
    8000, 9000, 10000, 20000, 30000, 40000, 
    50000, 60000, 70000, 80000, 90000, 100000]

Ly1 = [
    15, 16, 10, 19, 15, 47, 35, 6, 44, 29, 
    24, 26, 37, 124, 73, 80, 6, 150, 
    98, 44, 47, 90, 169, 12, 12, 
    40, 9, 71, 29, 15, 113, 
    43, 98, 29, 100, 84, 441]

Ly2 = [
    329, 306, 190, 333, 205, 705, 617, 128, 715, 490, 
    349, 427, 437, 1170, 800, 848, 113, 1416, 
    1023, 483, 481, 890, 1521, 155, 155, 
    476, 156, 752, 372, 199, 1193, 
    790, 10875, 534, 1076, 899, 4303]

Llog=[0.01*x*np.log(x) for x in Lx]

 
fig, ax1 = plt.subplots() 
plt.grid()
 
color = 'tab:orange'
ax1.set_xlabel('array size') 
ax1.set_ylabel('call counts', color = color,fontweight='bold') 
ax1.plot(Lx, Ly1, ":",color = color, marker="1") 
ax1.set_xscale("log")
ax1.tick_params(axis ='y', labelcolor = color) 
 
# Adding Twin Axes to plot using dataset_2
ax2 = ax1.twinx() 
 
color = 'tab:purple'
ax2.set_ylabel('runtime in ns', color = color,fontweight='bold') 
ax2.plot(Lx, Ly2, color = color, marker="1") 
ax2.plot(Lx, Llog, "--", color="brown", label="f(n)=0.01nlog(n)")
ax2.set_xscale("log")
ax2.legend()
ax2.tick_params(axis ='y', labelcolor = color) 
 
# Adding title
plt.title('Recursive search runtime and call count in a sorted array', fontweight ="bold") 
 
# Show plot
plt.savefig("recursive_search.pdf")
plt.show()
