import matplotlib.pyplot as plt
import numpy as np
from lib import c_MrcSim

ret_mrc = c_MrcSim.runMrcSim("./data/zipf_trace.txt", 10000, 1, 2, 10)

print(ret_mrc.shape)
print(ret_mrc)

cache_size = ret_mrc[:, 0]
cache_omrc = 1 - ret_mrc[:, 1]
cache_bmrc = 1 - ret_mrc[:, 2] + 0.05
plt.plot(cache_size, cache_omrc)  # plot() 函数是绘制二维图形的最基本函数
plt.plot(cache_size, cache_bmrc)  # plot() 函数是绘制二维图形的最基本函数
plt.savefig('mrc.jpg')
