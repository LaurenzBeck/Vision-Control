String in;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if( Serial.available()){
    in = Serial.readStringUntil('\n');
    if(in.equals("1")) digitalWrite(LED_BUILTIN, HIGH);
    else digitalWrite(LED_BUILTIN, LOW); 
    }
  delay(100);
}
