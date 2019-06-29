import cv2
import time
import csv
import serial
import math
import random
import struct
from threading import Thread


class PsEye(Thread):
    def __init__(self):
        super().__init__()
        self._run = True
        self._video = cv2.VideoCapture(1)
        self._last_frame = None

    def get_act_frame(self):
        return self._last_frame

    def run(self):
        while self._run:
            check, frame = self._video.read()
            if check:
                self._last_frame = frame

    def save_last_frame(self, img_file_path, img_name):
        cv2.imwrite(str(img_file_path + img_name), self._last_frame)

    def end(self):
        self._run = False
        self.join()

    def frame_valid(self):
        return self._last_frame is not None


Camera = PsEye()
Camera.start()

label = 0

img_file_path = "./pictures/"


while not Camera.frame_valid():
    time.sleep(0.5)

while label < 10:
    img_name = "{}_{}.png".format(label, time.time())
    print('Saving img ', img_name)
    Camera.save_last_frame(img_file_path, img_name)
    label += 1
    time.sleep(1)

print('END')
Camera.end()
exit(0)


# list available ports:
# $ python -m serial.tools.list_ports

video = cv2.VideoCapture(0)
ser = serial.Serial('/dev/ttyACM1', 115200)
csv_file_path = "./imgage_labels.csv"
img_file_path = "./pictures/"


def move(random_rad):
    ser.write(str(random_rad).encode('utf-8'))


def get_label_value():
    ser.flushInput()
    value = ser.read_until()
    return float(value)


def take_picture(video, img_file_path, img_name):
    check, frame = video.read()
    cv2.imwrite(str(img_file_path + img_name), frame)


def log_csv(file, img_name, label):
    with open(file, 'a') as csvFile:
        writer = csv.writer(csvFile)
        writer.writerow([img_name, label])
    csvFile.close()


for a in range(5):
    rand_increment = random.randint(100, 800)
    rand_rad = rand_increment / 4096 * (2 * math.pi)
    move(rand_rad)
    time.sleep(2)
    label = get_label_value()
    label = label % (2 * math.pi)
    img_name = "{}_{}.png".format(label, time.time())
    take_picture(video, img_file_path, img_name)
    log_csv(csv_file_path, img_name, label)

video.release()

