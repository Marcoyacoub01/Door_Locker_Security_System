# Door Locker Security System
This project is designed and implemented based on the layered architecture
with the specifications of Door Locker Security System  to unlock a door using a password.
## Using two ATmega32 Microcontrollers with frequency 8Mhz.
**Mc1 → HMI_ECU (Human Machine Interface)** with 2x16 LCD and 4x4 keypad.
**HMI_ECU** is just responsible interaction with the user just take inputs through keypad and display messages on the LCD.
![f1](https://github.com/Marcoyacoub01/Door_Locker_Security_System/assets/118049131/3d5065d6-adcb-4bc1-8ed1-f35e3e9efaf5)
**Mc2 → Control_ECU** with EEPROM, Buzzer, and Dc-Motor.
**CONTROL_ECU** is responsible for all the processing and decisions in the system like password checking, open the door and activate the system alarm.
![f2](https://github.com/Marcoyacoub01/Door_Locker_Security_System/assets/118049131/bb462e13-fa27-4e16-b759-eda887c2100d)

>Step1 – Create a System Password
+The LCD should display “Plz Enter Password”.
+Enter a password consists of 5 numbers, Display * in the screen for each number.
![f3](https://github.com/Marcoyacoub01/Door_Locker_Security_System/assets/118049131/e89a0e3d-602a-4a95-9b6c-e421b46600a0)
+Press enter button (choose any button in the keypad as enter button).
+Ask the user to renter the same password for confirmation by display this message “Please re-enter the same Pass”.
+Enter a password consists of 5 numbers, Display * in the screen for each number.
![f4](https://github.com/Marcoyacoub01/Door_Locker_Security_System/assets/118049131/36da1589-00b8-467e-b2ac-e48d1eea193c)
+Press enter button (choose any button in the keypad as enter button).
+HMI_ECU should send the two passwords to the Control_ECU through the UART.
+If the two passwords are matched then the system has a password now and save it inside the EEPORM and go to Step 2.
+If the two passwords are unmatched then repeat step 1 again.

> Step2 - Main Options
+The LCD will always display the main system option:
![f5](https://github.com/Marcoyacoub01/Door_Locker_Security_System/assets/118049131/52117204-d3e4-43ab-9a73-fa402844abeb)

> Step3 - Open Door (+)
+The LCD should display “Please Enter Password”.
+Enter the password then press enter button (choose any button in the keypad as enter button).
+HMI_ECU should send the Password to the Control_ECU and it should compare it with the one saved in the EEPROM.
+if two passwords are matched :
   -rotates motor for 15-seconds CW and display a message on the screen “Door is Unlocking”
   -hold the motor for 3-seconds.
   -rotates motor for 15-seconds A-CW and display a message on the screen “Door is Locking”

> Step 4 - Change Password (-)
+The LCD should display “Please Enter Password”.
+Enter the password then press enter button (choose any button in the keypad as enter button).
+HMI_ECU should send the Password to the Control_ECU and it should compare it with the one saved in the EEPROM.
+if two passwords are matched:
  -Repeat Step 1.

> Step 5
+If the two passwords are unmatched at step 3 (+ : Open Door) or step 4 (- : Change Password)
+Ask the user one more time for the password.
+The LCD should display “Please Enter Password” like that:
+Enter the password then press enter button (choose any button in the keypad as enter button).
+HMI_ECU should send the password to the Control_ECU and it should compare it with the one saved in the EEPROM.
+If two passwords are matched then open the door or change the password in steps 3 and 4.
+If the two passwords are not matched again then ask the user one last time for the password.
+If two passwords are matched then open the door or change the password in steps 3 and 4.
+If the two passwords are not matched for the third consecutive time, then:
  -Activate Buzzer for 1-minute.
  -Display error message on LCD for 1 minute.
  -System should be locked no inputs from Keypad will be accepted during this time period.
  -Go to Step 2 the main options again.


