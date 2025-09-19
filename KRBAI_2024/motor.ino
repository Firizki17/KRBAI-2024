void turun(){
  thrus_ki.writeMicroseconds(1700);
  thrus_ka.writeMicroseconds(1700);
}
void naik(){
  thrus_ki.writeMicroseconds(1300);
  thrus_ka.writeMicroseconds(1300);
}
void maju(){
  thrus_1.writeMicroseconds(1300);
  thrus_2.writeMicroseconds(1300);
  thrus_3.writeMicroseconds(1300);
  thrus_4.writeMicroseconds(1300); 
}
void maju_speed(int kec){
//  thrus_1.writeMicroseconds(1100);
//  thrus_2.writeMicroseconds(1100);
//  thrus_3.writeMicroseconds(1100);
//  thrus_4.writeMicroseconds(1100); 
  thrus_1.writeMicroseconds(1500-kec);
  thrus_2.writeMicroseconds(1500-kec);
  thrus_3.writeMicroseconds(1500-kec);
  thrus_4.writeMicroseconds(1500-kec);
}
void mundur(){
  thrus_1.writeMicroseconds(1700);
  thrus_2.writeMicroseconds(1700);
  thrus_3.writeMicroseconds(1700);
  thrus_4.writeMicroseconds(1700);
}
void geser_kanan(){
  thrus_1.writeMicroseconds(1200);
  thrus_2.writeMicroseconds(1800);
  thrus_3.writeMicroseconds(1800);
  thrus_4.writeMicroseconds(1200);
}
void geser_kiri(){
  thrus_1.writeMicroseconds(1800);
  thrus_2.writeMicroseconds(1200);
  thrus_3.writeMicroseconds(1200);
  thrus_4.writeMicroseconds(1800);
}
void putar_kanan(){
  thrus_1.writeMicroseconds(1300);
  thrus_2.writeMicroseconds(1700);
  thrus_3.writeMicroseconds(1300);
  thrus_4.writeMicroseconds(1700);
}
void rr_PID(int kec){
  thrus_1.writeMicroseconds(1500 - kec);
  thrus_2.writeMicroseconds(1500 + kec);
  thrus_3.writeMicroseconds(1500 - kec);
  thrus_4.writeMicroseconds(1500 + kec);
}
void putar_kiri(){
  thrus_1.writeMicroseconds(1700);
  thrus_2.writeMicroseconds(1300);
  thrus_3.writeMicroseconds(1700);
  thrus_4.writeMicroseconds(1300);
}
void lr_PID(int kec){
  thrus_1.writeMicroseconds(1500 + kec);
  thrus_2.writeMicroseconds(1500 - kec);
  thrus_3.writeMicroseconds(1500 + kec);
  thrus_4.writeMicroseconds(1500 - kec);
}
void berhenti(){
//  thrus_ka.writeMicroseconds(1500);
//  thrus_ki.writeMicroseconds(1500);
//  thrus_pi.writeMicroseconds(1500);
  thrus_1.writeMicroseconds(1500);
  thrus_2.writeMicroseconds(1500);
  thrus_3.writeMicroseconds(1500);
  thrus_4.writeMicroseconds(1500);
}

void lampu(int status)
{
  if (status == 1){
    digitalWrite(34, LOW);
  }
  else{
    digitalWrite(34, HIGH);
  }
}

void sos(int status)
{
//  if( currentMillis - previousMillis >= 200)
//  {
//    previousMillis = currentMillis;
//
//    if( count < 10)   // even number to turn led off during longer interval
//    {
//      blink = !blink;
//      digitalWrite( 6, blink ? HIGH : LOW);  // turn bool variable into HIGH and LOW
//    }
//
//    count++;
//    if( count > 20)
//      count = 0;      // reset the variable
//  }
  if (status == 1){
    digitalWrite(32, LOW);
    delay(500);
    digitalWrite(32, HIGH);
    delay(500);
    digitalWrite(32, LOW);
    delay(500);
    digitalWrite(32, HIGH);
    delay(500);
    digitalWrite(32, LOW);
    delay(500);
    digitalWrite(32, HIGH);
    delay(500);
    digitalWrite(32, LOW);
    delay(500);
  }
  else{
    digitalWrite(32, HIGH);
  }
}

