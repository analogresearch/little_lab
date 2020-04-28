//////////////////////
/// LITTLE████_LAB ///
//////////////////////
/// version 0.2 //////
//////////////////////
/// stef ♥ 2020 //////
//////////////////////

byte midi_channel;
byte program;

int in1, in2, in3;
int out1, out2, out3;

void setup() {
  Serial.begin(19200);

  setPwmFrequency(5, 8);
  setPwmFrequency(6, 8);
  setPwmFrequency(3, 8);

  //lb inputs
  pinMode(A0, INPUT); //in1
  pinMode(A1, INPUT); //in2
  pinMode(A2, INPUT); //in3

  //lb outputs
  pinMode(6, OUTPUT); //out1
  pinMode(5, OUTPUT); //out2
  pinMode(3, OUTPUT); //out3

  //schalter für midi channel
  pinMode(A3, INPUT_PULLUP); //1
  pinMode(A4, INPUT_PULLUP); //2
  pinMode(A5, INPUT_PULLUP); //4
  pinMode(A6, INPUT_PULLUP); //8

  //schalter für program nr
  pinMode(A7, INPUT_PULLUP); //1
  pinMode(8, INPUT_PULLUP);  //2
  pinMode(12, INPUT_PULLUP); //4
  pinMode(13, INPUT_PULLUP); //8

  set_address();
  set_program();
}

void loop() {
  set_address();
  set_program();
  Serial.print("m:\t");
  Serial.print(midi_channel);
  Serial.print("\tp:\t");
  Serial.print(program);

  switch (program) {
    case 1:
      //testprogramm 1

      in1 = analogRead(A0);
      in2 = analogRead(A1);
      in3 = analogRead(A2);

      out1 = in1 / 4;
      out2 = in2 / 4;
      out3 = in3 / 4;

      analogWrite(6, out1);
      analogWrite(5, out2);
      analogWrite(3, out3);

      Serial.print("\tin:\t");
      Serial.print(in1);
      Serial.print("\t");
      Serial.print(in2);
      Serial.print("\t");
      Serial.print(in3);

      Serial.print("\tout:\t");
      Serial.print(out1);
      Serial.print("\t");
      Serial.print(out2);
      Serial.print("\t");
      Serial.println(out3);

      delay(100);
      break;
    case 2:
      //testprogramm 2
      //do nothing
      break;
    default:
      //do nothing
      break;
  }
}

void set_address() { // midi codeswitch auslesen
  midi_channel = !digitalRead(A3) + (!digitalRead(A4) << 1) + (!digitalRead(A5) << 2 ) + (!(analogRead(A6) > 512) << 3);
  if (midi_channel == 0) midi_channel = 16;
}

void set_program() { // program codeswitch auslesen
  program = !(analogRead(A7) > 512) + (!digitalRead(8) << 1) + (!digitalRead(12) << 2 ) + (!digitalRead(13) << 3);
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
