# Arduino_security_padlock
Arduino security system
For wiring please refer to wiring diagram, instructions below:

 *A password is entered default (1234) this is done by pressing the numbers then pressing *
 *If correct the motor will operate, alternating between locking and unlocking
 *it switches by using a count system under lock_count, which is by default 0
 *once entered correctly the count is increased by 1,  but if entered correctly again
 *the count is then re-set to 0. So if its a 0 it locks, if its a 1 it unlocks
 *to acheive this the motor gets power to either line whislt the other is set as ground
 *If the password is entered incorrectly a buzzer is sounded and the failed attempt is logged.
 *this is done by using failed_password, the attempts can be viewed by pressing B. Also the attemps
 *are re-set once the correct password is entered. This allows the user to see if anyone has tried
 *to use the system before them.
 *The password can also be re-set by pressing the A key, where the user has to re-enter the password
 *to confirm.
