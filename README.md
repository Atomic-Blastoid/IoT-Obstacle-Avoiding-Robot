# IoT-Based Obstacle Avoiding Robot 🚗🤖

## 📌 Project Overview
This project is an IoT-based autonomous obstacle avoiding robot developed using Arduino UNO and ESP8266.  
The robot navigates autonomously by detecting obstacles using ultrasonic and IR sensors and can be monitored and controlled wirelessly through a web-based dashboard.

---

## 🔧 Hardware Components
- Arduino UNO  
- ESP8266 (ESP-01)  
- Ultrasonic Sensor (HC-SR04)  
- IR Obstacle Sensors (2)  
- Servo Motor  
- DC Motors with Motor Driver Shield  
- **Robot Chassis (4-Wheel / 2-Wheel Acrylic Chassis)**  
- Caster Wheel  
- Battery Pack  

---

## 🧠 Key Features
- Autonomous obstacle avoidance
- Sensor fusion using Ultrasonic and IR sensors
- Blind spot detection near front wheels
- Servo-based left and right ultrasonic scanning
- IoT-based monitoring and control using ESP8266
- Web dashboard with:
  - Live obstacle distance display
  - Speed control slider
  - STOP / CONTINUE buttons

---

## ⚙️ Working Principle
- The ultrasonic sensor detects obstacles in front of the robot.
- IR sensors detect very close obstacles near the front wheels to remove blind spots.
- When an obstacle is detected, the robot:
  - Stops immediately
  - Moves backward slightly
  - Scans left and right using the ultrasonic sensor mounted on a servo motor
  - Turns toward the direction with more free space
  - Continues moving forward autonomously

---

## 🌐 IoT Dashboard
The ESP8266 operates in SoftAP mode and hosts a local web server.  
The dashboard allows:
- Live monitoring of obstacle distance
- Speed adjustment of the robot
- Remote STOP and CONTINUE control

---

## 🚀 Future Improvements
- Camera-based navigation
- Cloud data logging
- Mobile application control
- AI-based obstacle recognition

---

## 👨‍💻 Author
**Your Name**  
IoT-Based Robotics Project  
