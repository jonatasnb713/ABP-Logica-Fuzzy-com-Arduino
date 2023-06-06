#include <Fuzzy.h>

int luminosidade = 0;
int sensor_lm35 = 0;
int led = 3;
float temperatura, tensao = 0;

void Read_Temperatura() {
    
    sensor_lm35 = analogRead(A0);

    temperatura = (sensor_lm35/1024.0)*5.0*100; 

    Serial.print("Temperatura: ");
    Serial.println(temperatura);

    delay(1000);

    return(temperatura);
}

void Read_luminosidade() {
    luminosidade = analogRead(A1);
    
    Serial.print("Valor lido pelo LDR = ");
    Serial.println(luminosidade);
    
    delay(1000);

    return(luminosidade);
}

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput temperatura
FuzzySet *frio = new FuzzySet(0, 0, 10, 10);
FuzzySet *meio_frio = new FuzzySet(10, 10, 20, 20);
FuzzySet *agradavel = new FuzzySet(18, 18, 23, 28);
FuzzySet *quente = new FuzzySet(20, 20, 30, 40);
FuzzySet *muito_quente = new FuzzySet(30, 30, 40, 50);

// FuzzyInput luminosidade
FuzzySet *escuro = new FuzzySet(0, 0, 50, 100);
FuzzySet *meio_escuro = new FuzzySet(50, 50, 150, 250);
FuzzySet *normal = new FuzzySet(200, 200, 500, 800);
FuzzySet *claro = new FuzzySet(500, 500, 800, 1000);
FuzzySet *muito_claro = new FuzzySet(800, 800, 1000, 1024);

// FuzzyOutput LED
FuzzySet *muito_baixo = new FuzzySet(0, 0, 64, 64);
FuzzySet *baixo = new FuzzySet(64, 64, 128, 128);
FuzzySet *medio = new FuzzySet(128, 128, 192, 192);
FuzzySet *alto = new FuzzySet(192, 192, 255, 255);
FuzzySet *muito_alto = new FuzzySet(255, 255, 255, 255);


void setup()
{
    Serial.begin(9600);

    pinMode(A1, INPUT);

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
    // Regra 1: Se temperatura é muito_frio OU luminosidade é escuro, então LED é muito_alto
    FuzzyRuleAntecedent *temp_muito_frio_e_luminosidade_escuro = new FuzzyRuleAntecedent();
    temp_muito_frio_e_luminosidade_escuro->joinWithOR(frio, escuro);

    FuzzyRuleConsequent *led_muito_alto = new FuzzyRuleConsequent();
    led_muito_alto->addOutput(muito_alto);

    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, temp_muito_frio_e_luminosidade_escuro, led_muito_alto);
    fuzzy->addFuzzyRule(fuzzyRule1);

    // Regra 2: Se temperatura é agradavel OU luminosidade é normal, então LED é medio
    FuzzyRuleAntecedent *temp_agradavel_ou_luminosidade_normal = new FuzzyRuleAntecedent();
    temp_agradavel_ou_luminosidade_normal->joinWithOR(agradavel, normal);

    FuzzyRuleConsequent *led_medio = new FuzzyRuleConsequent();
    led_medio->addOutput(medio);

    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, temp_agradavel_ou_luminosidade_normal, led_medio);
    fuzzy->addFuzzyRule(fuzzyRule2);

    // Regra 3: Se temperatura é quente OU luminosidade é claro, então LED é baixo
    FuzzyRuleAntecedent *temp_quente_e_luminosidade_claro = new FuzzyRuleAntecedent();
    temp_quente_e_luminosidade_claro->joinWithOR(quente, claro);

    FuzzyRuleConsequent *led_baixo = new FuzzyRuleConsequent();
    led_baixo->addOutput(baixo);

    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, temp_quente_e_luminosidade_claro, led_baixo);
    fuzzy->addFuzzyRule(fuzzyRule3);

    // Regra 4: Se temperatura é muito_quente OU luminosidade é muito_claro, então LED é muito_baixo
    FuzzyRuleAntecedent *temp_muito_quente_ou_luminosidade_muito_claro = new FuzzyRuleAntecedent();
    temp_muito_quente_ou_luminosidade_muito_claro->joinWithOR(muito_quente, muito_claro);

    FuzzyRuleConsequent *led_muito_baixo = new FuzzyRuleConsequent();
    led_muito_baixo->addOutput(muito_baixo);

    FuzzyRule *fuzzyRule4 = new FuzzyRule(4, temp_muito_quente_ou_luminosidade_muito_claro, led_muito_baixo);
    fuzzy->addFuzzyRule(fuzzyRule4);

    // Regra 5: Se temperatura é frio OU luminosidade é meio_escuro, então LED é alto
    FuzzyRuleAntecedent *temp_frio_e_luminosidade_meio_escuro = new FuzzyRuleAntecedent();
    temp_frio_e_luminosidade_meio_escuro->joinWithOR(frio, meio_escuro);

    FuzzyRuleConsequent *led_alto = new FuzzyRuleConsequent();
    led_alto->addOutput(alto);

    FuzzyRule *fuzzyRule5 = new FuzzyRule(5, temp_frio_e_luminosidade_meio_escuro, led_alto);
    fuzzy->addFuzzyRule(fuzzyRule5);
}

void loop()
{
    Read_Temperatura();
    Read_luminosidade();

    fuzzy->setInput(1, temperatura);
    fuzzy->setInput(2, luminosidade);

    fuzzy->fuzzify();

    float saida = fuzzy->defuzzify(1);

    Serial.print("Saída: ");
    Serial.println(saida);
    Serial.println("");

    analogWrite(led, saida);

    delay(2000);
}
