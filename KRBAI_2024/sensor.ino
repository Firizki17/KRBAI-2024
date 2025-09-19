void main_sensor(int compass){
  //pressure sensor 14BA
  suhu= sensor.getTemperature(CELSIUS, ADC_1024);     //read temperature from the sensor in celsius.
  pressure_abs = sensor.getPressure(ADC_1024);        // Read pressure from the sensor in mbar.
  myRA.addValue(pressure_abs);
  avg = myRA.getFastAverage();
  water_level = avg - reset_level;
  // water_level = pressure_abs - reset_level;

  if (compass == 1){
    //cmps12
    Wire.beginTransmission(CMPS12);  
    Wire.write(0X02);                     
    Wire.endTransmission(false);
    Wire.requestFrom(CMPS12,4);
    Yaw=(int16_t)(Wire.read()<<8|Wire.read())/10.00; //Two bytes Yaw in range of (0 to 359 degrees) 
    Pitch=((int8_t)Wire.read()); // One byte Pitch in range of (-90 to 90 degrees)
    Roll=((int8_t)Wire.read()); // One byte Roll in range of (-90 to 90 degrees)        
    
    Wire.beginTransmission(CMPS12);  
    Wire.write(0x1C);                     
    Wire.endTransmission(false);
    Wire.requestFrom(CMPS12,2);
    Pitch_2=(int16_t)(Wire.read()<<8|Wire.read()); // Two bytes Pitch in range of (-180 to 180 degrees) 

    Serial.print("Pitch= ");    Serial.print(Pitch);
    // Serial.print(" Roll=");     Serial.print(Roll); 
    Serial.print(" Yaw= ");     Serial.print(Yaw); 
    //  Serial.print(" Pitch_2= "); Serial.print(Pitch_2); 
    Serial.println("");
  }
  else{
    //BNO055
    sensors_event_t event;
    bno.getEvent(&event);
    Yaw = event.orientation.x,0;
    Pitch = event.orientation.y;
    /* Display the floating point data */
    Serial.print("Pitch: ");Serial.print(Pitch);//pitch
    Serial.print("\tYaw: ");Serial.print(Yaw);//heading
    Serial.print("\tZ: ");
    Serial.print(event.orientation.z,0);//roll
    /* Optional: Display calibration status */
    displayCalStatus();
    Serial.println("");
  }
  
//  //Print data serial
//  Serial.print(" Suhu = ");   Serial.print(suhu); 
//  Serial.print(", press_sensor= ");   Serial.print(pressure_abs); 
//  Serial.print(", reset_level= ");   Serial.print(reset_level); 
//  Serial.print(", water_level= ");   Serial.println(water_level); 

//  Serial2.print("Yaw= ");           Serial2.print(Yaw); 
//  Serial2.print(", Pitch= ");       Serial2.println(Pitch);
//  Serial2.print(", water_level= "); Serial2.print(water_level); 
//  Serial.println("");
}
