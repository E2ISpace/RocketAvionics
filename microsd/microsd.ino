#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(4)) { 
    Serial.println("initialization failed!"); 
    while (1);
  }
  Serial.println("initialization done.");

  // 파일을 쓸 준비
  myFile = SD.open("test.txt", FILE_WRITE); 
  
  if (myFile) { 
    Serial.print("Writing to test.txt...");
    myFile.println("OK");
    myFile.close(); 
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }

  // 파일을 읽기 위해 열기
  myFile = SD.open("test.txt");
  
  if (myFile) {
    Serial.println("test.txt:");

    while (myFile.available()) {
      Serial.write(myFile.read()); 
    }
    myFile.close(); 
  } else {
   
    Serial.println("error opening test.txt");
  }
}

void loop() {

}