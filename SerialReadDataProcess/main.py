
import serial
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
    smooth_index = new_lines.index('data_smooth_reading')
    std_index = new_lines.index('data_std')
    count_index = new_lines.index('data_count')

    mode = 1
    if mode == 1:
        raw_data = new_lines[raw_index + 1:smooth_index]
        smooth_data = new_lines[smooth_index + 1:std_index]
        std_data = new_lines[std_index + 1:count_index]
        count_number = new_lines[count_index+1]

        raw_data = np.array(list(map(int, raw_data)))
        smooth_data = np.array(list(map(int, smooth_data)))
        std_data = np.array(list(map(int, std_data)))
        count_number = int(count_number)

        plt.grid(color='dimgrey', linestyle='-', linewidth=0.2)
        x = range(count_number+1)

        plt.plot(x, raw_data, "b-", linewidth=1)
        plt.plot(x, smooth_data, "r-", linewidth=1)
        plt.plot(x, std_data, color='orange', linestyle='-', linewidth=1.2)
        plt.plot((0, count_number), (100, 100), "g--", linewidth=0.8)
        plt.plot((0, count_number), (1200, 1200), color='lightcoral', linestyle='--', linewidth=0.8)
        plt.plot((0, count_number), (1900, 1900), color='indianred', linestyle='--', linewidth=0.8)
        plt.plot((0, count_number), (2700, 2700), color='red', linestyle='--', linewidth=0.8)
        plt.xlabel('Sensor reading times from start to stop')
        plt.ylabel('Sensor reading values/STD values')

        # plt.legend(labels=['raw_data', 'smooth_data',  'std', 'std threshold', 'Boundary of black/dark gray',
        #                    'Boundary of dark gray/light gray', 'Boundary of light gray/white'])
        plt.legend(labels=['raw_data', 'smooth_data', 'std', 'std threshold'])
        plt.show()
    else:
        count_number = new_lines[count_index + 1]
        count_number = int(count_number) + 1
        raw_data = new_lines[raw_index + 1:smooth_index]
        raw_data = np.array(list(map(int, raw_data)))
        std_data = [0 for j in range(count_number)]
        for i in range(count_number-4):
            std_data[i+4] = np.std(raw_data[i:i+4])

        raw_data = np.array(list(map(int, raw_data)))
        std_data = np.array(list(map(int, std_data)))

        plt.grid(color='dimgrey', linestyle='-', linewidth=0.2)
        x = range(count_number)

        plt.plot(x, raw_data, "b-", linewidth=1)
        plt.plot(x, std_data, color='orange', linestyle='-', linewidth=1.2)
        plt.plot((0, count_number), (100, 100), "g--", linewidth=0.8)
        plt.plot((0, count_number), (1200, 1200), color='lightcoral', linestyle='--', linewidth=0.8)
        plt.plot((0, count_number), (1900, 1900), color='indianred', linestyle='--', linewidth=0.8)
        plt.plot((0, count_number), (2700, 2700), color='red', linestyle='--', linewidth=0.8)
        plt.xlabel('Sensor reading times from start to stop')
        plt.ylabel('Sensor reading values/STD values')

        plt.legend(labels=['raw_data', 'std', 'std threshold', 'Boundary of black/dark gray',
                           'Boundary of dark gray/light gray', 'Boundary of light gray/white'])
        plt.show()





