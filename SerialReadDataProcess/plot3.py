import serial.tools.list_ports
import time
import matplotlib.pyplot as plt
import numpy as np

ser = serial.Serial("COM5", 9600, timeout=0.5)
new_lines = []

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    ser.write("start".encode('utf-8'))
    time.sleep(3)
    lines = ser.readlines()
    for line in lines:
        line = str(line, 'UTF-8').strip('\r\n')
        new_lines.append(line)

    raw_index = new_lines.index('data_real_reading')
    count_index = new_lines.index('data_count')

    raw_data = new_lines[raw_index + 1:count_index]
    count_number = new_lines[count_index+1]

    raw_data = np.array(list(map(int, raw_data)))
    count_number = int(count_number)

    plt.grid(color='dimgrey', linestyle='-', linewidth=0.2)
    x = range(count_number+1)

    plt.plot(x, raw_data, "b-", linewidth=1)
    plt.plot((0, count_number), (1200, 1200), "g--", linewidth=0.8)
    plt.plot((0, count_number), (1900, 1900), "g--", linewidth=0.8)
    plt.plot((0, count_number), (2700, 2700), "g--", linewidth=0.8)
    plt.xlabel('Sensor reading times from start to stop', fontsize=13)
    plt.ylabel('Sensor reading values/STD values', fontsize=13)

    plt.legend(labels=['raw_data', 'Boundary of black/dark gray',  'Boundary of dark gray/light gray', 'Boundary of light gray/white'], fontsize=12)
    plt.show()





