import serial.tools.list_ports
import matplotlib.pyplot as plt
import numpy as np


# x = np.array(i+1 for i in range(12))
y = np.array((75, 100, 22.5, 100, 95, 100, 75, 95, 55, 45, 90, 100))
labels = [1023, 1032, 1203, 1302, 2013, 2031, 2103, 2301, 3012, 3021, 3102, 3201]
plt.bar(range(len(y)), y, tick_label=labels)
plt.xlabel('Mark combination', fontsize=13)
plt.ylabel('Recognition rate(%)', fontsize=13)
plt.show()