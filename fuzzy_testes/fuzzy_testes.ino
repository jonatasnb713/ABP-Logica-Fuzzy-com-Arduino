#include <Fuzzy.h>

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput temperatura
FuzzySet *frio = new FuzzySet(-10, 0, 10);
FuzzySet *meio_frio = new FuzzySet(0, 10, 20);
FuzzySet *agradavel = new FuzzySet(18, 23, 28);
FuzzySet *quente = new FuzzySet(20, 30, 40);
FuzzySet *muito_quente = new FuzzySet(30, 40, 50);

// FuzzyInput luminosidade
FuzzySet *escuro = new FuzzySet(0, 50, 100);
FuzzySet *meio_escuro = new FuzzySet(50, 150, 250);
FuzzySet *normal = new FuzzySet(200, 500, 800);
FuzzySet *claro = new FuzzySet(500, 800, 1000);
FuzzySet *muito_claro = new FuzzySet(800, 1000, 1024);


// FuzzyOutput LED
FuzzySet *muito_baixo = new FuzzySet(0, 0, 0, 64);
FuzzySet *baixo = new FuzzySet(0, 64, 64, 128);
FuzzySet *medio = new FuzzySet(64, 128, 128, 192);
FuzzySet *alto = new FuzzySet(128, 192, 192, 255);
FuzzySet *muito_alto = new FuzzySet(192, 255, 255, 255);

void setup()
{
  Serial.begin(9600);

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


  // Building FuzzyRules
  // Regra 1: Se temperatura é muito_frio E luminosidade é escuro, então LED é muito_alto
  FuzzyRuleAntecedent *temp_muito_frio_e_luminosidade_esuro = new FuzzyRuleAntecedent();
  temp_muito_frio_e_luminosidade_esuro->joinWithAND(temp_muito_frio, luminosidade_escuro);

  FuzzyRuleConsequent *led_muito_alto = new FuzzyRuleConsequent();
  led_muito_alto->addOutput(muito_alto);

  FuzzyRule *fuzzyRule1 = new FuzzyRule(temp_muito_frio_e_luminosidade_esuro, led_muito_alto);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Regra 2: Se temperatura é agradavel OU luminosidade é normal, então LED é medio
  FuzzyRuleAntecedent *temp_agradavel_ou_luminosidade_normal = new FuzzyRuleAntecedent();
  temp_agradavel_ou_luminosidade_normal->joinWithOR(temp_agradavel, luminosidade_normal);

  FuzzyRuleConsequent *led_medio = new FuzzyRuleConsequent();
  led_medio->addOutput(medio);

  FuzzyRule *fuzzyRule2 = new FuzzyRule(temp_agradavel_ou_luminosidade_normal, led_medio);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // Regra 3: Se temperatura é quente E luminosidade é claro, então LED é baixo
  FuzzyRuleAntecedent *temp_quente_e_luminosidade_claro = new FuzzyRuleAntecedent();
  temp_quente_e_luminosidade_claro->joinWithAND(temp_quente, luminosidade_claro);

  FuzzyRuleConsequent *led_baixo = new FuzzyRuleConsequent();
  led_baixo->addOutput(baixo);

  FuzzyRule *fuzzyRule3 = new FuzzyRule(temp_quente_e_luminosidade_claro, led_baixo);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // Regra 4: Se temperatura é muito_quente OU luminosidade é muito_claro, então LED é muito_baixo
  FuzzyRuleAntecedent *temp_muito_quente_ou_luminosidade_muito_claro = new FuzzyRuleAntecedent();
  temp_muito_quente_ou_luminosidade_muito_claro->joinWithOR(temp_muito_quente, luminosidade_muito_claro);

  FuzzyRuleConsequent *led_muito_baixo = new FuzzyRuleConsequent();
  led_muito_baixo->addOutput(muito_baixo);

  FuzzyRule *fuzzyRule4 = new FuzzyRule(temp_muito_quente_ou_luminosidade_muito_claro, led_muito_baixo);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Regra 5: Se temperatura é frio E luminosidade é meio_escuro, então LED é alto
  FuzzyRuleAntecedent *temp_frio_e_luminosidade_meio_escuro = new FuzzyRuleAntecedent();
  temp_frio_e_luminosidade_meio_escuro->joinWithAND(temp_frio, luminosidade_meio_escuro);

  FuzzyRuleConsequent *led_alto = new FuzzyRuleConsequent();
  led_alto->addOutput(alto);

  FuzzyRule *fuzzyRule5 = new FuzzyRule(temp_frio_e_luminosidade_meio_escuro, led_alto);
  fuzzy->addFuzzyRule(fuzzyRule5);
}


void loop()
{
  
  int input1 = random(0, 100);
  int input2 = random(0, 70);

  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\ttemperatura: ");
  Serial.print(input1);
  Serial.print(", Speed: ");
  Serial.print(input2);
  Serial.print(", and Temperature: ");
  Serial.println(input3);

  fuzzy->setInput(1, input1);
  fuzzy->setInput(2, input2);

  fuzzy->fuzzify();

  Serial.println("Input: ");
  Serial.print("\ttemperatura: frio-> ");
  Serial.print(frio->getPertinence());
  Serial.print(", Safe-> ");
  Serial.print(safe->getPertinence());
  Serial.print(", Distant-> ");
  Serial.println(distant->getPertinence());

  Serial.print("\tSpeed: Stoped-> ");
  Serial.print(stopedInput->getPertinence());
  Serial.print(",  Slow-> ");
  Serial.print(slowInput->getPertinence());
  Serial.print(",  Normal-> ");
  Serial.print(normalInput->getPertinence());
  Serial.print(",  Quick-> ");
  Serial.println(quickInput->getPertinence());

  Serial.print("\tTemperature: Cold-> ");
  Serial.print(cold->getPertinence());
  Serial.print(", Good-> ");
  Serial.print(good->getPertinence());
  Serial.print(", Hot-> ");
  Serial.println(hot->getPertinence());

  float output1 = fuzzy->defuzzify(1);

  Serial.println("Output: ");
  Serial.print("\tRisk: Minimum-> ");
  Serial.print(minimum->getPertinence());
  Serial.print(", Average-> ");
  Serial.print(average->getPertinence());
  Serial.print(", Maximum-> ");
  Serial.println(maximum->getPertinence());

  Serial.print("\tSpeed: Stoped-> ");
  Serial.print(stopedOutput->getPertinence());
  Serial.print(",  Slow-> ");
  Serial.print(slowOutput->getPertinence());
  Serial.print(",  Normal-> ");
  Serial.print(normalOutput->getPertinence());
  Serial.print(",  Quick-> ");
  Serial.println(quickOutput->getPertinence());

  Serial.println("Result: ");
  Serial.print("\t\t\tRisk: ");
  Serial.print(output1);
  Serial.print(", and Speed: ");
  Serial.println(output2);

  // wait 2 seconds
  delay(2000);
}