# RaspiTank

"Hope", a RaspberryPi-based tank robot which dreams to walk around home autonomously.

![Hope_front](https://github.com/manfredipist/RaspiTank/blob/main/doc/images/front.png?raw=true)
![Hope_side](https://github.com/manfredipist/RaspiTank/blob/main/doc/images/side.png?raw=true)
![Hope_qr_recognition](https://github.com/manfredipist/RaspiTank/blob/main/doc/images/qrcode.png?raw=true)

**DISCLAIMER**: :construction: This project is far from complete, don't expect everything to work flawlessly out of the box

## Prerequisites

1. RaspberryPi 3B+ or RaspberryPi 4
  - [RaspberryPi 4](https://amzn.to/3ig6KYC)
  - [RaspberryPi 3B+](https://amzn.to/3hIhGPX)
2. A chassis for your robot as well as a camera and bunch of sensors
  - [Tank Chassis](https://amzn.to/3xJkj9J)
  - [PiCamera](https://amzn.to/3elmetq)
  - [VL53L1X](https://amzn.to/3kl63zX)
  - [MPU6050](https://amzn.to/2TbG1Ea)
  - [HMC5883L](https://amzn.to/3wJPOz3)
  - [TCA9548A](https://amzn.to/3xKhjK5)
  - [PCA9685](https://amzn.to/2UbhgIE)
  - [L298N](https://amzn.to/3kkK8sI)
  - [LM2596S Buck converters](https://amzn.to/36DRnnC)
  - [Multimeter](https://amzn.to/3ig8otg)
  - [18650 UPS](https://amzn.to/3z1DM5O)
  - [18650 Battery Charger](https://amzn.to/3eEILBJ)
  - [Jumpers](https://amzn.to/3wJ6RkC)
  - [MicroServo SG90](https://amzn.to/3wGZr1t)

3. Working cross-compilation environment on both MacOS/Ubuntu and your Raspberry Pi OS. Follow my [guide](https://github.com/manfredipist/RPi3BPlusQtOpenCV) if it's not the case. If i recall correctly, the guide works on RaspberryPi 4 as well but you may not need to increase your swap size when compiling OpenCV due to increased RAM size on RPi4
4. Lot of patience, this project is not going to work out of the box, in particular, i'm referring to MacOS users who will have to deal with Privacy&Security features being introduced, starting from Catalina.


## Suggestions
If you encounter any problem you're not able to fix, please ask for support pushing an issue, i'll be more than happy to help you. Once you've got a fully working environment, i truly suggest you to __*backup the SD card of your RPi.*__


## The project
In principle my idea was to provide Hope of two ways of acting:
1. by planning tasks inside a 2D world
2. following goals by auto-relocating inside a world with the assistance of QR Codes / Landmarks.

Both operational modes are currently under construction.

The project is made up of 5 main folders:
1. **PiBus** - a C++ Qt implementation of I2C Protocol, you can find more information about this side project of mine [here](https://github.com/manfredipist/QI2CProtocol);
  - Status: Completed :white_check_mark:
2. **PiController** - the main class which setup the GUI and start PiThreads;
  - Status: Completed :white_check_mark:
3. **PiDevices** - contains few I2C sensors driver implementations;
  - Status: Completed :white_check_mark:
4. **PiThreads** - contains the main threads that run during the execution loop of the program;
  - Status: WIP :construction:
    - **PiMotion** : thread handling Hope's movement
      - Status: Completed :white_check_mark:
    - **PiPlanner** : thread handling Hope's task planning
      - Status: WIP :construction:
    - **PiSensing** : thread handling Hope's sensors data
      - Status: Completed :white_check_mark:
    - **PiVision** : thread handling Hope's computer vision
      - Status: WIP :construction:
5. **PiWidgets** - contains the GUI Widget that are injected into the MainWindow by PiController class.
  - Status: Completed :white_check_mark:

![RaspiTankGUI](https://github.com/manfredipist/RaspiTank/blob/main/doc/images/raspiTank_1.png?raw=true)
![RaspiTankGUI](https://github.com/manfredipist/RaspiTank/blob/main/doc/images/raspiTank_2.png?raw=true)
![RaspiTankGUI](https://github.com/manfredipist/RaspiTank/blob/main/doc/images/raspiTank_3.png?raw=true)

## The Build
I will try to upload a guide to build Hope from scratch, i gotta check my phone to retrieve some old photos when i was building it. To make it simple, Hope is built on 3 levels:
- At the lowest level you can find 2 buck converters which derive two power lines, one at 12v for the 2 DC motors and one at 5v for the servo motor
- On the middle level you can find all the sensors i've been using as well as 4 18650 batteries to power up all the sensors and the motors
- At the toppest level you can find the RPi, the camera and the UPS to power up the RPi

## Hope in Action

I will add a video ASAP
