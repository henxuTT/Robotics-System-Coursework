import serial.tools.list_ports
import matplotlib.pyplot as plt
import numpy as np


x = np.array((15, 25, 35, 45))
y = np.array((96.67, 83.33, 84.17, 53.33))
plt.grid(color='dimgrey', linestyle='-', linewidth=0.2)
plt.plot(x, y, color='blue', linewidth=1, linestyle='-', marker='o', markersize=8, label='Recognition rate changes with speed')
plt.xlabel('Car speed(PWM)', fontsize=13)
plt.ylabel('Recognition rate', fontsize=13)
plt.ylim((20, 100))
for a, b in zip(x, y):
    plt.text(a, b, b, ha='center', va='bottom', fontsize=13)
plt.legend(fontsize=14)
plt.show()