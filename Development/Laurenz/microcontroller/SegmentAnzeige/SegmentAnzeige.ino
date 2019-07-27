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

void setup() {
  // put your setup code here, to run once:
  // set output mode
  Serial.begin(9600);
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

  digitalWrite(dp1, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(dp1, HIGH);
  for (float i = 0.0; i<10.0;i=i+0.1)
  {
    display_y(i);
    delay(250);
  }    
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
