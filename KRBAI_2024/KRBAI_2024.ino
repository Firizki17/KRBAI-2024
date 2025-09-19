/*********************************************
 * Project   : Depth Control & Motion Control for Underwater ROV
 * Description : 
 *   Implementation of depth control using pressure sensor (MS5803-14B) 
 *   and motion control using IMU sensors (CMPS12 & BNO055) 
 *   for underwater Remotely Operated Vehicle (ROV).
 *
 * Author    : Muh Firizki
 * Email     : muhfirizki_2226270024@mhs.mdp.ac.id
 * GitHub    : https://github.com/firizki17
 *
 * Last Update : 21 Juni 2024
 * IDE         : Arduino IDE 1.8.5
 * Platform    : Arduino Mega 2560 Pro
 ********************************************/


#include <Wire.h>
#include <Servo.h>
#include <MS5803.h>
#include "I2Cdev.h"
//#include "KalmanFilter.h"
#include "PID_v1.h"
#include "RunningAverage.h"
//#include "MPU6050_6Axis_MotionApps612.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>

#define STX 0x02
#define ETX 0x03

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

char kamera,posisi;
int objek, koor_x, koor_y, width, height;

Servo myservo;

//#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
//bool blinkState = false;

int CMPS12=0x60;
float Pitch,Roll,Yaw,Pitch_2;

RunningAverage myRA(10);
MS5803 sensor(ADDRESS_HIGH);      //ADDRESS_HIGH = 0x76 ;  ADDRESS_LOW  = 0x77

unsigned int suhu;
double pressure_abs;
double water_level;
volatile double set_level;
volatile double set_head;
double reset_level = 0;
double avg;
double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)

Servo thrus_ka; Servo thrus_ki;
Servo thrus_pi; Servo thrus_1;
Servo thrus_2;  Servo thrus_3;  Servo thrus_4;

byte thrus_ka_pin = 11; byte thrus_ki_pin = 4;
byte thrus_pi_pin = 2;  byte thrus_1_pin = 3;
byte thrus_2_pin = 5;   byte thrus_3_pin = 7;
byte thrus_4_pin = 9;

String inputString1 = ""; String dataSensor = "";

int cmps_12 = 1; int bno055 = 0;
int nyala = 1;  int padam = 0;
bool nilai=0;
bool kondisi=0;
unsigned long previousMillis;
unsigned long currentMillis;
long period = 15000;
//long period2 = 15000;
//long period3 = 30000;
int count;  
bool blink;
int i;
int gap_atas, gap_bawah;
int geserkanan_counter = 0;int geserkiri_counter = 0;
int head_tolerance = 3;

//PID Pitch
double pi_Setpoint, pi_Input, pi_Output;
float pitch_kp,pitch_ki,pitch_kd;
PID pitch_PID(&pi_Input, &pi_Output, &pi_Setpoint,pitch_kp,pitch_ki,pitch_kd, DIRECT);

//PID Library parameter
double de_Setpoint, de_Input, de_Output;
float depth_kp,depth_ki,depth_kd;
PID depth_PID(&de_Input, &de_Output, &de_Setpoint,depth_kp,depth_ki,depth_kd, DIRECT);

//PID Heading
double he_Setpoint, he_Input, he_Output;
float head_kp,head_ki,head_kd;
PID head_PID(&he_Input, &he_Output, &he_Setpoint,head_kp,head_ki,head_kd, DIRECT);

boolean head_status = true;
boolean depth_status = true;
boolean pitch_status = true;

//pin 6 sos, pin 14 water sensor
boolean misi = true;
boolean finish = false;
boolean last_mission = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); Serial2.begin(19200);
  Wire.begin();

  //BNO055 compass
  BNO_init();
  
//  pressure sensor
  sensor.reset();     sensor.begin();

  //payload servo 
  myservo.attach(10); myservo.write(83);

  pinMode(32, OUTPUT); digitalWrite(32,HIGH);//SOS
  pinMode(34, OUTPUT); digitalWrite(34,HIGH);//Lampu
  
  Wire.setClock(400000); // I2C clock rate ,You can delete it but it helps the speed of I2C (default rate is 100000 Hz)
  
  main_sensor(bno055);//cmps_12 or bno055
  
  pressure_abs = sensor.getPressure(ADC_1024);
  reset_level = pressure_abs;
  set_level = 40;
  // de_Setpoint = 100;
  de_Input =  pressure_abs;
  //  set_level = EEPROM.read(1);

  thrus_ka.attach(thrus_ka_pin);  thrus_ka.writeMicroseconds(1500);
  thrus_ki.attach(thrus_ki_pin);  thrus_ki.writeMicroseconds(1500);
  thrus_pi.attach(thrus_pi_pin);  thrus_pi.writeMicroseconds(1500);
  thrus_1.attach(thrus_1_pin);  thrus_1.writeMicroseconds(1500);
  thrus_2.attach(thrus_2_pin);  thrus_2.writeMicroseconds(1500);
  thrus_3.attach(thrus_3_pin);  thrus_3.writeMicroseconds(1500);
  thrus_4.attach(thrus_4_pin);  thrus_4.writeMicroseconds(1500);

  head_PID.SetMode(AUTOMATIC);
  head_PID.SetOutputLimits(-300,300);
  head_kp = 15; head_ki = 0; head_kd = 2;
  
  pitch_PID.SetMode(AUTOMATIC);
  pitch_PID.SetOutputLimits(-300,300);
  pitch_kp = 15; pitch_ki = 0; pitch_kd = 1;

  depth_PID.SetMode(AUTOMATIC);
  depth_PID.SetOutputLimits(-300,300);
  depth_kp = 25; depth_ki = 0; depth_kd = 2;

  Serial.print("Set Point depth = ");Serial.println(set_level);
  
  delay(7000);
  Serial.println("OK_Ready!!!");
  lampu(padam);
//  sos(nyala);
  
  main_sensor(bno055);//cmps_12 or bno055
  set_head = Yaw;
  Serial.print("Set Point Heading = ");Serial.println(set_head); 
  Serial2.print("Set Point Heading = ");Serial2.println(set_head); 
}

void loop() {
  // put your main code here, to run repeatedly:
  main_sensor(bno055);
  PID_pitch(pitch_kp, pitch_ki, pitch_kd, Pitch);
  PID_Depth(depth_kp, depth_ki, depth_kd, avg);
  PID_Heading(head_kp, head_ki, head_kd, Yaw);
//  currentMillis = millis();
//  long timer = currentMillis - previousMillis;
  /*
  if (misi){
    Serial.println(timer);
    gap_atas = set_head + 5;
    gap_bawah = set_head - 5;
    if( timer <= period)
    {
//      maju_speed(200);
//      Serial.println("Mode 1");
//      Serial2.println("Mode 1");
      // previousMillis = currentMillis;
      if(head_status){
        if(Yaw < gap_bawah){ 
          rr_PID(abs(he_Output));
        }
        else if (Yaw > gap_atas){
          lr_PID(abs(he_Output));
        }
        else{
          maju_speed(200);
        }
      }
    }
    else if( timer >= period && timer <= period2)
    {
//      geser_kanan();  
//      Serial.println("Mode 2");
//      Serial2.println("Mode 2");
      if(head_status){
        if(Yaw < gap_bawah){ 
          rr_PID(abs(he_Output));
        }
        else if (Yaw > gap_atas){
          lr_PID(abs(he_Output));
        }
        else{
          geser_kiri();
        }
      }
    }
    else if( timer > period2 && timer <= period3 )
    {
//      Serial.println("Mode 3");
//      Serial2.println("Mode 3");
      if(head_status){
        if(Yaw < gap_bawah){ 
          rr_PID(abs(he_Output));
        }
        else if (Yaw > gap_atas){
          lr_PID(abs(he_Output));
        }
        else{
          maju_speed(200);
        }
      }
    }
    else if( timer >= period3)
    {
      misi = false;
//      Serial2.println("Mode 4");
      // previousMillis = currentMillis;
    }
  }

  */
  
  if(pitch_status){
    if(pi_Output>0){
      thrus_pi.writeMicroseconds(1500 + abs(pi_Output));
    }
    else if (pi_Output<0){
      thrus_pi.writeMicroseconds(1500 - abs(pi_Output));
    }
  }
  if(depth_status){
    if(de_Output<0){
      thrus_ka.writeMicroseconds(1500 - abs(de_Output));
      thrus_ki.writeMicroseconds(1500 - abs(de_Output));
    }
    else if (de_Output>0){
      thrus_ka.writeMicroseconds(1500 + abs(de_Output));
      thrus_ki.writeMicroseconds(1500 + abs(de_Output));
    }
  }

//  if(head_status){
//    gap_atas = set_head + head_tolerance;
//    gap_bawah = set_head - head_tolerance;
//    if(Yaw < gap_bawah){ 
//      rr_PID(abs(he_Output));
//      pitch_depth_head();
//    }
//    else if (Yaw > gap_atas){
//      lr_PID(abs(he_Output));
//      pitch_depth_head();
//    }
//    else if (Yaw<gap_atas && Yaw>gap_bawah){
////       maju();      
////====== Gate Atas ======
//      if(kamera == 'F' && objek == 0 && width >800){
////        Serial.println("Kamera Horizontal dan objek gate");
//        Serial2.println("Kamera Horizontal dan objek gate");
//        if(koor_x >520 && koor_x <760){
//          maju_speed(300);
//          Serial.println("Maju200");
////          delay(500);
//        }
//        else if(koor_x>760 && koor_x<1280){
//          do{
//            main_sensor(bno055);
//            pitch_depth_head();
//            geserkanan_head();
//            Serial.println("Geser Kanan");
//            i++;
//          }
//          while (i <50);
//          i=0;
//          objek = 2;
//          Serial.println("Geser Kanan Finish");
//        }
//        else if(koor_x>0 && koor_x<520){
//          do{
//            main_sensor(bno055);
//            pitch_depth_head();
//            geserkiri_head();
//            Serial.println("Geser Kiri");
//            i++;
//          }
//          while (i <50);
//          i=0;
//          objek = 2;
//          Serial.println("Geser Kiri Finish");
//        }
        // else{
//          berhenti();
//          Serial2.println("STOP");
        // }
     }
     
//====== Gate full =======
     else if(kamera == 'F' && objek == 1 && width >800){
      Serial.println("Gate Full");    
      if(koor_x >520 && koor_x <760){
          maju_speed(300);
          Serial.println("Maju200");
//          delay(500);
        }
        else if(koor_x>760 && koor_x<1280){
          do{
            main_sensor(bno055);
            pitch_depth_head();
            geserkanan_head();
            Serial.println("Geser Kanan");
            i++;
          }
          while (i <50);
          i=0;
          Serial.println("Geser Kanan Finish");
        }
        else if(koor_x>0 && koor_x<520){
          do{
            main_sensor(bno055);
            pitch_depth_head();
            geserkiri_head();
            Serial.println("Geser Kiri");
            i++;
          }
          while (i <50);
          i=0;
          Serial.println("Geser Kiri Finish");
        }
        else{
//          berhenti();
//          Serial2.println("STOP");
        }  
     }
     
//====== obstacle ========
//     else if(kamera == 'F' && objek == 4 && width > 400){
////      set_level = 45;
//      Serial.println("obstacle tong");      
//      if(koor_x > 640){
//        do {
//          main_sensor(bno055);
//          pitch_depth_head();
//          geserkiri_head();
//          Serial.println("geser kiri");
//          i++;        
//          Serial.println(i);   
//         }
//         while (i < 80);
//         objek = 2;
//         geserkiri_counter = geserkiri_counter + 1;
//         Serial.println("geser kiri Finish");
//         i=0;
//      }
//      else if(koor_x <640){
//        do {
//          main_sensor(bno055);
//          pitch_depth_head();
//          // geserkanan_head();
//          geserkiri_head();
//          Serial.println("geser kanan");
//          i++;        
//          Serial.println(i);   
//         }
//         while (i < 80);
//         objek = 2;
//         geserkanan_counter = geserkanan_counter + 1;
//         Serial.println("geser kanan Finish");
//         i=0;
//      }
     }
//set timer maju setelah obstacle
//      else if(geserkanan_counter > 1 || geserkiri_counter > 1){
//        misi = true;       
//      }

//===== drum merah kamera depan =====
//     else if(kamera == 'F' && objek == 5 && width > 250 ){
//      Serial2.println("objek drum");
//      set_level = 45;
////      last_mission = true;
//      if(koor_x >540 && koor_x <740){
//          maju_speed(150);
//          Serial.println("Maju200");
//          last_mission = true;
//          delay(500);
 }
//        else if(koor_x>740 && koor_x<1280){
//          do{
//            main_sensor(bno055);
//            pitch_depth_head();
//            geserkanan_head();
//            Serial.println("Geser Kanan");
//            i++;
//          }
//          while (i <20);
//          i=0;
//          objek = 2;
//          Serial.println("Geser Kanan Finish");
//        }
//        else if(koor_x>0 && koor_x<540){
//          do{
//            main_sensor(bno055);
//            pitch_depth_head();
//            geserkiri_head();
//            Serial.println("Geser Kiri");
//            i++;
//          }
//          while (i <20);
//          i=0;
//          objek = 2;
//          Serial.println("Geser Kiri Finish");
//        }
//        else{
////          berhenti();
////          Serial2.println("STOP");
//        }
//     }
//     
//
//     maju_speed(200);
//     Serial.println("Maju200");
////     Serial2.println("Maju50");
//    }
//
//  }
//
//
////===== drum kamera bawah =====
//  if(last_mission){
//    if (kamera == 'B' && objek == 5 && width > 100){
//  //      Serial2.println("Kamera bottom dan objek Drum");
//      Serial.println("Buka Payload");
//      myservo.write(5);
//      set_level = -10;
//      finish = true;
//    }
//  }
//  if (finish){
////      Serial2.println("Kamera bottom dan objek Drum");
//    Serial.println("Finish");
//    myservo.write(5);
////    head_status = false;
//    berhenti();
//    lampu(padam);
////    sos(nyala);
//  }
//
//}

