#include <stdio.h>

volatile long ticks = 0;
volatile uint8_t state_old = 0;
volatile uint8_t state_new = 0;
volatile bool dir = true;
volatile unsigned long timestamp = 0;

String in;
float r = 0.0;
float relative_change = 0;

void setup() {
  // Setup Serial Port
  Serial.begin(115200);
  
  // Setup Encoder Interrupts
  pinMode(2, INPUT);
  digitalWrite(2, LOW);
  pinMode(3, INPUT);
  digitalWrite(3, LOW);
  attachInterrupt(digitalPinToInterrupt(2), encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoder_interrupt, CHANGE);

  // Initialize Encoder Readings
  state_old = 0;
  if (PIND & (1 << PD2))
    state_old |= 0x01;
  if (PIND & (1 << PD3))
    state_old |= 0x02;

  // Initialize PWM - Fast-PWM Mode
  TCCR1A = 0x00;
  TCCR1A |= _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11); // Enable Outputs, Set Mode to 9-Bit Fast PWM
  TCCR1B = 0X00;
  TCCR1B |= _BV(WGM12) | _BV(CS10); // Prescaler Off
  pinMode(9, OUTPUT);

  // Setup Direction Output
  pinMode(8, OUTPUT);
}

class YValid {
  private:
    unsigned long valid_since;
  public:
  YValid() {
    valid_since = 0;
  }
  
  int is_valid(float r, float y)
  {
    if (y > (r - (r*0.004)) && y < (r + (r*0.004))) // 0.025
    {
      if (valid_since == 0)
      {
        valid_since = micros(); 
      }

      unsigned long dT = micros() - valid_since;
      
      if (dT > 50000) // 60000
      {
        return 1;
      }
      else 
      {
        return 0;
      }
    } else {
    valid_since = 0;
    }
    return 0;
  }
};

YValid checker = YValid();

void loop() {
  if( Serial.available()){
    in = Serial.readStringUntil('\n');
    r = in.toFloat();
    //if (r > 2*PI) r-=2*PI;
    //if (r<0) r+=2*PI;
  }

  unsigned long dT = micros() - timestamp;

  if (dT >= 10000)
  {
    float y = encoder_radian();
    float e = r - y;
    float u = e*22; // 18
    motor(u);
    timestamp = micros();
    send_y(r, y);
  }
}

void send_y(float r, float y){
  char ystr[6];
  dtostrf(y, 5, 2, &ystr[0]);
  char debug[8];
  Serial.print(checker.is_valid(r, y));
  Serial.print(',');
  //sprintf(debug, "%s", ystr);
  Serial.print(ystr);
  Serial.print(',');
  char rstr[6];
  dtostrf(r, 5, 2, &rstr[0]);
  Serial.println(rstr);
}

void debug(float r, float y, float e, float u)
{
  char rstr[6];
  char ystr[6];
  char estr[6];
  char ustr[6];
  dtostrf(r, 5, 2, &rstr[0]);
  dtostrf(y, 5, 2, &ystr[0]);
  dtostrf(e, 5, 2, &estr[0]);
  dtostrf(u, 5, 2, &ustr[0]);

  char debug[26];
  sprintf(debug, "%s,%s,%s,%s", ystr, rstr, estr, ustr);
  Serial.println(debug);
}

void debug_ticks(long ticks, float y)
{
  char ystr[8];
  char debug[42];
  dtostrf(y, 7, 4, &ystr[0]);
  sprintf(debug, "y in ticks: %lu ; y in radians: %s", ticks, ystr);
  Serial.println(debug);   
}

void motor(float voltage)
{
  // Saturation
  if (voltage > 12.0)
    voltage = 12.0;
  if (voltage < -12.0)
    voltage = -12.0;

  if (voltage > 0)
    digitalWrite(8, LOW);
  else
    digitalWrite(8, HIGH);

  volatile short out = abs((short)(voltage * 42.5833));
  OCR1AH = out >> 8;
  OCR1AL = (unsigned char)out;
}

float encoder_radian()
{
  return ((float)ticks) * 0.00153398;
}

long encoder_ticks()
{
  return ticks;
}

void encoder_interrupt()
{
  state_new = 0;
  if (PIND & (1 << PD2))
    state_new |= 0x01;
  if (PIND & (1 << PD3))
    state_new |= 0x02;

    switch (state_new)
  {
    case 1:
      if (state_old == 3)
        ticks++;
      else if (state_old == 0)
        ticks--;
      break;
    case 0:
      if (state_old == 1)
        ticks++;
      else if (state_old == 2)
        ticks--;
      break;
    case 2:
      if (state_old == 0)
        ticks++;
      else if (state_old == 3)
        ticks--;
      break;
    case 3:
      if (state_old == 2)
        ticks++;
      else if (state_old == 1)
        ticks--;
      break;
  }
  state_old = state_new;
  //if (ticks == 4096) ticks = 0;
  //if (ticks == -1) ticks = 4095;
}
