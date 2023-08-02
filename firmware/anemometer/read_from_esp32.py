import requests
import time
import datetime
import os 

# Replace with your ESP's IP address
esp_ip = "192.168.142.240"  

def set_mode(mode):
    response = requests.get(f"http://{esp_ip}/mode/{mode}")
    print(response.text)

def get_pin_reads():
    response = requests.get(f"http://{esp_ip}/")
    timestamp, *pin_reads = response.text.split(',')
    pin_reads = [int(val) for val in pin_reads]
    return timestamp, pin_reads

# Use this function to change mode
# set_mode("pinread")  # Switch to pin read mode
set_mode("calibrate")  # Switch to pin read mode
time.sleep(2)

# Prepare the file for storing the readings
start_time = datetime.datetime.now()
filename = start_time.strftime("%Y%m%d%H%M%S") + ".csv"
file = open(filename, 'w')

# Now get the pin reads
try:
    while True:
        try:
            timestamp, pin_reads = get_pin_reads()
            print(f"Timestamp: {timestamp}, Pin Reads: {pin_reads}")
            file.write(f"{timestamp},{','.join(map(str, pin_reads))}\n")
            time.sleep(0.01)
        except requests.exceptions.RequestException as err:
            print ("OOps: Something Else Happened",err)
        except requests.exceptions.HTTPError as errh:
            print ("Http Error:",errh)
        except requests.exceptions.ConnectionError as errc:
            print ("Error Connecting:",errc)
        except requests.exceptions.Timeout as errt:
            print ("Timeout Error:",errt)
except KeyboardInterrupt:
    # Close the file and rename it to include the end time
    file.close()
    end_time = datetime.datetime.now()
    new_filename = start_time.strftime("%Y%m%d%H%M%S") + "-" + end_time.strftime("%Y%m%d%H%M%S") + ".csv"
    os.rename(filename, new_filename)
