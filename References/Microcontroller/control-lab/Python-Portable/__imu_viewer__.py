from OpenGL.GL import *
from OpenGL.GLU import *
import pygame
from pygame.locals import *
import serial
import math
import threading
import time

# ser = serial.Serial('/dev/tty.usbserial', 38400, timeout=1)
ser = serial.Serial('COM74', 115200, timeout=1)
# ser = None

run = True
ax = ay = az = 0.0
yaw_mode = False


def resize(width, height):
    if height == 0:
        height = 1
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, 1.0 * width / height, 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()


def init():
    glShadeModel(GL_SMOOTH)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)


def drawText(position, textString):
    font = pygame.font.SysFont("Courier", 18, True)
    textSurface = font.render(textString, True, (255, 255, 255, 255), (0, 0, 0, 255))
    textData = pygame.image.tostring(textSurface, "RGBA", True)
    glRasterPos3d(*position)
    glDrawPixels(textSurface.get_width(), textSurface.get_height(), GL_RGBA, GL_UNSIGNED_BYTE, textData)


def draw():
    global rquad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity()
    glTranslatef(0, 0.0, -7.0)

    osd_text = "ay: " + str("{0:.2f}".format(ay)) + ", ax: " + str("{0:.2f}".format(ax))

    if yaw_mode:
        osd_line = osd_text + ", az: " + str("{0:.2f}".format(az))
    else:
        osd_line = osd_text

    drawText((-2, -2, 2), osd_line)

    # the way I'm holding the IMU board, X and Y axis are switched
    # with respect to the OpenGL coordinate system
    if yaw_mode:  # experimental
        glRotatef(az, 0.0, 1.0, 0.0)  # Yaw,   rotate around y-axis
    else:
        glRotatef(0.0, 0.0, 1.0, 0.0)
    glRotatef(ay, 1.0, 0.0, 0.0)  # Pitch, rotate around x-axis
    glRotatef(-1 * ax, 0.0, 0.0, 1.0)  # Roll,  rotate around z-axis

    glBegin(GL_QUADS)
    glColor3f(0.0, 1.0, 0.0)
    glVertex3f(1.0, 0.2, -1.0)
    glVertex3f(-1.0, 0.2, -1.0)
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f(1.0, 0.2, 1.0)

    glColor3f(1.0, 0.5, 0.0)
    glVertex3f(1.0, -0.2, 1.0)
    glVertex3f(-1.0, -0.2, 1.0)
    glVertex3f(-1.0, -0.2, -1.0)
    glVertex3f(1.0, -0.2, -1.0)

    glColor3f(1.0, 0.0, 0.0)
    glVertex3f(1.0, 0.2, 1.0)
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f(-1.0, -0.2, 1.0)
    glVertex3f(1.0, -0.2, 1.0)

    glColor3f(1.0, 1.0, 0.0)
    glVertex3f(1.0, -0.2, -1.0)
    glVertex3f(-1.0, -0.2, -1.0)
    glVertex3f(-1.0, 0.2, -1.0)
    glVertex3f(1.0, 0.2, -1.0)

    glColor3f(0.0, 0.0, 1.0)
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f(-1.0, 0.2, -1.0)
    glVertex3f(-1.0, -0.2, -1.0)
    glVertex3f(-1.0, -0.2, 1.0)

    glColor3f(1.0, 0.0, 1.0)
    glVertex3f(1.0, 0.2, -1.0)
    glVertex3f(1.0, 0.2, 1.0)
    glVertex3f(1.0, -0.2, 1.0)
    glVertex3f(1.0, -0.2, -1.0)
    glEnd()


def read_data():
    global ax, az, ay
    line = ser.readline()
    data = line.split(b',')
    if len(data) != 9:
        return

    dT = float(data[0]) / 1000
    rawax = int(data[1])
    raway = int(data[2])
    rawaz = int(data[3])
    rawgx = int(data[4])
    rawgy = int(data[5])
    rawgz = int(data[6])
    phi_hat = float(data[7]) / 1000
    theta_hat = float(data[8]) / 1000

    ax = phi_hat
    ay = theta_hat
    az = 0
    print('{},{},{},{},{},{},{},{},{}'.format(dT,
                                              rawax, raway,
                                              rawaz, rawgx,
                                              rawgy, rawgz,
                                              phi_hat, theta_hat))

    with open('data.txt', 'a') as datafile:
        datafile.write('{},{},{},{},{},{},{},{},{}\r\n'.format(dT,
                                                               rawax, raway,
                                                               rawaz, rawgx,
                                                               rawgy, rawgz,
                                                               phi_hat, theta_hat))


def thread_func():
    if ser is None:
        return
    
    while run:
        read_data()


def main():
    global yaw_mode, run

    video_flags = OPENGL | DOUBLEBUF

    t = threading.Thread(target=thread_func)
    t.start()

    pygame.init()
    screen = pygame.display.set_mode((640, 480), video_flags)
    pygame.display.set_caption("Press Esc to quit, z toggles yaw mode")
    resize(640, 480)
    init()
    frames = 0
    ticks = pygame.time.get_ticks()
    while 1:
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            pygame.quit()  # * quit pygame properly
            break
        if event.type == KEYDOWN and event.key == K_z:
            yaw_mode = not yaw_mode
            ser.write(b"z")
        draw()

        pygame.display.flip()
        frames = frames + 1

    run = False
    t.join()

    print("fps:  %d" % ((frames * 1000) / (pygame.time.get_ticks() - ticks)))
    ser.close()


if __name__ == '__main__':
    main()
