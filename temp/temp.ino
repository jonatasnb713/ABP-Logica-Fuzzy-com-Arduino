float temperatura, tensao = 0;
int sensor_lm35 = 0;

void setup() {
  Serial.begin(9600);
}            

void loop() {

  sensor_lm35 = analogRead(A0);

  //Conversao da leitura do sensor em tensão V
  temperatura = (sensor_lm35/1024.0)*5.0*100; 


  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  delay(1000);
}
