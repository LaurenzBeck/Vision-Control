#include <stdio.h>

volatile long ticks = 0;
volatile uint8_t state_old = 0;
volatile uint8_t state_new = 0;
volatile bool dir = true;
volatile unsigned long timestamp = 0;

String in;
float r = 0.0;
float relative_change = 0;

//control variables
volatile float u = 0;
volatile float e_1 = 0;
volatile float u_1 = 0;
volatile float e_2 = 0;
volatile float u_2 = 0;

//display pins
int a1  = 15;
int b1  = 16;
int c1  = 5;
int d1  = 6;
int e1  = 7;
int f1  = 14;
int g1  = 13;
int dp1 = 4;
int a2  = 19;
int b2  = 20;
int c2  = 21;
int d2  = 22; // zu löten
int e2  = 10; // zu löten
int f2  = 18;
int g2  = 17;
int display_counter = 0;

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

  //Setup Display Pins
  pinMode(a1,  OUTPUT);
  pinMode(b1,  OUTPUT);
  pinMode(c1,  OUTPUT);
  pinMode(d1,  OUTPUT);
  pinMode(e1,  OUTPUT);
  pinMode(f1,  OUTPUT);
  pinMode(g1,  OUTPUT);
  pinMode(dp1, OUTPUT);
  pinMode(a2,  OUTPUT);
  pinMode(b2,  OUTPUT);
  pinMode(c2,  OUTPUT);
  pinMode(d2,  OUTPUT);
  pinMode(e2,  OUTPUT);
  pinMode(f2,  OUTPUT);
  pinMode(g2,  OUTPUT);
  pinMode(11,  OUTPUT);

  digitalWrite(dp1, HIGH);
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
    if (y > (r - (r*0.007)) && y < (r + (r*0.007)))
    {
      if (valid_since == 0)
      {
        valid_since = micros(); 
      }

      unsigned long dT = micros() - valid_since;
      
      if (dT > 50000)
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
  //Setpoint over Serial
  if( Serial.available()){
    in = Serial.readStringUntil('\n');
    r = in.toFloat();
  }

  unsigned long dT = micros() - timestamp;
  if (dT >= 5000)
  {
    float y = encoder_radian();
    float e = r - y;
    //float u = e*22; // 18
    //u =  15 * e + 0.5 * e_1 - 8 * e_2 + 1.01 * u_1 - 0.36 * u_2;
    u =  15 * e + 0.5 * e_1 - 9 * e_2 + 1.2 * u_1 - 0.6 * u_2;
    motor(u);
    timestamp = micros();
    u_2 = u_1;
    e_2 = e_1;
    u_1 = u;
    e_1 = e;
    send_y(r, y);
    display_counter += 1;
    if(display_counter%20) display_y(y);
  }
}

void send_y(float r, float y){
  char ystr[6];
  dtostrf(y, 5, 3, &ystr[0]);
  char debug[8];
  digitalWrite(11, checker.is_valid(r, y) ? HIGH : LOW);
  Serial.print(checker.is_valid(r, y));
  Serial.print(',');
  //sprintf(debug, "%s", ystr);
  Serial.print(ystr);
  Serial.print(',');
  char rstr[6];
  dtostrf(r, 5, 3, &rstr[0]);
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

void display_y(float y){
  int digit1 = y;
  int digit2 = (y - digit1) * 10;
  display_segment1(digit1);
  display_segment2(digit2);
  }

void display_segment1(int digit){
  switch(digit){
    case 0:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, HIGH);
      digitalWrite(f1, HIGH);
      digitalWrite(g1, LOW);
      break;
    case 1:
      digitalWrite(a1, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, LOW);
      digitalWrite(e1, LOW);
      digitalWrite(f1, LOW);
      digitalWrite(g1, LOW);
      break;
    case 2:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, LOW);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, HIGH);
      digitalWrite(f1, LOW);
      digitalWrite(g1, HIGH);
      break;
    case 3:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, LOW);
      digitalWrite(f1, LOW);
      digitalWrite(g1, HIGH);
      break;
    case 4:
      digitalWrite(a1, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, LOW);
      digitalWrite(e1, LOW);
      digitalWrite(f1, HIGH);
      digitalWrite(g1, HIGH);
      break;
    case 5:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, LOW);
      digitalWrite(f1, HIGH);
      digitalWrite(g1, HIGH);
      break;
    case 6:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, HIGH);
      digitalWrite(f1, HIGH);
      digitalWrite(g1, HIGH);
      break;
    case 7:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, LOW);
      digitalWrite(e1, LOW);
      digitalWrite(f1, LOW);
      digitalWrite(g1, LOW);
      break;
    case 8:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, HIGH);
      digitalWrite(f1, HIGH);
      digitalWrite(g1, HIGH);
      break;
    case 9:
      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(c1, HIGH);
      digitalWrite(d1, HIGH);
      digitalWrite(e1, LOW);
      digitalWrite(f1, HIGH);
      digitalWrite(g1, HIGH);
      break;
    }
  }

void display_segment2(int digit){
  switch(digit){
    case 0:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, HIGH);
      digitalWrite(f2, HIGH);
      digitalWrite(g2, LOW);
      break;
    case 1:
      digitalWrite(a2, LOW);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, LOW);
      digitalWrite(e2, LOW);
      digitalWrite(f2, LOW);
      digitalWrite(g2, LOW);
      break;
    case 2:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, LOW);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, HIGH);
      digitalWrite(f2, LOW);
      digitalWrite(g2, HIGH);
      break;
    case 3:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, LOW);
      digitalWrite(f2, LOW);
      digitalWrite(g2, HIGH);
      break;
    case 4:
      digitalWrite(a2, LOW);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, LOW);
      digitalWrite(e2, LOW);
      digitalWrite(f2, HIGH);
      digitalWrite(g2, HIGH);
      break;
    case 5:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, LOW);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, LOW);
      digitalWrite(f2, HIGH);
      digitalWrite(g2, HIGH);
      break;
    case 6:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, LOW);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, HIGH);
      digitalWrite(f2, HIGH);
      digitalWrite(g2, HIGH);
      break;
    case 7:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, LOW);
      digitalWrite(e2, LOW);
      digitalWrite(f2, LOW);
      digitalWrite(g2, LOW);
      break;
    case 8:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, HIGH);
      digitalWrite(f2, HIGH);
      digitalWrite(g2, HIGH);
      break;
    case 9:
      digitalWrite(a2, HIGH);
      digitalWrite(b2, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(e2, LOW);
      digitalWrite(f2, HIGH);
      digitalWrite(g2, HIGH);
      break;
    }
}
