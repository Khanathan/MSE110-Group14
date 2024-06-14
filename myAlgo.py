#!/usr/bin/env pybricks-micropython
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import (Motor, TouchSensor, ColorSensor, InfraredSensor, UltrasonicSensor, GyroSensor)
from pybricks.parameters import Port, Stop, Direction, Button, Color
from pybricks.tools import wait, StopWatch, DataLog
from pybricks.robotics import DriveBase
from pybricks.media.ev3dev import SoundFile, ImageFile

ev3 = EV3Brick()
rightMotor = Motor(Port.A)
leftMotor = Motor(Port.D)
pinchMotor = Motor(Port.C)
colorSensor = ColorSensor(Port.S1)
ultraSensor = UltrasonicSensor(Port.S4)


detectDistance = 130
forwardSpeed = 250
turnSpeed = 140
pinchSpeed = 3000
pinchDur = 2300
lowRed = 10
highRed = 15
highGreen = 20
turnLimit = 230 #SEARCH SWEEP ANGLE
finish = False
lastLeft = False
lastGreen = True #0 Green, 1 Blue

def beep():
    ev3.speaker.set_volume(50, '_all_')
    ev3.speaker.beep(200, 2000)

def mega():
    baseDur = 75
    baseNoteDur = 50
    ev3.speaker.set_volume(50, '_all_')

    ev3.speaker.beep(293, baseNoteDur * 1)
    wait(baseDur * 1)
    ev3.speaker.beep(293, baseNoteDur * 1)
    wait(baseDur * 1)
    ev3.speaker.beep(587, baseNoteDur * 2)
    wait(baseDur * 2)
    ev3.speaker.beep(440, baseNoteDur * 2)
    wait(baseDur * 2)
    wait(baseDur * 1)
    ev3.speaker.beep(415, baseNoteDur * 1)
    wait(baseDur * 1)
    wait(baseDur * 1)
    ev3.speaker.beep(392, baseNoteDur * 1)
    wait(baseDur * 1)
    wait(baseDur * 1)
    ev3.speaker.beep(349, baseNoteDur * 2)
    wait(baseDur * 2)
    ev3.speaker.beep(293, baseNoteDur * 1)
    wait(baseDur * 1)
    ev3.speaker.beep(349, baseNoteDur * 1)
    wait(baseDur * 1)
    ev3.speaker.beep(392, baseNoteDur * 1)
    wait(baseDur * 1)

def stopMoving():
    rightMotor.hold()
    leftMotor.hold()

def forward():
    rightMotor.run(-forwardSpeed)
    leftMotor.run(-forwardSpeed)

def forwardFor(deg):
    rightMotor.reset_angle(0)
    forward()
    while (-rightMotor.angle() < deg):
        wait(10)

def reverse():
    rightMotor.run(forwardSpeed)
    leftMotor.run(forwardSpeed)

def reverseFor(deg):
    rightMotor.reset_angle(0)
    reverse()
    while (rightMotor.angle() < deg):
        wait(10)

def rotateRight():
    leftMotor.run(-turnSpeed)
    rightMotor.run(turnSpeed)

def rotateLeft():
    leftMotor.run(turnSpeed)
    rightMotor.run(-turnSpeed)

def rotateRightFor(deg):
    rotateMotorFor(deg, leftMotor)

def rotateLeftFor(deg):
    rotateMotorFor(deg, rightMotor)

def rotateMotorFor(deg, motor):
    motor.reset_angle(0)
    if motor == leftMotor:
        rotateRight()
    else:
        rotateLeft()
    while (-motor.angle() < deg):
        wait(10)

def findLeftFor(deg):
    rightMotor.reset_angle(0)
    rotateLeft()
    while (-rightMotor.angle() < deg):
        colors = colorSensor.rgb()
        if (colors[0] < lowRed):
            stopMoving()
            global lastLeft
            lastLeft = True
            return True    
    return False

def findRightFor(deg):
    leftMotor.reset_angle(0)
    rotateRight()
    while (-leftMotor.angle() < deg):
        colors = colorSensor.rgb()
        if (colors[0] < lowRed):
            stopMoving()
            global lastLeft
            lastLeft = False
            return True
    return False

def turnBack():
    reverse()
    wait(150)
    rotateRightFor(360)

def findLine():
    global finish
    stopMoving()

    if lastLeft:
        if findLeftFor(turnLimit):
            return
        
        if findRightFor(turnLimit * 2):
            return
        rotateLeftFor(turnLimit * 1.2)
    else:
        if findRightFor(turnLimit):
            return
        
        if findLeftFor(turnLimit * 2):
            return
        rotateRightFor(turnLimit * 1.2)
    
    stopMoving()
    finish = True

def processObject():
    #Pause and beep
    stopMoving()
    beep()
    wait(500)

    colors = colorSensor.rgb()
    notOnline = (colors[0] > lowRed)
    if notOnline:
        findLine()
        
    global lastGreen
    global lastLeft
    
    if lastGreen:
        #move towwards object
        forward()
        wait(450)
        stopMoving()

        #close pinch
        pinchMotor.run(-pinchSpeed)
        wait(pinchDur)
        pinchMotor.hold()

        #turn away
        if lastLeft:
            rotateLeftFor(210)
        else:
            rotateRightFor(210)
        
        stopMoving()

        #move away from track
        forwardFor(480)
        stopMoving()

        #open pinch
        pinchMotor.run(pinchSpeed)
        wait(pinchDur)
        pinchMotor.hold()

        #reverse back into the line
        reverseFor(480)
        stopMoving()
        wait(100)
        
        #rotate back into the line
        if lastLeft:
            rotateRightFor(210)
        else:
            rotateLeftFor(210)
        stopMoving()

    else:
        turnBack()
        lastLeft = not lastLeft



def main():
    global finish
    global detectDistance

    while not finish:
        distance = ultraSensor.distance(False)
        
        if distance <= detectDistance:
            processObject()

        colors = colorSensor.rgb()

        if colors[0] > highRed:
            findLine()
        
        global lastGreen
        if (colors[1] >= highGreen):
            lastGreen = True #Green
        else:
            lastGreen = False #Blue
        forward()
        wait(50)

    stopMoving()
    mega()
    wait(2000)

main()
