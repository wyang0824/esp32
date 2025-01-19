
#!/usr/bin/python
# -*- coding: UTF-8 -*-
from threading import Thread
from threading import Event
from threading import Lock
from queue import Queue
import time
import serial
import serial.tools.list_ports

exit_event = Event()
recv_event = Event()
serial_lock = Lock()

def my_serial_write(name, ser,input_queue,output_queue):
    while not exit_event.is_set():
        try:
            user_input = input_queue.get(timeout=0.1)  # 从队列中获取用户输入
            if user_input.lower() == "exit":
                exit_event.set()
                break
            with serial_lock:
                ser.write(user_input.encode('utf-8'))
                print(f"发送的数据为: {user_input}")
            # output_queue.put(f"发送的数据为: {user_input}")
        except Exception as e:
            pass

def my_serial_read(name,ser,output_queue):
    while not exit_event.is_set():
        if ser.in_waiting:
            with serial_lock:
                read_data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
            print(f"读取的数据为: {read_data}")
            recv_event.clear()
            # output_queue.put(f"读取的数据为: {read_data}")
        time.sleep(0.1)  # 避免CPU占用过高


def msg_output(name,output_queue):
    while not exit_event.is_set() or not output_queue.empty():
        try:
            # 非阻塞读取队列中的消息
            message = output_queue.get(timeout=0.1)
            print(message)
        except:
            pass

def msg_input(name,input_queue):
    """输入线程：获取用户输入"""
    while not exit_event.is_set():
        try:
            if not recv_event.is_set():
                user_input = input("请输入您要发送的数据,输入exit退出: ")
                recv_event.set()
                input_queue.put(user_input)
                if user_input.lower() == 'exit':
                    exit_event.set()
        except KeyboardInterrupt:
            exit_event.set()



if __name__ == "__main__":
    print("当前pyserial的版本为:V%s" % (serial.VERSION))
    list_comports = serial.tools.list_ports.comports()
    print(type(list_comports))
    print("当前电脑中存在的串口号如下:")
    for item in list_comports:
        print(item.device,item.name,item.description)
    

    port_name = input("请选择您要使用的串口号(如COM12):")
    is_have_this_port = False
    for item in list_comports:
        if port_name == item.device:
            is_have_this_port = True
            break
    if is_have_this_port == False:
        print("没有此串口号请重新输入!!!")

    my_serial = serial.Serial(port_name, baudrate=115200)

    print(f"串口{my_serial.name}打开成功") if my_serial.is_open else print(f"串口{my_serial.name}打开失败")
    if my_serial.is_open:
        print("开始创建串口读写线程....")
        
        # 创建队列用于跨线程输出
        output_queue = Queue()
        input_queue  = Queue()

        threads = [
        Thread(target=my_serial_write,  args=("my serial write thread",my_serial,input_queue,output_queue)),
        Thread(target=my_serial_read,  args=("my serial write thread",my_serial,output_queue)),
        Thread(target=msg_output, args=("output msg",output_queue,)),
        Thread(target=msg_input, args=("input msg",input_queue,)),]


        print("开始启动串口读写线程....")
        for item in threads:
            item.start()

        for item in threads:
            item.join()
    print("开始关闭串口.....")
    my_serial.close()




