import serial
import serial.tools.list_ports
"""
class series(serial.Serial):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
"""
if __name__ == "__main__":
    print("serial.VERSINON: ", serial.VERSION)
    info = serial.tools.list_ports.comports()
    print("list current port: ", info,info.device)