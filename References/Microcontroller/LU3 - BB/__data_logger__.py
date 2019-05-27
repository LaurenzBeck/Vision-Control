import serial
import time

# ser = serial.Serial('/dev/tty.usbserial', 38400, timeout=1)
ser = serial.Serial('COM5', 115200, timeout=1)
#ser = None

def read_data():
    if ser is None:
        time.sleep(1)
        print('No Serial-Port given')
        return

    line = ser.readline()
    data = line.split(b',')
    
    if len(data) != 4:
        return

    y = 0.0
    r = 0.0
    e = 0.0
    u = 0.0
    
    try:
        y = float(data[0])
    except ValueError:
        pass

    try: 
        r = float(data[1])
    except ValueError:
        pass

    try: 
        e = float(data[2])
    except ValueError:
        pass

    try: 
        u = float(data[3])
    except ValueError:
        pass
    
    print('{},{},{},{}'.format(y,
                               r,
                               e,
                               u))

    with open('data.txt', 'a') as datafile:
        datafile.write('{},{},{},{}\r\n'.format(y,
                                            r,
                                            e,
                                            u))


def main():
    try:    
        while True:
            read_data()
    except KeyboardInterrupt:
        pass

    ser.close()


if __name__ == '__main__':
    main()
