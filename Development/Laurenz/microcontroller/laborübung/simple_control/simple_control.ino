#include <stdio.h>

volatile long ticks = 0;
volatile uint8_t state_old = 0;
volatile uint8_t state_new = 0;
volatile bool dir = true;
volatile unsigned long last = 0;

volatile float e_1 = 0;
volatile float u_1 = 0;
volatile float e_2 = 0;
volatile float u_2 = 0;

class PulseGenerator {
  private:
    double _period;
    double _duty;
    bool _state;
    unsigned long _start;

  public:
    PulseGenerator(double period, double duty) 
    {
      _period = period;
      _duty = duty;
      _state = true;
      _start = millis();
  }

  int step()
  {
    double passed = (double)(millis() - _start)/1000;

    if (passed > (_period*_duty))
    {
      _state = false;
    }

    if (passed > _period)
    {
      _state = true;
      _start = millis();
    }
    return _state ? 1:0;
  }
};

PulseGenerator ref(1, 0.5);

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

void loop() {
  float r = ref.step() * PI/10;
  //float r = 1;
  float y = 0;
  float e = 0;
  float u = 0;

  unsigned long dT = micros() - last;
  if (dT > 5000)
  {
    y = encoder_radian();
    e = r - y;
    //u =  13.37 * e + 0.66 * e_1 - 12.7 * e_2 + 1.1 * u_1 - 0.1 * u_2;
    //u =  16 * e + 0.4 * e_1 - 14 * e_2 + 1.0003 * u_1 - 0.3 * u_2;
    ///u =  15 * e + 0.9 * e_1 - 10.5 * e_2 + 1.0003 * u_1 - 0.3 * u_2; // super kandidat
    //u =  15 * e + 0.9 * e_1 - 10.5 * e_2 + 1.0003 * u_1 - 0.3 * u_2; // super kandidat 2
    //u =  15 * e + 0.5 * e_1 - 10 * e_2 + 1.0006 * u_1 - 0.39 * u_2; // super kandidat 2 5000
    //u =  15 * e + 0.5 * e_1 - 8 * e_2 + 1.0006 * u_1 - 0.39 * u_2; // super kandidat 2 5000
    //u =  15 * e + 0.5 * e_1 - 8 * e_2 + 1.1 * u_1 - 0.36 * u_2; // super kandidat 2 5000
    //u =  15 * e + 0.5 * e_1 - 8 * e_2 + 1.1 * u_1 - 0.62 * u_2; // super kandidat 2 5000
    u =  15 * e + 0.5 * e_1 - 9 * e_2 + 1.2 * u_1 - 0.6 * u_2; // super kandidat 2 5000
    //u =  13.37 * e + 0.66 * e_1 - 12.7 * e_2 + 1.1 * u_1 - 0.1 * u_2;
    //u =  13.37 * e + 0.66 * e_1 - 12.7 * e_2 + 1.1 * u_1 - 0.1 * u_2;
    //u =  13.37 * e + 0.66 * e_1 - 12.7 * e_2 + 1.1 * u_1 - 0.1 * u_2;
    //u= 15 * e;
    motor(u);

    u_2 = u_1;
    e_2 = e_1;

    u_1 = u;
    e_1 = e;
    
    last = micros();
    
    debug(r,y,e,u);
  }
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
  sprintf(debug, "%s,%s", ystr, rstr);
  Serial.println(debug);
}

void motor(float voltage)
{
  // Saturation
  if (voltage > 12.0)
    voltage = 12.0;
  if (voltage < -12.0)
    voltage = -12.0;

  // Toggle Direction
  if (voltage > 0)
    digitalWrite(8, LOW);
  else
    digitalWrite(8, HIGH);

  // Update PWM Register
  volatile short out = abs((short)(voltage * 42.5833));
  OCR1AH = out >> 8;
  OCR1AL = (unsigned char)out;
}

float encoder_radian()
{
  return ((float)ticks) * 0.0015;
}

void encoder_interrupt()
{
  // Get New State
  state_new = 0;
  if (PIND & (1 << PD2))
    state_new |= 0x01;
  if (PIND & (1 << PD3))
    state_new |= 0x02;

  // Depending on Old-State, increase or decrease ticks
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
}
