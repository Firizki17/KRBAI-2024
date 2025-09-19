#include <Servo.h>
#include <Pixy2I2C.h>
Pixy2I2C pixy; 

Servo myservo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Starting...\n");
  myservo.attach(4);
  myservo.write(83);
  pixy.init();
}

void loop() {
  
  int i; 
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
    if(pixy.ccc.blocks[0].m_signature == 1){
      myservo.write(20);
      delay(2000);
    }
//    if (pixy.ccc.blocks[i].m_signature  == sigRed)
//        {
//          digitalWrite(led, HIGH);
//          Serial.println("red");
//        }
  }
  myservo.write(83);

//  myservo.write(83);                  
//  delay(5000);                          
//  myservo.write(175);
//  delay(5000);
}
