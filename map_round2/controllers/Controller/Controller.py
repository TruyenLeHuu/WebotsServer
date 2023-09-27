from controller import Robot
from controller import Motor
from controller import DistanceSensor
from controller import Camera
from controller import LED
from controller import Supervisor
import math
robot = Robot()

# get the time step of the current world
timestep = 8
robot.step(timestep)

#camera     
cam = robot.getDevice("camera")
cam.enable(64)
# Leff motor, Right motor   
lm = robot.getDevice("left wheel motor")
lm.setPosition(float("inf"))
lm.setVelocity(0)

rm = robot.getDevice("right wheel motor")
rm.setPosition(float("inf"))
rm.setVelocity(0)

# Sensors
NB_GROUND_SENS = 8
gs = []
gsNames = [
    'gs0', 'gs1', 'gs2', 'gs3',
    'gs4', 'gs5', 'gs6', 'gs7'
]
for i in range(NB_GROUND_SENS):
    gs.append(robot.getDevice(gsNames[i]))
    gs[i].enable(timestep)

# LEDs
NB_LEDS = 5
leds = []
led_Names = [
    'led0', 'led1', 'led2', 'led3', 'led4'
]
for i in range(NB_LEDS):
    leds.append(robot.getDevice(led_Names[i]))

### Private Functions ###
# Function to control LEDs
def LED_Alert():
    if (robot.getTime() - initTime)*1000 % 3000 >= 2000:
        #leds[1].set(not(leds[1].get()))
        leds[1].set(1)
        #for i in range(NB_LEDS):
            #leds[i].set(not(leds[i].get()))
    return

# Waiting for completing initialization
### thí sinh không được bỏ phần này
initTime = robot.getTime()
while robot.step(timestep) != -1:
    if (robot.getTime() - initTime) * 1000.0 > 200:
        break


### Phần code cần chỉnh sửa cho phù hợp ##
# Định nghĩa các tín hiệu của xe
NOP = -1
MID = 0
LEFT = 1
RIGHT = 2
FULL_SIGNAL  = 3
BLANK_SIGNAL = -3
TURN_SIGNAL = -1
TURN_CORNER = -1
TURN_CIRCLE = -1
COUNT_SIGNAL = 0
COUNT_CORNER = 0
COUNT_CIRCLE = 0
COUNT_STOP = 0
VAOCUA = -1
VAOCUAVUONG = -1
# MAX_SPEED <= 1000
MAX_SPEED = 50
threshold = [330, 330, 330, 330, 330, 330, 330, 330]
preFilted = 0b00000000

# Biến lưu giá trị tỉ lệ tốc độ của động cơ
left_ratio = 0.0
right_ratio = 0.0
# Hàm đọc giá trị của sensors
def ReadSensors():
    gsValues = []
    filted = 0x00
    for i in range(NB_GROUND_SENS):
        gsValues.append(gs[i].getValue())
        if gsValues[i] > threshold[i]:
            filted |= (0x01 << (NB_GROUND_SENS - i - 1))
    #print(*gsValues, sep = '\t')
    return filted

# Phần code điều khiển xe

#xác định độ lệch của xe từ việc đọc giá trị cảm biến
def DeterminePosition(filted):
    if (filted == 0b11100111 or filted == 0b11101111 or filted == 0b11001111 or filted == 0b11110011 or filted == 0b11110111):
         return MID
    elif (filted == 0b01111111 or filted == 0b00111111 or filted == 0b10111111 or filted == 0b11011111 or filted == 0b10011111):
        return LEFT
    elif (filted == 0b11111110 or filted == 0b11111100 or filted == 0b11111101 or filted == 0b11111011 or filted == 0b11111001):
        return RIGHT
    elif filted == 0b11111111:
        return FULL_SIGNAL
    elif filted == 0b00000000:
        return BLANK_SIGNAL
    return NOP
#các hàm điều khiển xe di chuyển
def GoStraight(filted):
    if filted == 0b11100111:
        return 1.0, 1.0
    if filted == 0b11101111:
        return 0.9, 1.0
    if filted == 0b11110111:
        return 1.0, 0.9
    if filted == 0b11001111:
        return 0.7, 1.0
    if filted == 0b11110011:
        return 1.0, 0.7
    return 1.0, 1.0
        
def TurnLeft(filted):
    if filted == 0b01111111:
        return 0.2, 1.0
    elif filted == 0b00111111:
        return 0.3, 1.0
    elif filted == 0b10111111:
        return 0.3, 1.0
    elif filted == 0b10011111:
        return 0.35, 1.0
    elif filted == 0b11011111:
        return 0.4, 1.0
    return -0.3, 0.68
        
def TurnRight(filted):
    if filted == 0b11111110:
        return 1.0, 0.2
    elif filted == 0b11111100:
        return 1.0, 0.3
    elif filted == 0b11111101:
        return 1.0, 0.3
    elif filted == 0b11111001:
        return 1.0, 0.35
    elif filted == 0b11111011:
        return 1.0, 0.4
    return 0.68, -0.3
    
lastPos = 0  
# Main loop:
# Chương trình sẽ được lặp lại vô tận 
while robot.step(timestep) != -1:
    filted = ReadSensors()
    #pos: position - vị trí của xe
    pos = DeterminePosition(filted)
    # In ra màn hình giá trị của filted ở dạng nhị phân
    #print('Position: ' + str(format(filted, '08b')), sep = '\t')
    #print(left_ratio, right_ratio)
    #print(VAOCUA, VAOCUAVUONG)
    #print(MAX_SPEED)
    # Xác định hướng rẽ của ngã tư
    if (filted == 0b00011111 or filted == 0b00001111 or filted == 0b00000111):
        TURN_CORNER = 1
    elif (filted == 0b11111000 or filted == 0b11110000 or filted == 0b11100000): 
        TURN_CORNER = 2
    #Gọi các hàm điều khiển
    if TURN_CIRCLE != -1:
        MAX_SPEED = 40
    else: 
        MAX_SPEED = 47
    if VAOCUA != -1:
        VAOCUA -= 1
        continue
    if VAOCUAVUONG == 1 and pos != LEFT:
        continue
    elif VAOCUAVUONG == 2 and pos != RIGHT:
        continue
    VAOCUAVUONG = -1
    if TURN_CIRCLE == 1 or TURN_CIRCLE == 0:
        COUNT_CIRCLE += 1
    if TURN_CIRCLE == 1 and COUNT_CIRCLE > 100:
        TURN_CIRCLE = -1
    if TURN_SIGNAL != -1:
        COUNT_SIGNAL += 1
    if COUNT_SIGNAL >= 20:
        TURN_SIGNAL = -1
        COUNT_SIGNAL = 0
    if TURN_CORNER != -1:
        COUNT_CORNER += 1
    if COUNT_CORNER >= 100:
        TURN_CORNER = -1
        COUNT_CORNER = 0
    if TURN_CIRCLE == -1:
        if (filted == 0b00111111 or filted == 0b00011111 or filted == 0b00001111 or filted == 0b00000111):
            TURN_SIGNAL = 1
        elif (filted == 0b11111100 or filted == 0b11111000 or filted == 0b11110000 or filted == 0b11100000): 
            TURN_SIGNAL = 2
    if TURN_SIGNAL != -1:
        if pos == MID:
            left_ratio, right_ratio = GoStraight(filted)
            TURN_SIGNAL = -1
            COUNT_SIGNAL = 0
            COUNT_STOP = 0
        elif pos == FULL_SIGNAL:
            if TURN_SIGNAL == 1:
                left_ratio, right_ratio = TurnLeft(filted)
                TURN_SIGNAL = -1
                TURN_CORNER = -1
                COUNT_CORNER = 0
                COUNT_SIGNAL = 0
                VAOCUAVUONG = 1
            elif TURN_SIGNAL == 2:
                left_ratio, right_ratio = TurnRight(filted)
                TURN_SIGNAL = -1
                TURN_CORNER = -1
                COUNT_CORNER = 0
                COUNT_SIGNAL = 0
                VAOCUAVUONG = 2
        elif pos == LEFT:
            left_ratio, right_ratio = TurnLeft(filted)
        elif pos == RIGHT:
            left_ratio, right_ratio = TurnRight(filted)
    else:
        if pos == MID:
            left_ratio, right_ratio = GoStraight(filted)
            COUNT_STOP = 0
        elif pos == LEFT:
            left_ratio, right_ratio = TurnLeft(filted)
        elif pos == RIGHT:
            left_ratio, right_ratio = TurnRight(filted)
        else:
            if TURN_CIRCLE == 0 and COUNT_CIRCLE > 50:
                left_ratio, right_ratio = 0.7, 0
                COUNT_CIRCLE = 0
                TURN_CIRCLE = -1
                VAOCUAVUONG = 2
            else:
                if pos == BLANK_SIGNAL:
                    if TURN_CORNER == 1:
                        left_ratio, right_ratio = TurnLeft(filted)
                        TURN_CORNER = -1
                        VAOCUA = 30
                        COUNT_CORNER = 0
                        VAOCUAVUONG = 1
                    elif TURN_CORNER == 2:
                        left_ratio, right_ratio = TurnRight(filted)
                        TURN_CORNER = -1
                        VAOCUA = 30
                        COUNT_CORNER = 0
                        VAOCUAVUONG = 2
                    else:
                        TURN_CIRCLE = 1
                        COUNT_STOP += 1
                else:
                    if TURN_CIRCLE == 1 and COUNT_CIRCLE > 20:
                        left_ratio, right_ratio = 0.7, 0
                        VAOCUA = 20
                        VAOCUAVUONG = 2
                        TURN_CIRCLE = 0
                        COUNT_CIRCLE = 0
    if lastPos == MID and pos == FULL_SIGNAL:
        left_ratio, right_ratio = 1.0, 1.0
    if COUNT_STOP > 30:
        left_ratio, right_ratio = 0,0
    #print('TURN_SIGNAL', TURN_SIGNAL, COUNT_SIGNAL)
    #print('TURN_CORNER', TURN_CORNER, COUNT_CORNER)
    #print('TURN_CIRCLE', TURN_CIRCLE, COUNT_CIRCLE)
    lm.setVelocity(left_ratio * MAX_SPEED)
    rm.setVelocity(right_ratio * MAX_SPEED)
    preFilted = filted
    lastPos = pos
    #print('UIT CAR RACING 2022')
    pass
# Enter here exit cleanup code.D