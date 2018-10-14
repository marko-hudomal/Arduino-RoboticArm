#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
PS2X ps2x; // create PS2 Controller Class

LiquidCrystal_PCF8574 lcd(0x27);

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;
int val=80;      // variable on custom val
int val2=160;    // variable on custom val
int val3=34;     // variable on custom val
int spd=1;
int l_x,l_y,r_x,r_y;
int show;

void setup(){
 Serial.begin(57600);
  int error;
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  lcd.begin(16, 2);
  
 myservo.attach(9);// attaches the servo on pin 9 to the servo object
 myservo2.attach(7);// attaches the servo on pin 7 to the servo object 
 myservo3.attach(5);// attaches the servo on pin 5 to the servo object 
  
 error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug.");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
     }
  
}

void loop(){
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values  
   you should call this at least once a second
   */
  lcd.setBacklight(255); 
   
   
 if(error == 1) //skip loop if no controller found
  return; 
 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

    /*
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
    */   
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       //Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
       val3-=2*spd;
           lcd.clear();
           lcd.print("ARM UP");
           lcd.display();
           //lcd.cursor();
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
        //Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
        val2-=2*spd;
           lcd.clear();
           lcd.print("ARM RIGHT");
           lcd.display();
          // lcd.cursor();
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
        //Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
        val2+=2*spd;
           lcd.clear();
           lcd.print("ARM LEFT");
           lcd.display();
           //lcd.cursor();
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       //Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
       val3+=2*spd;
           lcd.clear();
           lcd.print("ARM DOWN");
           lcd.display();
           //lcd.cursor();
      
      }   
  
    
     vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
                                              //how hard you press the blue (X) button    
    
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
        /*
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        */

        //Setting spped of robotic arm with R2 button
        if(ps2x.Button(PSB_R2))
         {
          if (spd==1)spd=2;else spd=1;
           lcd.clear();
           lcd.print("SPEED CHANGE");
           lcd.display();
           //lcd.cursor();
         }
    }   
         
    
    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
         //Serial.println("Circle just pressed");
         
    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
    {
       Serial.print("Val=");
       Serial.println(val); 
       Serial.print("Val2=");
       Serial.println(val2); 
       Serial.print("Val3=");
       Serial.println(val3); 
       lcd.clear();
       lcd.print("TEST");
       lcd.display();
       //lcd.cursor();
     }    
    
    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
     {    
        //Serial.println("X just changed"); 
             
     }
    bool l1;
    bool r1;
    if((l1=ps2x.Button(PSB_L1)) || (r1=ps2x.Button(PSB_R1))) // print stick values if either is TRUE
    {
      /*
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC); 
      */
        if (l1) val+=2*spd;
        else if (r1) val-=2*spd;
    } 
    
    //---------------------------------------------------------------------------------------------

  l_x= ps2x.Analog(PSS_LY) -127; l_x = map(l_x, -127, 127, -10, 10);
  l_y= ps2x.Analog(PSS_LX) -128;  l_y = map(l_y, -128, 127, -10, 10);
  r_x= ps2x.Analog(PSS_RY) -127;  r_x = map(r_x, -127, 128, -10, 10);
  r_y= ps2x.Analog(PSS_RX) -128;  r_y = map(r_y, -128, 127, -10, 10);
  lcd.clear();
  lcd.print("l_x,l_y:");
  lcd.print(l_x);
  lcd.print(",");
  lcd.print(l_y);
  lcd.print(",");
  lcd.print(val);
  lcd.display();
  val3+=l_x;
  val2+=l_y;

  //CUSTOM BORDER VALUES
  if  (val>115)
  {
    val=115;
  }
  if  (val<50)
  {
    val=50;
  }
  if  (val2>168)
  {
    val2=168;
  }
  if  (val2<0)
  {
    val2=0;
  }
if  (val3>164)
  {
    val3=164;
  }
  if  (val3<0)
  {
    val3=0;
  }
 
  //Serial.println(val);
   myservo.write(val);                  // sets the servo position according to the scaled value
   myservo2.write(val2);
   myservo3.write(val3);
    
 }
 
 
 delay(50);


}
