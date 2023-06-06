int luminosidade = 0;

void setup() {
  pinMode(A1, INPUT);
  Serial.begin(9600);
  }

void loop() {
  luminosidade = analogRead(A1);
  Serial.print("Valor lido pelo LDR = ");
  Serial.println(luminosidade);
  delay(1000);
  }
