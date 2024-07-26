#include <SoftwareSerial.h>
#include <Keypad.h>
#include <string.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

SoftwareSerial myserial(26,25); // (rx , tx)

#define i2c_Address 0x3c // adress of oled
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys [ROWS] [COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

float I0;
float I;
float Abs;
float Trans;
float con;
float MAC;
float path_length;

char customKey;

byte rowPins[ROWS] = {14,13,4,16}; 
byte colPins[COLS] = {17,18,19,23}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys) , rowPins , colPins , ROWS , COLS );

int phone_no_size = 10;
char phone_no[20];
int data_count = 0;

// SoftwareSerial myserial(17,16); // (rx , tx)

void test_sim800_module();
void updateSerial();
void send_SMS();
void phone_input();
void device_function();

void setup() {
  Serial.begin(115200);
  myserial.begin(115200);
  delay(250); 

  display.begin(i2c_Address, true); // Address 0x3C default

 
  display.display();  // show logo in oled
  delay(2000);  

 
  display.clearDisplay();   // fot clen screen of oled

  display.display();    // start display

  display.clearDisplay();

  display.setTextSize(1);    // set text size 
  display.setTextColor(SH110X_WHITE);    // set text color white
  display.setCursor(0, 0);

  display.setTextColor(SH110X_WHITE); // set text color black and backgroung light on  
  display.print("Hello");   // show background light on
  display.display();       // show above task
  delay(1000);
  display.setTextSize(2);     
  display.setTextColor(SH110X_WHITE);    // set text color whit and backgroung light off
  display.print("world");  // show background linght off
  display.display();   

  display.clearDisplay();
  Serial.println("hello Sir");
  device_function();
  test_sim800_module();
  send_SMS();
  Serial.println("Thanku");
  // GSM.begin(9600);
  // gsm_msg();
}


void loop() {
//  updateSerial();
  // myserial.println("AT+CSQ");
  // myserial.println("AT+CMGF=1"); // Configuring TEXT mode
  // updateSerial();
  // updateSerial();
  // delay(1000);
}

void test_sim800_module()
{
  Serial.println("testing sim function enter");
  myserial.println("AT");
  updateSerial();
  myserial.println("AT+CSQ");
  updateSerial();
  myserial.println("AT+CCID"); //mahendra ki sim ki ccid 8991000920944598631f
  updateSerial();
  myserial.println("AT+CREG?");
  updateSerial();
  myserial.println("AT+COPS?");
  updateSerial();
  myserial.println("ATI");
  updateSerial();
  // myserial.println("AT+SMSC=");
  // updateSerial();
  // myserial.println("AT+CBC");
  // updateSerial();
  Serial.println("testing sim function exit");
}

void updateSerial()
{
  delay(500);
  while (Serial.available()){
   display.setTextSize(1);
   display.setCursor(0, 0);
   delay(1000);
   display.print(myserial.write(Serial.read()));
   display.display();
   Serial.println(myserial.write(Serial.read()));//Forward what Serial received to Software Serial Port
  //  display.display();
   delay(1000);
  }
  while (myserial.available())
  {
    display.setTextSize(1);
    display.setCursor(0, 0);
    delay(1000);
    display.println(Serial.write(myserial.read()));
    display.display();
    Serial.println(Serial.write(myserial.read()));//Forward what Software Serial received to Serial Port 
    // display.display();
    delay(5000);
  }
  display.clearDisplay();
}

void send_SMS()
{
  Serial.println("sending sms function enter");
  // display.println("sending sms function enter");
  String smsc = "+919829003333";
  // myserial.println("AT+CSCA");  //+CSCA: "+919829003333"",145  smsc=+919829003333   
  // updateSerial();
  myserial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  char *number = phone_no;
  String phone_number=String(number);
  myserial.println("AT+CMGS=\""+phone_number+"\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  // myserial.println("AT+CMGS=\"+7307813301\"");
  myserial.print("Hello"); //text content
  updateSerial();
  display.setTextSize(1);
  display.setCursor(0, 0);
  delay(1000);
  display.println("Message Sent");
  display.display();
  Serial.println("Message Sent");
  myserial.write(26);
  display.setTextSize(1);
  display.setCursor(0, 0);
  delay(1000);
  display.println("sending sms function exit");
  display.display();
  Serial.println("sending sms function exit");
  delay(5000);
  display.clearDisplay();

//   display.setTextSize(1);
//   display.setCursor(0, 0);
//   display.println(" SMS sent to your mobile phone successfully ");
//   display.display();
//   delay(5000);
//   display.clearDisplay();
}

void phone_input(){
  Serial.print("phone number input enter");
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Enter your phone number");
  display.display();
  delay(10000);
  display.clearDisplay();
  Serial.println("Enter your phone number ");
  // display.display();
  Serial.println("press key * to enter + then enter your country code then continue with your phone number");
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  delay(1000);
  display.println("press key * to enter + then enter your country code then continue with your phone number");
  display.display();
  delay(3000);
  display.clearDisplay();

  delay(15000);
  // display.clearDisplay();
  for(int i=1;i>0;i++){
    char j = customKeypad.getKey();
    if(j){
      if(j=='*'){
        j='+';
      }
      phone_no[data_count] = j;
      Serial.println(j);
      display.setTextSize(1);
      display.setCursor(0, 1);
      delay(1000);
      display.println(j);
      display.display();
      display.clearDisplay();        
      data_count=data_count+1;
      if(data_count>=13){
        Serial.println(phone_no);
        display.setTextSize(1);
        display.setCursor(0, 0);
        delay(1000);
        display.println(phone_no);
        display.display();
        delay(5000);
        display.clearDisplay();
        break;
      }
    }
  }
  data_count=0;
  Serial.println("out of for");
  delay(1000);
  char *number = phone_no;
  String phone_number=String(number);

  Serial.print(phone_number);
  Serial.print("phone input exit");
}

// void gsm_msg(){
//   char *number = phone_no;
//   char *text ="hello";
//   bool msg = GSM.sendSms(number,text);
//   Serial.println(msg);
// }

void device_function()
{
  Serial.println("entering device function ");
  // display.setTextSize(1);
  //       display.setCursor(0, 0);
        Serial.println("Press Key A to take Calibration reading ");
        display.setTextSize(1);
        display.setCursor(0, 0);
        delay(1000);
        display.println("Press Key A to take Calibration reading ");
        display.display();
      
        // display.display();
        delay(2000);
        // display.clearDisplay();
  for(int i=1;i>0;i++){
  customKey = customKeypad.getKey();
    if(customKey){
        Serial.println(customKey);
      if(customKey == 'A'){
        I0 = analogRead(33);
        Serial.println(I0);
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        delay(1000);
        display.println("The value of I0 is: ");
        display.display();
        // display.setTextSize(1);
        // display.setCursor(0, 0);
        delay(1000);
        display.println(I0);
        display.display();
        // display.setTextSize(1);
        // display.setCursor(0, 0);
        display.println("Reading of I0 taken sucessfully");
        display.display();
        delay(10000);
        display.clearDisplay();

        Serial.println("The value of I0 is: ");
        // display.display();
        Serial.println(I0);
        Serial.println("Reading of I0 taken sucessfully");
        // display.display();
        delay(30000);
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Put cuvatte");
        display.display();
        Serial.println("Press key B to take reading of sample ");
        display.display();
        delay(15000);
        display.clearDisplay();
        display.println("Press key B to take reading of sample ");
        display.display();
      
        // I0 = 'i';
        // Serial.println(I0);
        // // break;
      }
      else if (customKey == 'B'){
         I = analogRead(33); 
        //  Serial.println(I);
        //  Serial.println("reading of I is taken sucessfully");
        display.clearDisplay();
         display.setTextSize(1);
         display.setCursor(0, 0);
         display.println("Reading of I is taken sucessfully");
         display.display();
         display.println("the value of I is:");
         display.display();
         display.println(I);
         display.display();        
         delay(10000);
         display.clearDisplay();
         Serial.println("reading of I is taken sucessfully");
        //  display.display();
         Serial.println("the value of I is: ");
         Serial.println(I);
        //  display.display();


        //  delay(10000);
        //  display.clearDisplay();

         Trans = I0/I;
         Abs =  log(Trans);
        //  Serial.println("the value of transmittance is ");
        //  Serial.println(Trans);

         display.setTextSize(1);
         display.setCursor(0, 0);
         delay(1000);
         display.println("The value of transmittance is: ");
         display.display();
         display.println(Trans);
         display.display();
         display.println("The value of Absorbance is: ");
         display.display();
         display.println(Abs);
         display.display();
         delay(10000);
         display.clearDisplay();
         Serial.println("The value of transmittance is: ");
        //  display.display();
         Serial.println(Trans);
        //  display.display();
        Serial.println("the value of absorbance is: ");
        //  display.display();
         Serial.println(Abs);
        //  display.display();
         
        //  display.clearDisplay();

         display.setTextSize(1);
         display.setCursor(0, 0);
         display.println("Press Key C to send result through SMS ");
         display.display();
         Serial.println("Press Key C to send result through SMS ");
        //  display.display();
        //  delay(10000);
        //  display.clearDisplay();
         
        //  Serial.println(Abs);

        //  break;
      }

      else if(customKey == 'C'){
        display.clearDisplay();
        phone_input();
        send_SMS();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Press key # to end the testing");
        display.display();
        delay(5000);
        display.clearDisplay();

      }
      else if(customKey == '#'){
        // Serial.println(customKey);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Thank You ");
        display.display();
        delay(6000);
        display.clearDisplay();
        break;
      }
    }
  }
  Serial.println("exiting for loop");
  delay(2000);
}
