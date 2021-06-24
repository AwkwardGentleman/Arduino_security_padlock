# Arduino Security System

## Description
An electronic security padlock implemented with an Arduino.

## Operating stages
 1. A password is entered (the default value: 1234) performed by pressing the numbers.
 2. If correct, the motor will operate alternating between locking and unlocking.
 3. It switches by using a count system under lock_count, which is by default 0.
 4. Once entered correctly the count is incremented by 1, but if entered correctly again the count is then re-set to 0. 0 = locked, 1 = unlocked. This is accomplished by the motor gets power to either line, whilst the other is set as ground.
 5. If the password is entered incorrectly a buzzer is sounded and the failed attempt is logged. This is done by using failed_password, the attempts can be viewed by pressing B. 
 6.	Attempts are re-set once the correct password is entered. This allows the user to see if anyone has tried to use the system before them.
 7.	The password can also be re-set by pressing the A key, where the user has to re-enter the password to confirm.

## Wiring diagram
![wires](https://github.com/MaybeNotABob/Arduino_security_padlock/blob/main/wiring-diagram/Scaled-wiring-diagram.png)
