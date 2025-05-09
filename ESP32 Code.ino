#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace with your network credentials
#define WIFI_SSID "Vansh"
#define WIFI_PASSWORD "qwertyuiop"

// Firebase project credentials
#define API_KEY "AIzaSyAFkFNEgeO1DIq407MyxV5KlXHKvZwQyxQ"
#define DATABASE_URL "https://iot-project-31020-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase objects
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Define relay and sensor pins
#define RELAY1 23
#define RELAY2 22
#define RELAY3 21
#define RELAY4 19

#define MOTOR1_RELAY 25
#define MOTOR2_RELAY 26
#define MOTOR3_RELAY 27

#define SENSOR1_PIN 34
#define SENSOR2_PIN 35
#define SENSOR3_PIN 32
#define SENSOR4_PIN 33

#define RAIN_SENSOR_PIN 4
#define DRY_THRESHOLD 2000

int currentZone = 1;
unsigned long zoneStartTime = 0;
const unsigned long maxZoneDuration = 60000;

bool motorsStarted = false;
bool motor3Started = false;
unsigned long motorStartTime = 0;
unsigned long motor3DelayStart = 0;

void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(RELAY1, OUTPUT); pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT); pinMode(RELAY4, OUTPUT);
  pinMode(MOTOR1_RELAY, OUTPUT); pinMode(MOTOR2_RELAY, OUTPUT); pinMode(MOTOR3_RELAY, OUTPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);

  // Turn off all relays initially
  digitalWrite(RELAY1, LOW); digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW); digitalWrite(RELAY4, LOW);
  digitalWrite(MOTOR1_RELAY, LOW); digitalWrite(MOTOR2_RELAY, LOW); digitalWrite(MOTOR3_RELAY, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWi-Fi connected: " + WiFi.localIP().toString());

  // Firebase Config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Anonymous sign-in
  auth.user.email = "vanshbadjate108@gmail.com";
  auth.user.password = "9403931288@";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase initialized");

  motorStartTime = millis();
}

void loop() {
  int moisture1 = analogRead(SENSOR1_PIN);
  int moisture2 = analogRead(SENSOR2_PIN);
  int moisture3 = analogRead(SENSOR3_PIN);
  int moisture4 = analogRead(SENSOR4_PIN);

  if (!motorsStarted) {
    digitalWrite(MOTOR1_RELAY, HIGH);
    digitalWrite(MOTOR2_RELAY, HIGH);
    if (millis() - motorStartTime >= 4000) {
      digitalWrite(MOTOR1_RELAY, LOW);
      digitalWrite(MOTOR2_RELAY, LOW);
      motorsStarted = true;
      motor3DelayStart = millis();
      Serial.println("Motors 1 & 2 stopped. Preparing Motor 3.");
    }
    return;
  }

  if (motorsStarted && !motor3Started) {
    if (millis() - motor3DelayStart >= 3000) {
      digitalWrite(MOTOR3_RELAY, HIGH);
      motor3Started = true;
      Serial.println("Motor 3 Started.");
    }
    return;
  }

  if (motor3Started && moisture4 > DRY_THRESHOLD) {
    digitalWrite(MOTOR3_RELAY, LOW);
    motor3Started = false;
    zoneStartTime = millis();
    Serial.println("Motor 3 OFF. Sensor 4 is wet. Starting irrigation.");
  }

  if (motor3Started) {
    Serial.print("Waiting for Sensor4 to be wet. Moisture: ");
    Serial.println(moisture4);
    delay(1000);
    return;
  }

  switch (currentZone) {
    case 1:
      digitalWrite(RELAY1, HIGH);
      updateFirebaseLineStatus(1);
      if (moisture1 < DRY_THRESHOLD || millis() - zoneStartTime > maxZoneDuration) {
        digitalWrite(RELAY1, LOW);
        currentZone = 2;
        zoneStartTime = millis();
      }
      break;
    case 2:
      digitalWrite(RELAY2, HIGH);
      updateFirebaseLineStatus(2);
      if (moisture2 < DRY_THRESHOLD || millis() - zoneStartTime > maxZoneDuration) {
        digitalWrite(RELAY2, LOW);
        currentZone = 3;
        zoneStartTime = millis();
      }
      break;
    case 3:
      digitalWrite(RELAY3, HIGH);
      updateFirebaseLineStatus(3);
      if (moisture3 < DRY_THRESHOLD || millis() - zoneStartTime > maxZoneDuration) {
        digitalWrite(RELAY3, LOW);
        currentZone = 4;
        zoneStartTime = millis();
      }
      break;
    case 4:
      digitalWrite(RELAY4, HIGH);
      updateFirebaseLineStatus(4);
      if (moisture4 < DRY_THRESHOLD || millis() - zoneStartTime > maxZoneDuration) {
        digitalWrite(RELAY4, LOW);
        currentZone = 1;
        zoneStartTime = millis();
      }
      break;
  }

  delay(1000);
}

void updateFirebaseLineStatus(int lineNumber) {
  if (Firebase.ready()) {
    String path = "/currentLine";
    if (Firebase.setInt(firebaseData, path.c_str(), lineNumber)) {
      Serial.print("Firebase updated: ");
      Serial.println(lineNumber);
    } else {
      Serial.print("Firebase update failed: ");
      Serial.println(firebaseData.errorReason());
    }
  } else {
    Serial.println("Firebase not ready.");
  }
}
