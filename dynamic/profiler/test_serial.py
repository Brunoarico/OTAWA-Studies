import serial
import serial.tools.list_ports

def setup_serial(port, baud_rate):
    ser = serial.Serial(port, baud_rate)
    return ser

def read_serial_data(ser):
    while True:
        data = ser.readline()
        if data:
            print("Recebido: {}".format(data.strip().decode()))

def close_serial(ser):
    ser.close()

if __name__ == "__main__":
    # Substitua pela porta serial do seu microcontrolador (por exemplo, "/dev/ttyUSB0" no Linux)
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print (p[0])
    baud_rate = 115200  # Substitua pela taxa de transmissão (baud rate) usada pelo microcontrolador
    ser = setup_serial(p[0], baud_rate)
    read_serial_data(ser)
    close_serial(ser)