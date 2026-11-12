import serial
import csv
import time

# Change COM5 to your XIAO's actual COM port
ser = serial.Serial("COM6", 115200, timeout=1)

# Allow the board to restart after opening serial
time.sleep(2)

filename = "e_nose_data.csv"

header = [
    "ms", "temp_C", "rh_pct", "co2_ppm",
    "h2_ppm", "ethanol_ppm", "acetone_dV",
    "acetone_present", "acetone_ready", "chamber_clear"
]

print("Recording sensor data. Press Ctrl+C to stop.")

try:
    with open(filename, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(header)

        while True:
            line = ser.readline().decode(
                "utf-8", errors="replace"
            ).strip()

            if not line:
                continue

            values = line.split(",")

            # Only save complete 10-column measurement rows
            if len(values) == 10 and values[0].isdigit():
                writer.writerow(values)
                file.flush()
                print(line)

except KeyboardInterrupt:
    print("\nRecording stopped.")

finally:
    ser.close()