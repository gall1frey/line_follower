Components:
  1. Arduino Uno
  2. Line Follower Array
  3. HC-SR04 Ultrasonic Sensor
  4. HC-05 Bluetooth module
  5. L9110s H bridge
  6. DC motors x 2
  7. Chassis, wheels, breadboard, battery

  #define MOTOR_R_DIR 2
  #define MOTOR_R_PWM 3
  #define MOTOR_L_DIR 4
  #define MOTOR_L_PWM 5


Connections:

  HC-SR04 PINS:
    COMPONENT             ARDUINO
    TRIG        ------>   12
    ECHO        ------>   11
    GROUND      ------>   GND
    VCC         ------>   5V

  HC-05 PINS:
    COMPONENT             ARDUINO
    TX          ------>   RX
    RX          ------>   TX
    GROUND      ------>   GND
    VCC         ------>   5V

  L9110 PINS:
    COMPONENT             ARDUINO
      A1A       ------>   3
      A1B       ------>   2
      VCC       ------>   5V + additional power source (optional)
      GND       ------>   GND
      B1A       ------>   5
      B1B       ------>   4

  Line Follower Array PINS:
    COMPONENT             ARDUINO
      SDA       ------>   SDA
      SCL       ------>   SCL
      GND       ------>   GND
      VCC       ------>   5V
