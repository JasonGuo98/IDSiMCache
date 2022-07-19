import matplotlib.pyplot as plt
import numpy as np
from lib import c_MrcSim

ret_mrc = c_MrcSim.runMrcSim("./data/ARC_data/P1.lis", -1, 1024, 2, 10)

print(ret_mrc.shape)
print(ret_mrc)

cache_size = ret_mrc[:, 0]
cache_omrc = ret_mrc[:, 1]
cache_bmrc = ret_mrc[:, 2]
plt.plot(cache_size, cache_omrc)  # plot() 函数是绘制二维图形的最基本函数
plt.plot(cache_size, cache_bmrc)  # plot() 函数是绘制二维图形的最基本函数
plt.savefig('mrc.jpg')
