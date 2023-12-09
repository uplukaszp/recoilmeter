# recoilmeter
Recoilmeter based on mpu6050 and arduino

# communication

## serial port: baud rate 38400

## beep codes
- 3 long           - ready to work
- 2 short          - sensor error
- 2 long 2 short   - sd card error

### yellow button
- 2 long            - calibration complete
- 2 short           - sensor error

### green button
- 2 long, 2 long    - measurement start, writting complete
- 1 long 2 short    - sensor error
- 2 long            - sd card error

# connections

## mpu6050

MPU6050	-	ESP12E
VCC		-	3V
GND		-	GND
SDA		-	D2 (SDA)
SCL		- 	D1 (SCL)


## keyboard
keyboard	-	ESP12E
1			-	GND
2			-	D4
3			-	D3

## sd card reader
reader  - ESP12E
GND     - GND
3V      - 3V
CS      - D8 
MOSI    - D7
SCK     - D5
MISO    - D6

## buzzer
buzzer  - ESP12E
-       - GND
+       - 3V
OUT     - D0