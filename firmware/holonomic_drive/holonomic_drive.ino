
#define NUM_MOTORS 4
//   ^
//w1---w2
//|     |
//|     |
//w3---w4

int pwm_pins[NUM_MOTORS] = {D2, D4, D5, D7};
int dir_pins[NUM_MOTORS] = {D1, D3, D0, D6};

String data;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");
  for (int i = 0; i < NUM_MOTORS; i++)
  {
    pinMode(dir_pins[i], OUTPUT);
    pinMode(pwm_pins[i], OUTPUT);

    digitalWrite(pwm_pins[i], 0);
  } 
}

void loop()
{
  data = "";
  while(Serial.available())
  {
    delay(1);
    if (Serial.available() > 0)
    {
      char c = Serial.read();
      data += c;
    }
  }
  // We expect input data to be in the form
  // "[w1, w2, w3, w4]"
  if (data.length() > 0)
  {
    // Remove the brackets
    data = data.substring(1, data.length() - 1);
    
    int comma_indices[NUM_MOTORS -1] = {0, 0, 0};
    int comma_count = 0;
    int wheel_speeds[NUM_MOTORS] = {0, 0, 0, 0};
    
    for (int i = 0; i < data.length() - 1; i++)
    {
      if (data[i] == ',' && comma_count < 3)
      {
        comma_indices[comma_count] = i;
        comma_count++;
      }
    }

    Serial.print("Comma count: ");
    Serial.println(comma_count);
    for (int i = 0; i < NUM_MOTORS; i++)
    {
      String raw_data = "";
      if (i == 0)
        raw_data = data.substring(0, comma_indices[i]);
        
      else if (i == NUM_MOTORS - 1)
        raw_data = data.substring(comma_indices[i -1] + 1, data.length());

      else
        raw_data = data.substring(comma_indices[i-1] + 1, comma_indices[i]);

      Serial.println(raw_data);
      wheel_speeds[i] = raw_data.toInt();
    }


    for (int i = 0; i < NUM_MOTORS; i++)
    {
      int wheel_speed = map(wheel_speeds[i], -128, 128, -255, 255);
      if (wheel_speed >= 0 ) {
        analogWrite(pwm_pins[i], wheel_speed);
        digitalWrite(dir_pins[i], LOW);
      } else {
        analogWrite(pwm_pins[i], -wheel_speed);
        digitalWrite(dir_pins[i], HIGH);        
      }
    }
  }
}
