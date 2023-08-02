import serial
import matplotlib.pyplot as plt
import time
import matplotlib
matplotlib.use('TkAgg')


# the port might be different on your PC
ser = serial.Serial('COM3', 115200, timeout=1)

ser.flushInput()

plot_window = 100
y_var = [0]*plot_window
x_var = list(range(plot_window))
fig, ax = plt.subplots()
line, = ax.plot(x_var, y_var)

while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = float(ser_bytes[0:len(ser_bytes) - 2].decode("utf-8"))
        print("Reading: ", decoded_bytes)  # print real time readings
        y_var.append(decoded_bytes)
        y_var = y_var[-plot_window:]
        line.set_ydata(y_var)
        ax.relim()
        ax.autoscale_view()
        fig.canvas.draw()
        plt.pause(0.01)  # add a short pause to allow GUI to update
        fig.canvas.flush_events()
    except:
        print("Keyboard Interrupt")
        break
