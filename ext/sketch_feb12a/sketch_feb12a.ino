void setup() {
   Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  while (Serial.available() > 0) {
    int val = Serial.read();
    if (val >= 'a' && val <= 'z') Serial.write('A' + val - 'a');
    else if (val >= 'A' && val <= 'Z') Serial.write('a' + val - 'A');
    else if (val >= '0' && val <= '8') Serial.write(val + 1);
    else if (val == '9') Serial.write('0');
    
  }




}
