#include <Fuzzy.h>

int luminosidade_leitura = 0;
int saida_led = 3;
float temperatura_lida = 0;
int sensor_lm35 = 0;
int gnd = 7;

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput temperatura
FuzzySet *frio = new FuzzySet(0, 5, 5, 10);
FuzzySet *meio_frio = new FuzzySet(10, 15, 25, 20);
FuzzySet *agradavel = new FuzzySet(18, 23, 23, 28);
FuzzySet *quente = new FuzzySet(20, 30, 30, 40);
FuzzySet *muito_quente = new FuzzySet(30, 40, 40, 50);

// FuzzyInput luminosidade
FuzzySet *muito_claro = new FuzzySet(0, 50, 50, 100);
FuzzySet *claro = new FuzzySet(50, 150, 150, 250);
FuzzySet *normal = new FuzzySet(200, 500, 500, 800);
FuzzySet *meio_escuro = new FuzzySet(500, 800, 800, 1000);
FuzzySet *escuro = new FuzzySet(800, 1000, 1000, 1024);

// FuzzyOutput LED
FuzzySet *muito_baixo = new FuzzySet(0, 32, 32, 50);
FuzzySet *baixo = new FuzzySet(32, 64, 64, 94);
FuzzySet *medio = new FuzzySet(64, 110, 110, 128);
FuzzySet *alto = new FuzzySet(110, 150, 150, 180);
FuzzySet *muito_alto = new FuzzySet(150, 210, 210, 255);


void setup()
{
    Serial.begin(9600);
    pinMode(gnd, OUTPUT);

    // FuzzyInput
    FuzzyInput *temperatura = new FuzzyInput(1);

    temperatura->addFuzzySet(frio);
    temperatura->addFuzzySet(meio_frio);
    temperatura->addFuzzySet(agradavel);
    temperatura->addFuzzySet(quente);
    temperatura->addFuzzySet(muito_quente);
    fuzzy->addFuzzyInput(temperatura);

    // FuzzyInput
    FuzzyInput *luminosidade = new FuzzyInput(2);

    luminosidade->addFuzzySet(escuro);
    luminosidade->addFuzzySet(meio_escuro);
    luminosidade->addFuzzySet(normal);
    luminosidade->addFuzzySet(claro);
    luminosidade->addFuzzySet(muito_claro);
    fuzzy->addFuzzyInput(luminosidade);

    // FuzzyOutput
    FuzzyOutput *led = new FuzzyOutput(1);

    led->addFuzzySet(muito_baixo);
    led->addFuzzySet(baixo);
    led->addFuzzySet(medio);
    led->addFuzzySet(alto);
    led->addFuzzySet(muito_alto);
    fuzzy->addFuzzyOutput(led);
    
    // Regras Fuzzy
    // Regra 1: SE luminosidade é muito_claro OU temperatura é muito_quente, então LED é muito_baixo
    FuzzyRuleAntecedent *muito_claro_muito_quente = new FuzzyRuleAntecedent();
    muito_claro_muito_quente->joinWithOR(muito_claro, muito_quente);

    FuzzyRuleConsequent *led_muito_baixo = new FuzzyRuleConsequent();
    led_muito_baixo->addOutput(muito_baixo);

    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, muito_claro_muito_quente, led_muito_baixo);
    fuzzy->addFuzzyRule(fuzzyRule1);
    
    

    // Regra 2: SE luminosidade é claro OU temperatura é quente, então LED é baixo
    FuzzyRuleAntecedent *claro_quente = new FuzzyRuleAntecedent();
    claro_quente->joinWithOR(claro, quente);

    FuzzyRuleConsequent *led_baixo = new FuzzyRuleConsequent();
    led_baixo->addOutput(baixo);

    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, claro_quente, led_baixo);
    fuzzy->addFuzzyRule(fuzzyRule2);

    

    // Regra 3: SE luminosidade é agradavel OU temperatura é normal, então LED é medio
    FuzzyRuleAntecedent *agradavel_normal = new FuzzyRuleAntecedent();
    agradavel_normal->joinWithOR(agradavel, normal);

    FuzzyRuleConsequent *led_medio = new FuzzyRuleConsequent();
    led_medio->addOutput(medio);

    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, agradavel_normal, led_medio);
    fuzzy->addFuzzyRule(fuzzyRule3);

    

    // Regra 4: SE luminosidade é meio_escuro OU temperatura é meio_frio, então LED é alto
    FuzzyRuleAntecedent *meio_escuro_meio_frio = new FuzzyRuleAntecedent();
    meio_escuro_meio_frio->joinWithOR(meio_escuro, meio_frio);

    FuzzyRuleConsequent *led_alto = new FuzzyRuleConsequent();
    led_alto->addOutput(alto);

    FuzzyRule *fuzzyRule4 = new FuzzyRule(4, meio_escuro_meio_frio, led_alto);
    fuzzy->addFuzzyRule(fuzzyRule4);

    

    // Regra 5: SE luminosidade é escuro OU temperatura é frio, então LED é muito_alto
    FuzzyRuleAntecedent *escuro_frio = new FuzzyRuleAntecedent();
    escuro_frio->joinWithOR(escuro, frio);

    FuzzyRuleConsequent *led_muito_alto = new FuzzyRuleConsequent();
    led_muito_alto->addOutput(muito_alto);

    FuzzyRule *fuzzyRule5 = new FuzzyRule(5, escuro_frio, led_muito_alto);
    fuzzy->addFuzzyRule(fuzzyRule5);
}

void loop()
{  
  digitalWrite(gnd, 0); 
  sensor_lm35 = analogRead(A0);

  temperatura_lida = (sensor_lm35/1024.0)*5.0*100;

  Serial.print("Temperatura: ");
  Serial.println(temperatura_lida);
    
  luminosidade_leitura = analogRead(A1);
  
  delay(100);  
  
    fuzzy->setInput(1, temperatura_lida);
    fuzzy->setInput(2, luminosidade_leitura);

    fuzzy->fuzzify();

    float saida = fuzzy->defuzzify(1);    
    
        
    Serial.print("Valor lido pelo LDR = ");
    Serial.println(luminosidade_leitura);

    Serial.print("Saída: ");
    Serial.println(saida);
    
    Serial.println("");

    tone(saida_led, saida);

    delay(1000);
}
