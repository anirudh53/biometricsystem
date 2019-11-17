#include <LiquidCrystal.h>  
LiquidCrystal lcd(13,12,11,10,9,8);
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);
#include <Keypad.h>
#include <Servo.h>

#include <Adafruit_Fingerprint.h>
Servo myservo; 
uint8_t id;

int led=4;
int red=5;

const byte ROWS = 4; /* four rows */
const byte COLS = 4; /* four columns */
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, A5, A4}; 
byte colPins[COLS] = {A0, A1, A2, A3}; 

/* initialize an instance of class NewKeypad */
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
void setup()
{ 
     pinMode(led,OUTPUT);
     pinMode(red,OUTPUT);
    Serial.begin(9600);
  
    lcd.begin(16,2);
    lcd.print(" Ravinder Sirs ");
    lcd.setCursor(0,1);
    lcd.print("Security System");
    delay(2000);
    lcd.clear();
    finger.begin(57600);
     finger.getTemplateCount();
    //customKeypad.addEventListener(keypadEvent);
    lcd.print(" Finding Module ");
    lcd.setCursor(0,1);
    delay(2000);
    if (finger.verifyPassword()) 
    {
      Serial.println("Found fingerprint sensor!");
      lcd.clear();
      lcd.print(" Module Found  ");
      delay(2000);
      lcd.clear();
    } 
    else 
    {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("Module Not Found");
    lcd.setCursor(0,1);
    lcd.print("Check Connections");
    while (1);  
  }
  
}

 
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
void loop()
{
    mat();
       
}


void enroll()
{ 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enroll!!");
  lcd.setCursor(0,1);
  lcd.print("enter id:-");
  delay(3000);
  lcd.clear();

  
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 1000) you want to save this finger as...");
  id = GetNumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

void del()
{ lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter ID which");
  lcd.setCursor(0,1);
  lcd.print("you want to del");
  delay(2000);
  lcd.clear();
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  int id = GetNumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Deleting id ");
  lcd.setCursor(0,1);
  lcd.print(id);
  delay(2000);
  deleteFingerprint(id);
}

void mat()
{  
   for(int i=0;i<5;i++) 
   { 
      lcd.clear();
      lcd.print("Place Finger");
      delay(2000);
      int result=getFingerprintIDez();
      Serial.println(result);
       if(result>=0)
        {
        lcd.clear();
        lcd.print("Matched..!! ");
        digitalWrite(led,HIGH);
        
        stop1();
        
        return;
        
         }    
}
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("place finger for id ");
  lcd.setCursor(0,1);
  lcd.print(id);
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println("Remove finger");
  lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Place same");
  lcd.setCursor(0,1);
  lcd.print("finger again");
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("did not match");
    for(int i=1;i<10;i++)
    {
    digitalWrite(red,HIGH);
    delay(250);
    digitalWrite(red,LOW);
    delay(250);
    }
    delay(3000);
    return p;
    
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Matched..!!");
    delay(3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
    for(int i=1;i<50;i++)
  {
  digitalWrite(led,HIGH);
  delay(250);
  digitalWrite(led,LOW);
  delay(250);
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) { 
  return -1;
 }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  {
  return -1;
  
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  
  {
    
    lcd.clear();
    lcd.print("Finger not found");
    lcd.setCursor(0,1);
    lcd.print("try again");
    for(int i=1;i<10;i++)
    { 
    digitalWrite(red,HIGH);
    delay(250);
    digitalWrite(red,LOW);
    delay(250);
    }
    lcd.clear();
    choice();  
    
      } 
  
    
    
  return finger.fingerID; 
}

int GetNumber()
{  
   int num = 0;
   char key = customKeypad.getKey();
   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            
            lcd.print(key);
            num = num * 10 + (key - '0');
            break;

         case '*':
            num = 0;
            lcd.clear();
            break;
      }

      key = customKeypad.getKey();
   }

   return num;
}

void choice()
{
 while(1)
      {
        
      lcd.setCursor(0,0);
      lcd.print("  Please Enter ");
      lcd.setCursor(0,1);
      lcd.print("  your choice");
      delay(1);
     
     
  
    
          char key=customKeypad.getKey();
          if(key=='A')
            { 
               enroll(); 
            }
               else if(key=='B')
              { 
                  del();
              }
                else if(key=='C')
              { 
               
                      mat();
                }  

      }
}
void stop1()
{ 
  while(1)
  {  
 for(int i=0;i<10;i++) 
   { 
      lcd.clear();
      lcd.print("Place Finger"); 
      lcd.setCursor(0,1);
      lcd.print("to stop");
      delay(2000);
      int result=getFingerprintIDez1();
       if(result>=0)
        {
        lcd.clear();
     
        digitalWrite(led,LOW);
        mat();
        
        return;
        
         }    
  } 
  }
}
int getFingerprintIDez1() {
  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) { 
  return -1;
 }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  {
  return -1;
  
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  
  {
    
  return -1;     
    
      } 
  
    
    
  return finger.fingerID; 
}    
