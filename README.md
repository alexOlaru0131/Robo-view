# Robo-view

The robot from this project is built to test the power of `6V DC engines`, to test the power of an `development board made by Espressif` and what I could do with a camera.

![alt text](https://github.com/alexOlaru0131/Robo-view/blob/main/Robot%20photo.jpg)

For the structural part of the robot I used some `PVC planks` on which the components were glued, except the power supply module.

The main board, an `ESP32 DevKIT V1`, is where the main code was loaded. Next to it, I attached a `engine driver module L298N` that drives the `6V DC engines`. The speed of the engines could not be adjusted as I just wanted to make it move. The mobile part of the robot consists of the two engines and a `mini servomotor SG90`. The servo was used for moving the camera up and down. The camera used is an `ESP32-CAM`. The power supply module provides 3,3V, 5V and 12V. The main board is powered through the microUSB port, and the camera is powered from the main board.

For the software part, I created the web server the it's setup is initialized using the function `setupWebServer()`, in which it's the HTML string. In the front page I added:
- button `Inainte` for moving forward and it calls the function `moveForward()`
- button `Inapoi` for moving backward and it calls the function `moveBackward()`
- button `Stanga` for rotating left and it calls the function `turnLeft()`
- button `Dreapta` for rotating right and it calls the function `turnRight()`
- button `Stop` for stopping the motors in case of emergency and it calls the function `stopMotors()`
- the enable `Detectie fete` for tuning on the face detection model installed on the ESP32-CAM
- the slider `Pozitie servo` for controlling the position of the servo motor
- and the live image from the camera

The moving functions command PWM signals with a fixed duty cicle (100%).

![alt text](https://github.com/alexOlaru0131/Robo-view/blob/main/Server%20screenshot.png)

[![Watch the video]((https://github.com/alexOlaru0131/Robo-view/blob/main/Snapchat-774478484.mp4)](https://github.com/alexOlaru0131/Robo-view/blob/main/Snapchat-774478484.mp4)
