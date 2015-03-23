const int led_pins[] = {11, 10, 9, 6, 5, 3};
const int led_count = sizeof(led_pins) / sizeof(int);
const int step_size = 8;

int value = 0;
long last_input = 0;

void setup()
{
  Serial.setTimeout(100);
  Serial.begin(4800);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (int i = 0; i < led_count; ++i) {
    pinMode(led_pins[i], OUTPUT);
    analogWrite(led_pins[i], 0);
  }
}

void loop()
{
  if (Serial.available() > 0) {
    value = Serial.parseInt();
    last_input = millis();
  }
  
  long dt = millis() - last_input;
  if (dt < 0 || dt > 1000)
    value = 0;

  int led_level = map(value, 0, 1000, 0, led_count * step_size);
  
  for (int i = 0; i < led_count; ++i) {
    int rpins = led_level / step_size;
    if (i < rpins) {
      analogWrite(led_pins[i], 255);
    } else if (i == rpins) {
      int res = led_level % step_size;
      analogWrite(led_pins[i], (1 << res) - 1);
    } else {
      analogWrite(led_pins[i], 0);
    }
  }
}
