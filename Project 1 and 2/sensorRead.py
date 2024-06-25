#!/usr/bin/env pybricks-micropython
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import (Motor, TouchSensor, ColorSensor, InfraredSensor, UltrasonicSensor, GyroSensor)
from pybricks.parameters import Port, Stop, Direction, Button, Color
from pybricks.tools import wait, StopWatch, DataLog
from pybricks.robotics import DriveBase
from pybricks.media.ev3dev import SoundFile, ImageFile

def main():
    ev3 = EV3Brick()
    colorSensor = ColorSensor(Port.S1)
    while True:
        colors = colorSensor.rgb()
        ev3.screen.print("Red =", colors[0], "\nGreen =", colors[1], "\nBlue =", colors[2])
        wait(200)
        ev3.screen.clear()

main()