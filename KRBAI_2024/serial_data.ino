void serialEvent() {
   while (Serial.available()) {
     char inChar = (char)Serial.read();
     inputString1 += inChar;
     if (inChar == '\n') {
      if(inputString1.substring(0,2)=="xx"){
         
      }
      else if(inputString1.substring(0,2)=="pi"){
        //  thrus_pi.writeMicroseconds(1700); //naik
        pitch_status =! pitch_status;
        Serial.print("Pitch_status = ");Serial.println(pitch_status);
      }
      else if(inputString1.substring(0,2)=="de"){
        //  thrus_pi.writeMicroseconds(1700); //naik
        depth_status =! depth_status;
        Serial.print("depth_status = ");Serial.println(depth_status);
      }
      else if(inputString1.substring(0,2)=="he"){
        //  thrus_pi.writeMicroseconds(1700); //naik
        head_status =! head_status;
        Serial.print("head_status = ");Serial.println(head_status);
        berhenti();
      }
      else if(inputString1.substring(0,2)=="la"){
          nilai =! nilai;
          lampu(nilai);
          // lampu(nyala);
      }
      else if(inputString1.substring(0,2)=="so"){
          kondisi =! kondisi;
//          sos(nilai);
          // lampu(nyala);
      }
      else if(inputString1.substring(0,2)=="lo"){
          myservo.write(10);
      }
      else if(inputString1.substring(0,2)=="lx"){
          myservo.write(83);
      }
      else if(inputString1.substring(0,2)=="ki"){
          thrus_ki.writeMicroseconds(1900); //turun
      }
      else if(inputString1.substring(0,2)=="ka"){
          thrus_ka.writeMicroseconds(1900); //turun
      }
      else if(inputString1.substring(0,2)=="sa"){
          thrus_1.writeMicroseconds(1100); //mundur
      }
      else if(inputString1.substring(0,2)=="du"){
          thrus_2.writeMicroseconds(1900); //mundur
      }
      else if(inputString1.substring(0,2)=="ti"){
          thrus_3.writeMicroseconds(1900); //maju
      }
      else if(inputString1.substring(0,2)=="em"){
          thrus_4.writeMicroseconds(1900); //mundur
      }
      else if(inputString1.substring(0,2)=="ma"){
          maju();
      }
      else if(inputString1.substring(0,2)=="mu"){
          mundur();
      }
      else if(inputString1.substring(0,2)=="sr"){
          geser_kanan();
      }
      else if(inputString1.substring(0,2)=="sl"){
          geser_kiri();
      }
      else if(inputString1.substring(0,2)=="rr"){
          putar_kanan();
      }
      else if(inputString1.substring(0,2)=="rl"){
          putar_kiri();
      }
      else if(inputString1.substring(0,2)=="st"){
        thrus_ka.writeMicroseconds(1500);
        thrus_ki.writeMicroseconds(1500);
        thrus_pi.writeMicroseconds(1500);
        thrus_1.writeMicroseconds(1500);
        thrus_2.writeMicroseconds(1500);
        thrus_3.writeMicroseconds(1500);
        thrus_4.writeMicroseconds(1500);
      }
      else if(inputString1.substring(0,1)=="s"){
        int nilai =inputString1.substring(1).toInt();
//        EEPROM.write(1,nilai);
        set_level = nilai;
        Serial.print("Set_point"); Serial.println(nilai);
      }
      else if(inputString1.substring(0,1)=="p"){
        int pid =inputString1.substring(1).toInt();
        
      }
      inputString1="";
     }
   }
}
     

