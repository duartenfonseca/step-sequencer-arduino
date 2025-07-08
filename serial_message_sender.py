import serial
import threading
import time

ser = serial.Serial("COM3", baudrate=9600, bytesize=8, parity='N',dsrdtr=True, timeout=1)
mutex = threading.Lock()

def read_function():
    while True:
        with mutex:
            if ser.isOpen():
                input_data = ser.readline().strip().decode("utf-8")
                print(input_data)

if __name__ == "__main__":
    thread = threading.Thread(target=read_function, daemon=True)
    thread.start()
    while True:
        if input("Press A to send message...") == "a":   
            mutex.acquire()
            print("Main thread is running")
            val = input("Enter your value: ")
            print(val)
            if ser.isOpen():
                msg = [0x3C,0x11,0xff,0xa6,0x3e]
                ser.write(msg)
            mutex.release()
