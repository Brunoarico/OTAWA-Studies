import serial
import time
import json
from serial.tools import list_ports

def setup_serial(port, baud_rate):
    ser = serial.Serial(port, baud_rate)
    return ser

def connSerial ():
    ports = list(list_ports.comports())
    if len(ports) == 0:
        print("Nenhuma porta disponivel")
        return None
    else:
        print("Portas disponiveis:")
        for p in ports:
            print (p[0])
        baud_rate = 115200  
        ser = setup_serial(p[0], baud_rate)
        return ser

def send_and_receive_json(ser, data):
    try:
        json_data = json.dumps(data)
        ser.write(json_data.encode())
        response = ser.readline().decode().strip()
        ser.close()
        return response
    except Exception as e:
        print("Erro na comunicação serial:", e)
        return None

if __name__ == "__main__":
    
    data = {
        "namefunc": "binary_search_test",
        "parametersArray": [
            [1],
            [4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
            [7, 8, 9],
            [10, 21, 76, 190, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
        ],
        "parameters": [1,11, 5, 8, 400]
    }


    ser = connSerial()
    response = send_and_receive_json(ser, data)
    print("Resposta:", response)