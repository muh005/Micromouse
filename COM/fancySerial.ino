void setup() {
  // put your setup code here, to run once:
    //pin 19 rx
    //pin 18 tx
    Serial.begin(9600);
    //Serial1.begin(9600);
}

const int sendSize = 100;
char tosend[sendSize]; 
int Status = 0;

void clearAll(char * arg, int sz){

  // Setting all chars in the string to null
  for(int i = 0; i < sz; i++) {
    arg[i] = char(0);
  }
}

void fillCharWithDouble(double val, char * arg, int sz) {

  clearAll(arg, sz);
  //char tmp[100];
  dtostrf(val, 40, 20, arg);
 // snprintf(arg, sz, "Yo: %s", tmp);
  arg[sz-1] = char(0);
}

void sendData(char * arg, int sz) {

 // while(!Serial.available()) {
    // Looping over chars
    for(int i = 0; i < sz; i++) {
      // If see null, then stop printing
      if(arg[i] == char(0)) {
        break;
      } 
      // Print one char at a time
      Serial.print(arg[i]);
    }
    // Leave a line at end
    Serial.println();
    // Delay 
    delay(100);
 // }
}

void clearData(char * arg) {
  // Clearing all data in arg 
  arg[0] = char(0);
  /*for(int i = 0; i < sz; i++) {
    arg[i] = char(0);
  }*/
}

void readAndSerial() {
  // put your main code here, to run repeatedly:
    if(Status == 0){
      int i = 0;
      Status = 1;
      while(Serial.available()) {
        tosend[i++] = char(Serial.read());
      }
      tosend[i] = char(0);
    }
    if(Status == 1) {
      sendData(tosend, strlen(tosend));
      clearData(tosend);
      Status = 0;
    }
}

void sendDoubles() {

  double startTime = millis();

  double ar[] = {42, 13, 9, 0, 7, 1, 99, 53, 43, 250};

  for(int i = 0 ; i < 10 ; i++){
      // Order - time, data

      // Sending time from start
      fillCharWithDouble(millis()-startTime, tosend, sendSize);
      sendData(tosend, sendSize);

      // Sending data
      fillCharWithDouble(ar[i], tosend, sendSize);
      sendData(tosend, sendSize);
  }
}

void loop() {

 // while(!Serial.available()) {
      sendDoubles();
 // }

  // Stopping after 1 iteration
  while(1){}    
}
