
#!/usr/bin/python
# -*- coding: UTF-8 -*-
from threading import Thread
import time
import serial
import serial.tools.list_ports
"""
class series(serial.Serial):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
"""

def ser_write(name, ser):
    while True:
        data = input("input data: ")
        if data == "exit":
            ser.write(data.encode()) #发送的类型必须是byte类型，所以需要encode
            ser.flush()
            time.sleep(1)
            print("exit write thread")
            break
        ser.write(data.encode())
        ser.flush()
        print("write data: ", data)
def ser_read(name,ser):
    while True:
        num = ser.in_waiting
        if num:
            bytes = ser.read(num)
            print("read data: ", bytes)
            if bytes.decode() == "exit":  #接收的类型是byte类型，所以需要decode 转成str
                time.sleep(1)
                print("exit read thread")
                break
        else:
            pass
        #print("read data: ", bytes)

if __name__ == "__main__":
    print("serial.VERSINON: ", serial.VERSION)
    info = serial.tools.list_ports.comports()
    print("info: ", info,type(info)) # list
    print("list port info: ")
    for item in info:
        print(item.device, item.name, item.description)

    port_name = input("input port name: ")

    ser = serial.Serial(port_name, baudrate=115200)
    #ser.open()


    ser_write_thread = Thread(target=ser_write,  args=("ser write thread",ser))
    ser_read_thread  = Thread(target=ser_read,   args=("ser read thread" ,ser))


    ser_write_thread.start()
    ser_read_thread.start()

    ser_write_thread.join()
    ser_read_thread.join()


    ser.close()
    print("exit main thread")
