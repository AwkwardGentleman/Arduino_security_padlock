/*--------------------------How it operates-----------------------------------------------
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
 */
 /*
  * ---------------------Instructions--------------------------------
  * enter the default password 1234 followed by the * key
  * the lock will then lock, or un-lock
  * Press the A key to re-set the password
  * press the B key to view failed attempts
  * press # to go back
  */
//the library
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
/* The Keypad */
const byte ROWS = 4;
const byte COLS = 4;
char customKey;  // Reference for knowing
char hexaKeys[ROWS][COLS] = { //for the layout of the keyboard
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//How the keypad is connected
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/*-------------------------------Constants------------------------------------*/
LiquidCrystal_I2C lcd(0x27, 16, 2);  
const int buzzer = 10; //buzzer to arduino pin 10
int lock_open = 11; // lock open
int lock_close = 12; // lock close
/*-------------------------------Variables------------------------------------*/
byte lock_count = 0; // To change lock and unlock direction
byte failed_password = 0; // Failed password attempts
String password="1234"; //Variable to store the current password
String tempPassword=""; //Variable to store the input password
int doublecheck;    //Check twice the new passoword
boolean armed = false;  //Variable for system state (armed:true / unarmed:false)
boolean input_pass;   //Variable for input password (correct:true / wrong:false)
boolean storedPassword = true;
boolean changedPassword = false;
boolean checkPassword = false;
int i = 1; //variable to index an array

/*----------------------------------Main Code---------------------------------*/

void setup(){
  lcd.init(); 
  lcd.backlight();
  pinMode(lock_open, OUTPUT);
  pinMode(lock_close, OUTPUT);
  lcd.clear();
  lcd.begin(16, 2); //LCD's number of columns and rows 
  //welcome message...
  lcd.setCursor(0,0);
  lcd.print("Door Security");
  lcd.setCursor(0,1);
  lcd.print("   ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Please change  ");
  lcd.setCursor(0,1);
  lcd.print(" password  ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password  ");
  lcd.setCursor(0,1);
  lcd.print(password);
  delay(3000);
}
void loop() { //Main loop
  unlockTheDoor();
}

/********************************FUNCTIONS*************************************/
/*----------------------------------Locking and Unlocking---------------------------------*/
void unlockTheDoor(){
  lockAgain: //goto label
  tempPassword="";
  lcd.clear();
  i=6;
  noTone(buzzer);
  while(!checkPassword){
    lcd.setCursor(0,0);
    lcd.print("Enter Password");
    lcd.setCursor(0,1);
    lcd.print(">");
    customKey = customKeypad.getKey();   //Read pressed keys
    if (customKey != NO_KEY){    //Accept only numbers and * from keypad
      if (customKey == '0' || customKey == '1' || customKey == '2' || customKey == '3' ||
      customKey == '4' || customKey == '5' || customKey == '6' || customKey == '7' ||
      customKey == '8' || customKey == '9' ){
        tempPassword += customKey;
        lcd.setCursor(i,1);
        lcd.print("*");       //replace with *
        i++;
      }
      else if (customKey == 'A'){
        changePassword();
        goto lockAgain;
      }
      else if (customKey == 'B'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Failed attempts");
      lcd.setCursor(0,1);
      lcd.print(failed_password);
      delay(5000);
      lcd.clear();
      goto lockAgain;
      }
      else if (customKey=='#'){
        break;
      }
      else if (customKey == '*'){  //Check for password
      if (password==tempPassword){//If it's correct...
        failed_password = 0;
        if(lock_count == 0){
          lcd.clear(); // clear display
          lcd.print("Locking"); // display "locking"
          digitalWrite(lock_open, HIGH);  // send power to motor
          delay(5000); // for 5 seconds
          digitalWrite(lock_open, LOW); // turn the power off
          lock_count++; // adds 1 to the clock count
          goto lockAgain; //go to code called "lockAgian"
      }
      else{ //If it's correct...
      lcd.clear(); // clear display
      lcd.print("Unlocking"); // display "unlocking"
      digitalWrite(lock_close, HIGH);   // send power to motor
      delay(5000); // for 5 seconds
      digitalWrite(lock_close, LOW); // turn the power off
      lock_count = 0; // re-sets the lock count to 0
      goto lockAgain; //go to code called "lockAgian"
      }
        }
        else{           //if it's false
          lcd.clear(); // clear display
          lcd.print("Wrong Password"); // display "wrong password"
          tone(buzzer,500,200); // buzzer sound
          delay(300); // delay
          tone(buzzer,500,200); // buzzer sound
          delay(300); // delay
          failed_password++; // adds 1 to failed password count
          goto lockAgain; //go to code called "lockAgian" 
        }
      }
    }
  }
}

/*----------------------------------Changing the Password---------------------------------*/
void changePassword(){
  retry: //label for goto
  tempPassword="";
  lcd.clear();
  i=1;
  while(!changedPassword){        //Waiting for current password
    customKey = customKeypad.getKey();   //Read pressed keys
    lcd.setCursor(0,0);
    lcd.print("CURRENT PASSWORD");
    lcd.setCursor(0,1);
    lcd.print(">");
    if (customKey != NO_KEY){
      if (customKey == '0' || customKey == '1' || customKey == '2' || customKey == '3' ||
      customKey == '4' || customKey == '5' || customKey == '6' || customKey == '7' ||
      customKey == '8' || customKey == '9' ){
        tempPassword += customKey;
        lcd.setCursor(i,1);
        lcd.print("*");
        i++;
        tone(buzzer,800,200);       
      }
      else if (customKey=='#'){
        break;
      }
      else if (customKey == '*'){
        i=1;
        if (password==tempPassword){
          storedPassword=false;
          tone(buzzer,500,200);
          newPassword();          //Password is corrent, so call the newPassword function
          break;
        }
        else{               //Try again
          tempPassword="";
          tone(buzzer,500,200);
          delay(300);
          tone(buzzer,500,200);
          delay(300);
          goto retry;
        }
      }
    }
  }
}
String firstpass;
/*----------------------------------Making new password---------------------------------*/
void newPassword(){
  tempPassword="";
  changedPassword=false;
  lcd.clear();
  i=1;
  while(!storedPassword){
    customKey = customKeypad.getKey();   //Read pressed keys
    if (doublecheck==0){
      lcd.setCursor(0,0);
      lcd.print("SET NEW PASSWORD");
      lcd.setCursor(0,1);
      lcd.print(">");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("One more time...");
      lcd.setCursor(0,1);
      lcd.print(">");
    }
    if (customKey != NO_KEY){
      if (customKey == '0' || customKey == '1' || customKey == '2' || customKey == '3' ||
      customKey == '4' || customKey == '5' || customKey == '6' || customKey == '7' ||
      customKey == '8' || customKey == '9' ){
        tempPassword += customKey;
        lcd.setCursor(i,1);
        lcd.print("*");
        i++;
          tone(buzzer,800,200);
      }
      else if (customKey=='#'){
        break;
      }
      else if (customKey == '*'){
        if (doublecheck == 0){
          firstpass=tempPassword;
          doublecheck=1;
          newPassword();
        }
        if (doublecheck==1){
          doublecheck=0;
          if (firstpass==tempPassword){
            i=1;
            firstpass="";
            password = tempPassword; // New password saved
            tempPassword="";//erase temp password
            lcd.setCursor(0,0);
            lcd.print("PASSWORD CHANGED");
            lcd.setCursor(0,1);
            lcd.print("----------------");
            storedPassword=true;
            tone(buzzer,500,400);
            delay(2000);
            lcd.clear();
            break;
          }
          else{
            firstpass="";
            newPassword();
          }
        }
      } 
    }
  }
}
