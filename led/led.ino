int led = 3;
int pwm = 255;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  analogWrite(led, pwm);
}
