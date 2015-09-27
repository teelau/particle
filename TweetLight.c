TCPClient client; 

int ledPin = D7; //LED pin - change this pin to whatever pin your LED is connected to
String responseLine;
String ledStat;
unsigned long nextPoll = 0;
int toggle = 0;

void setup() { 
    Serial.begin(9600);  
    pinMode(ledPin, OUTPUT); //Define each pin
    delay(2000);
    connectToTwitter();
}

void connectToTwitter() {
  Serial.println("Connecting to Thingspeak...");
  if (client.connect("api.thingspeak.com", 80)){
    Serial.println("Connected to API...");
    client.println("GET /channels/40048/field/1/last.txt HTTP/1.0"); //Change CHANNEL_ID to the ID of your channel!!!
    client.println();
    Serial.println("Connected to channel. Good job!");
  }
  else {
    Serial.println("Connection failed");
  }
  responseLine = "";
  ledStat = "";
}

void readStatus() {
  Serial.println("Entering readStatus");
  while (client.available()) {
    char ch = client.read();
    ledStat += ch;
  };
  client.stop();
  ledStat.trim();
  Serial.println(ledStat);
  if (ledStat == "lighton"){ //Change trigger!!!
    Serial.println("LED is on");
    switchLight(49);
  }
  else if (ledStat == "lightoff"){ //Change trigger!!!
    Serial.println("LED is off");
    switchLight(50);
  }
  else if (ledStat == "lighttoggle"){
    Serial.println("LED toggling");
    switchLight(48);
  }
  else {
    Serial.println("Didn't find the status");
    Serial.println(ledStat);
    Serial.println(ledStat.length());
  };
}

void switchLight (int ledStatus) {
  switch (ledStatus) {
    case 48:
    if(toggle = 1){
      digitalWrite(ledPin, HIGH);
      toggle = 0;
    }
    else{
      digitalWrite(ledPin, LOW);
      toggle = 1;
    }
      break;
    case 49:
      digitalWrite(ledPin, HIGH);
      break;
    case 50:
      digitalWrite(ledPin, LOW);
      break;
    default:
      digitalWrite(ledPin, LOW);
  };
}

void loop() {
  if (nextPoll <= millis()) {
    nextPoll = millis()+30000;
    connectToTwitter();
  };
  if (client.available()) {
    char ch = client.read();
    responseLine += ch;
    Serial.print(ch);
    //if end of line found
    if (ch == 10) {
      Serial.println(responseLine.length());
      if (responseLine.length() == 2) {
        readStatus();
      };
      responseLine = "";
    };
  };
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    Serial.print("got: ");
    Serial.println(cmd);
    switchLight(cmd);
  };
}
