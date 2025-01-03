// For Arduino: Using Serial1 for ESP32 cam communication
int motor1 = 7;
int motor_1 = 6;
int motor2 = 5;
int motor_2 = 4;
int enb = 3;
int ena = 9;

// Variable to store the last received command
String lastCommand = "";

void setup() {
  pinMode(motor1, OUTPUT);
  pinMode(motor_1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor_2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  Serial.begin(9600);   // For Serial Monitor
  Serial1.begin(9600);  // Initialize Serial1 for ESP32 communication
}

void loop() {
  // forward();
  // turn_right();
  if (Serial1.available() > 0) {
    char message[100];                                                        // Buffer to store incoming message
    int length = Serial1.readBytesUntil('\n', message, sizeof(message) - 1);  // Read until newline
    message[length] = '\0';                                                   // Null-terminate the string
    Serial.print("Received: ");
    Serial.println(message);  // Print the received message

    // Trim whitespace from the message
    String command = trimWhitespace(message);

    // Execute command
    if (command.equals("forward")) {
      lastCommand = "forward";
      Serial.println("Moving Forward");
      forward();
    } else if (command.equals("turn_right")) {
      lastCommand = "turn_right";
      Serial.println("Taking right path");
      turn_right();
    } else if (command.equals("left")) {
      lastCommand = "left";
      Serial.println("Turning Left");
      left();
    } else if (command.equals("right")) {
      lastCommand = "right";
      Serial.println("Turning Right");
      right();
    } else if (command.equals("backward")) {
      lastCommand = "backward";
      Serial.println("Moving Backward");
      backward();
    } else if (command.equals("stop")) {
      lastCommand = "stop";
      Serial.println("Stopping");
      STOP();
    }
  } else {
    // If no new message, continue the last valid command
    if (lastCommand == "forward") {
      forward();
    } else if (lastCommand == "turn_right") {
      lastCommand = "turn_right";
      Serial.println("Taking right path");
      turn_right();
    } else if (lastCommand == "left") {
      left();
    } else if (lastCommand == "right") {
      right();
    } else if (lastCommand == "backward") {
      backward();
    } else if (lastCommand == "stop") {
      STOP();
    } else {
      STOP();  // Stop if no valid command was received before
    }
  }
}

// Function to trim whitespace from the message
String trimWhitespace(const char* msg) {
  String str = String(msg);
  str.trim();  // Remove leading and trailing whitespace
  return str;
}

void backward() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor_1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor_2, LOW);
  analogWrite(ena, 100);
  analogWrite(enb, 100);
}
void turn_right() {
  forward();
  delay(1000);
  STOP();
  delay(1000);
  right();
  delay(375);
  STOP();
  delay(1000);
  forward();
  delay(1000);
  STOP();
}
void right() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor_1, HIGH);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor_2, LOW);
  analogWrite(ena, 60);
  analogWrite(enb, 60);
}

void left() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor_1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor_2, HIGH);
  analogWrite(ena, 60);
  analogWrite(enb, 60);
}

void forward() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor_1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor_2, HIGH);
  analogWrite(ena, 60);
  analogWrite(enb, 60);
}

void STOP() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor_1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor_2, LOW);
  lastCommand = "";  // Reset last command to stop
}
