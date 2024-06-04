from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.parameters import Port
ev3 = EV3Brick()
rightMotor = Motor(Port.A)
leftMotor = Motor(Port.D)
pinchMotor = Motor(Port.C)
colorSensor = ColorSensor(Port.S1)
ultraSensor = UltrasonicSensor(Port.S4)


detectDistance = 15
forwardSpeed = 25
defSpeed = 15
lowRed = 10
highRed = 15
highGreen = 20
turnLimit = 230 #SEARCH SWEEP ANGLE
finish = False
lastLeft = False

def beep():
    ev3.speaker.set_volume(50, '_all_')
    ev3.speaker.beep(200, 2000)

def mega():
    baseDur = 75
    baseNoteDur = 50
    ev3.speaker.set_volume(50, '_all_')

    ev3.speaker.beep(293, baseNoteDur * 1)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(293, baseNoteDur * 1)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(587, baseNoteDur * 2)
    ev3.wait(baseDur * 2)
    ev3.speaker.beep(440, baseNoteDur * 2)
    ev3.wait(baseDur * 2)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(415, baseNoteDur * 1)
    ev3.wait(baseDur * 1)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(392, baseNoteDur * 1)
    ev3.wait(baseDur * 1)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(349, baseNoteDur * 2)
    ev3.wait(baseDur * 2)
    ev3.speaker.beep(293, baseNoteDur * 1)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(349, baseNoteDur * 1)
    ev3.wait(baseDur * 1)
    ev3.speaker.beep(392, baseNoteDur * 1)
    ev3.wait(baseDur * 1)

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
        ev3.wait(10)

def reverse():
    rightMotor.run(forwardSpeed)
    leftMotor.run(forwardSpeed)

def reverseFor(deg):
    rightMotor.reset_angle(0)
    forward()
    while (rightMotor.angle() < deg):
        ev3.wait(10)

def rotateRight():
    leftMotor.run(-defSpeed)
    rightMotor.run(defSpeed)

def rotateLeft():
    leftMotor.run(defSpeed)
    rightMotor.run(-defSpeed)

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
        ev3.wait(10)

def findLeftFor(deg):
    rightMotor.reset_angle(0)
    rotateLeft()
    while (-rightMotor.angle() < deg):
        colors = colorSensor.rgb()
        if (colors[0] <= lowRed):
            stopMoving()
            lastLeft = True
            return True    
    return False

def findRightFor(deg):
    leftMotor.reset_angle(0)
    rotateRight()
    while (-leftMotor.angle() < deg):
        colors = colorSensor.rgb()
        if (colors[0] <= lowRed):
            stopMoving()
            lastLeft = False
            return True
    return False

def turnBack():
    reverse()
    ev3.wait(150)
    rotateRightFor(360)

def findLine():
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
    stopMoving()
    beep()
    ev3.wait(2000)

    colors = colorSensor.rgb()
    notOnline = (colors[0] >= highRed)
    if notOnline:
        findLine()
    
    isOnGreen = (colors[1] >= highGreen)
    if isOnGreen:
        forward()
        ev3.wait(300)
        stopMoving()

        pinchMotor.run(-90)
        ev3.wait(1500)
        pinchMotor.hold()

        if lastLeft:
            rotateRightFor(240)
        else:
            rotateLeftFor(240)
        
        stopMoving()

        forwardFor(480)
        stopMoving()

        pinchMotor.run(90)
        ev3.wait(1500)
        pinchMotor.hold()

        reverseFor(480)
        stopMoving()
        ev3.wait(500)

        if lastLeft:
            rotateLeftFor(240)
        else:
            rotateRightFor(240)
        
        stopMoving()
    else:
        turnBack()

def main():
    while not finish:

        distance = ultraSensor.distance(False)

        if distance <= detectDistance:
            processObject()

        colors = colorSensor.rgb()

        if colors[0] >= highRed:
            findLine()

        forward()
        ev3.wait(40)

    stopMoving()
    mega()
    ev3.wait(2000)
