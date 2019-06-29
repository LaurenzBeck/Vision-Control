# Imports
import numpy as np
import cv2
import csv
from time import time, sleep
from serial import Serial
from random import randint
from threading import Thread
from tqdm import tqdm


# Class and Function Definitions

################################################################
# Serial Thread

class Ser(Thread):
    def __init__(self, portname):
        super().__init__()
        self._run = True
        self._ser = Serial(portname, 115200)
        self._last_value = 0.0
        self._reference = 0.0
        self._expected_reference = 0.0
        self._valid = False

    def get_act_value(self):
        return self._last_value

    def value_valid(self):
        return self._valid

    def increase_reference(self, random_rad):
        self._expected_reference += round(random_rad, 2)

    def write_reference(self):
        self._ser.write(str(self._expected_reference).encode('utf-8'))
        self._ser.write('\r\n'.encode('utf-8'))

    def reference_accepted(self):
        return self._expected_reference == self._reference

    def run(self):
        print('Serial Thread running')
        while self._run:
            line = self._ser.readline()
            data = line.split(b',')
            if len(data) == 3:
                self._valid = bool(int(data[0]))

                try:
                    self._last_value = float(data[1])
                    self._reference = float(data[2])
                except ValueError:
                    self._last_value = None
                    self._reference = None
                    self._valid = False

                print(self._reference)
            else:
                self._last_value = None
                self._reference = None
                self._valid = False

    def end(self):
        self._run = False
        self.join()
        self._ser.close()


################################################################
# Camera Thread

class PsEye(Thread):
    def __init__(self):
        super().__init__()
        self._run = True
        self._video = cv2.VideoCapture(0)
        self._last_frame = None

    def get_act_frame(self):
        return self._last_frame

    def run(self):
        while self._run:
            check, frame = self._video.read()
            if check:
                self._last_frame = frame

    def save_last_frame(self, img_file_path, img_name):
        print('Saving img {}'.format(img_name))
        cv2.imwrite(str(img_file_path + img_name), self._last_frame)

    def end(self):
        self._run = False
        self.join()
        self._video.release()

    def frame_valid(self):
        return self._last_frame is not None


################################################################
# Main Functions

def get_label_value():
    for attempt in range(10):
        if ser.value_valid():
            repeat = False
            label = ser.get_act_value()
            label = round(label % (2 * np.pi), 2)
            print("got label {}".format(str(label)))
            return (True, label)
        sleep(0.01)
    print("No valid value")
    return (False, None)


def log_csv(file, img_name, label):
    with open(file, 'a') as csvFile:
        writer = csv.writer(csvFile)
        writer.writerow([img_name, label])
    pass


# Main Programm

port = '/dev/ttyACM1'

csv_file_path = "./image_labels_20_6_19.csv"
img_file_path = "./pictures_20_6_19/"

num_images = 1

print("Starting Serial Thread...")
ser = Ser(port)
ser.start()

print("\nStarting Camera Thread...")
camera = PsEye()
camera.start()

# Waiting until camera ready
while not camera.frame_valid():
    sleep(0.05)

print('Camera Thread running...')

img_counter = 0
labels = []

for a in tqdm(range(num_images)):
    # clear_output(wait=True)
    rand_increment = randint(100, 200)
    rand_rad = rand_increment / 4096 * (2 * np.pi)
    ser.increase_reference(rand_rad)

    for _ in range(10):
        if not ser.reference_accepted():
            ser.write_reference()
            sleep(0.010)
        else:
            break

    if not ser.reference_accepted():
        raise IOError

    while not ser.value_valid():
        sleep(0.01)

    (success, label) = get_label_value()
    if success:
        img_name = "{}_{}.png".format(time(), label)
        camera.save_last_frame(img_file_path, img_name)
        log_csv(csv_file_path, img_name, label)
        img_counter += 1
        print('attenpt {} successful!'.format(a + 1))
        labels.append(label)
    else:
        print('attenpt {} FAILED!'.format(a + 1))

print("{} out of {} attempts successful".format(img_counter, num_images))
camera.end()
ser.end()
print("closing Threads...")
