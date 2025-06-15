#define RELAY_EAST 8
#define RELAY_WEST 9
#define LIMIT_EAST 6
#define LIMIT_WEST 7
#define LDR_EAST A0
#define LDR_WEST A1

void setup() {
  Serial.begin(9600);

  pinMode(LDR_EAST, INPUT);
  pinMode(LDR_WEST, INPUT);

  pinMode(RELAY_EAST, OUTPUT);
  pinMode(RELAY_WEST, OUTPUT);

  pinMode(LIMIT_EAST, INPUT_PULLUP);
  pinMode(LIMIT_WEST, INPUT_PULLUP);
}

void loop() {
  int eastLDR = analogRead(LDR_EAST);
  int westLDR = analogRead(LDR_WEST);
  int diff = eastLDR - westLDR;

  int limitEast = digitalRead(LIMIT_EAST); // LOW = pressed
  int limitWest = digitalRead(LIMIT_WEST);

  bool isNight = (eastLDR < 200 && westLDR < 200);

  Serial.println("===== TRACKER STATUS =====");
  Serial.print("LDR East: "); Serial.println(eastLDR);
  Serial.print("LDR West: "); Serial.println(westLDR);
  Serial.print("Diff     : "); Serial.println(diff);
  Serial.print("Limit East: "); Serial.println(limitEast == LOW ? "PRESSED" : "OPEN");
  Serial.print("Limit West: "); Serial.println(limitWest == LOW ? "PRESSED" : "OPEN");
  Serial.print("Night Mode: "); Serial.println(isNight ? "YES" : "NO");

  // === NIGHT MODE ===
  if (isNight) {
    if (limitEast == HIGH) {
      Serial.println("Night detected -> Returning to EAST");
      digitalWrite(RELAY_EAST, HIGH);
      digitalWrite(RELAY_WEST, LOW);
    } else {
      Serial.println("At EAST limit -> Staying put at night");
      digitalWrite(RELAY_EAST, LOW);
      digitalWrite(RELAY_WEST, LOW);
    }
  }

  // === DAY MODE ===
  else {
    if (diff < -300 && limitWest == HIGH) {
      Serial.println("Sun is in the WEST -> Moving WEST");
      digitalWrite(RELAY_EAST, LOW);
      digitalWrite(RELAY_WEST, HIGH);
    } 
    else if (diff > 300 && limitEast == HIGH) {
      Serial.println("Sun is in the EAST -> Moving EAST");
      digitalWrite(RELAY_EAST, HIGH);
      digitalWrite(RELAY_WEST, LOW);
    } 
    else {
      Serial.println("No significant light difference or at limit -> STOP");
      digitalWrite(RELAY_EAST, LOW);
      digitalWrite(RELAY_WEST, LOW);
    }
  }

  Serial.println("===========================\n");
  delay(1000);
}
