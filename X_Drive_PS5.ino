#include <ps5Controller.h>

/* ================= PIN DEFINITIONS ================= */
#define DIR1 5
#define DIR2 14
#define DIR3 4
#define DIR4 22

#define PWM1 25
#define PWM2 26
#define PWM3 27
#define PWM4 23

int x, y, w;
int vx = 0, vy = 0, omega = 0;

unsigned long curr = 0, prev = 0;
float m1, m2, m3, m4, maxraw;

void Calculation(int vx, int vy, int omega);
void Motor(float speed, int dirPin, int pwmPin);

void setup() {
  Serial.begin(115200);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(DIR4, OUTPUT);

  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  ps5.begin("90:B6:85:64:59:2B");
  Serial.println("ESP32 X-Drive with PS5 Controller Initialized");
}

void loop() {
  if (ps5.isConnected()) {
    x = ps5.LStickY();
    y = ps5.LStickX();
    w = ps5.RStickX();

    if (abs(x) < 20) x = 0;
    if (abs(y) < 20) y = 0;
    if (abs(w) < 20) w = 0;

    vx = (x * 255) / 127;
    vy = (y * 255) / 127;
    omega = (w * 255) / 127;

    Calculation(vx, vy, omega);

    Motor(m1, DIR1, PWM1);
    Motor(m2, DIR2, PWM2);
    Motor(m3, DIR3, PWM3);
    Motor(m4, DIR4, PWM4);
  }
}

void Calculation(int vx, int vy, int omega) {
  m1 = (-vx + vy + omega);
  m2 = (-vx - vy - omega);
  m3 = (-vx + vy - omega);
  m4 = (-vx - vy + omega);

  maxraw = max(max(abs(m1), abs(m2)), max(abs(m3), abs(m4)));

  if (maxraw > 255) {
    float scale = 255.0 / maxraw;
    m1 *= scale;
    m2 *= scale;
    m3 *= scale;
    m4 *= scale;
  }

  curr = millis();
  if (curr - prev >= 1000) {
    Serial.print("M1: "); Serial.print(m1);
    Serial.print(" M2: "); Serial.print(m2);
    Serial.print(" M3: "); Serial.print(m3);
    Serial.print(" M4: "); Serial.println(m4);
    prev = curr;
  }
}

void Motor(float speed, int dirPin, int pwmPin) {
  if (speed > 0) {
    digitalWrite(dirPin, HIGH);
  } else {
    digitalWrite(dirPin, LOW);
    speed = abs(speed);
  }
  analogWrite(pwmPin, (int)speed);
}
