void serialEvent2() {
  while (Serial2.available()) {  
    char incomingByte = Serial2.read();
    if (incomingByte == STX) {
      // Baca data sampai ditemukan ETX
      String data = readData();
      if (data.length() > 0) {
        parseData(data);
      }
    }
  }
  
//  if(kamera == "F" && objek == 0){
//    Serial.println("Kamera Horizontal dan objek gate");
//    if(koor_x >320){
//      Serial.println("koordinat X");
//    }
//  }
//  else if (kamera == "F" && objek == 3){
//    Serial.println("Kamera Vertikal dan obstacle");
//  }
//  else if (kamera == "B" && objek == 4){
//    Serial.println("Kamera Vertikal dan objek Drum");
//    Serial.println("Buka Payload");
//  }
  
}

String readData() {
  String data = "";
  while (true) {
    if (Serial2.available() > 0) {
      char c = Serial2.read();
      if (c == ETX) {
        break;
      }
      data += c;
    }
  }
  return data;
}

void parseData(String data) {
  int index = 0;
  String parsed[7];
  
  // Pisahkan data berdasarkan koma
  while (data.length() > 0) {
    int delimiterIndex = data.indexOf(',');
    if (delimiterIndex == -1) {
      parsed[index++] = data;
      data = "";
    } else {
      parsed[index++] = data.substring(0, delimiterIndex);
      data = data.substring(delimiterIndex + 1);
    }
  }

  // Konversi data yang telah diparsing ke tipe yang sesuai
  kamera = parsed[0].charAt(0);
  objek = parsed[1].toInt();
  koor_x = parsed[2].toInt();
  koor_y = parsed[3].toInt();
  width = parsed[4].toInt();
  height = parsed[5].toInt();
  posisi = parsed[6].charAt(0);
  
//   Cetak hasil parsing ke Serial Monitor untuk verifikasi
//  Serial.print("Kamera: ");   Serial.println(kamera);
//  Serial.print("Objek ID: ");  Serial.println(objek);
//  Serial.print("X : ");  Serial.println(koor_x);
//  Serial.print("Y : ");  Serial.println(koor_y);
//  Serial.print("w : ");  Serial.println(width);
//  Serial.print("h : ");  Serial.println(height);
//  Serial.print("Posisi Objek: ");  Serial.println(posisi);
//  Serial.println("");
  
  Serial2.print("Kamera: ");   Serial2.println(kamera);
  Serial2.print("ID: ");  Serial2.println(objek);
  Serial2.print("X : ");  Serial2.println(koor_x);
  Serial2.print("Y : ");  Serial2.println(koor_y);
  Serial2.print("w : ");  Serial2.println(width);
  Serial2.print("h : ");  Serial2.println(height);
  Serial2.print("Posisi Objek: ");  Serial2.println(posisi);
}
