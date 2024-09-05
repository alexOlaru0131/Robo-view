#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

const char* ssid = "Alexandru's Galaxy S22 Ultra";
const char* password = "vguc2110";

WebServer server(80);

const char* camServerIP = "192.168.196.252";

const int in1 = 14;
const int in2 = 12;
const int in3 = 27;
const int in4 = 26;

const int servoPin = 13;

Servo cameraServo;

bool faceDetectionEnabled = false;

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setupMotorControl() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  stopMotors();
}

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void setFaceDetection(bool enable) {
  HTTPClient http;
  String url = "http://" + String(camServerIP) + "/control?var=face_detect&val=" + String(enable ? "1" : "0");
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void setServoPosition(int position) {
  Serial.print("Setting servo position to: ");
  Serial.println(position);
  cameraServo.write(position);
}

void setupWebServer() {
  server.on("/", []() {
    String html = "<!DOCTYPE html>"
                  "<html><head><title>Control Motoare</title>"
                  "<style>"
                  "body { font-family: Arial; text-align: center; margin-top: 50px;}"
                  "button { font-size: 20px; padding: 10px 20px; margin: 10px; width: 150px;}"
                  "input[type='checkbox'] { transform: scale(2); margin: 20px; }"
                  "input[type='range'] { width: 300px; margin: 20px; }"
                  "</style></head>"
                  "<body>"
                  "<h1>Control Motoare</h1>"
                  "<button onmousedown=\"sendCommand('forward')\" onmouseup=\"sendCommand('stop')\">Inainte</button>"
                  "<button onmousedown=\"sendCommand('backward')\" onmouseup=\"sendCommand('stop')\">Inapoi</button>"
                  "<button onmousedown=\"sendCommand('left')\" onmouseup=\"sendCommand('stop')\">Stanga</button>"
                  "<button onmousedown=\"sendCommand('right')\" onmouseup=\"sendCommand('stop')\">Dreapta</button>"
                  "<button onclick=\"sendCommand('stop')\">Stop</button>"
                  "<br><br>"
                  "<label for='faceDetectionSwitch'>Detectie Fete:</label>"
                  "<input type='checkbox' id='faceDetectionSwitch' onclick='toggleFaceDetection()'>"
                  "<br><br>"
                  "<label for='servoSlider'>Pozitie Servo:</label>"
                  "<input type='range' id='servoSlider' min='0' max='180' oninput='adjustServoPosition()'>"
                  "<br><br>"
                  "<img id=\"cameraFeed\" src=\"http://" + String(camServerIP) + ":81/stream\" width=\"640\" height=\"480\">"
                  "<script>"
                  "function sendCommand(command) {"
                  "  var xhttp = new XMLHttpRequest();"
                  "  xhttp.open('GET', '/' + command, true);"
                  "  xhttp.send();"
                  "}"
                  "function toggleFaceDetection() {"
                  "  var checkBox = document.getElementById('faceDetectionSwitch');"
                  "  var xhttp = new XMLHttpRequest();"
                  "  xhttp.open('GET', '/toggleFaceDetection?enable=' + (checkBox.checked ? '1' : '0'), true);"
                  "  xhttp.send();"
                  "}"
                  "function adjustServoPosition() {"
                  "  var slider = document.getElementById('servoSlider');"
                  "  var xhttp = new XMLHttpRequest();"
                  "  xhttp.open('GET', '/setServoPosition?position=' + slider.value, true);"
                  "  xhttp.send();"
                  "}"
                  "</script>"
                  "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/forward", []() { moveForward(); server.send(200, "text/plain", ""); });
  server.on("/backward", []() { moveBackward(); server.send(200, "text/plain", ""); });
  server.on("/left", []() { turnLeft(); server.send(200, "text/plain", ""); });
  server.on("/right", []() { turnRight(); server.send(200, "text/plain", ""); });
  server.on("/stop", []() { stopMotors(); server.send(200, "text/plain", ""); });

  server.on("/toggleFaceDetection", []() {
    if (server.hasArg("enable")) {
      faceDetectionEnabled = server.arg("enable") == "1";
      setFaceDetection(faceDetectionEnabled);
      server.send(200, "text/plain", faceDetectionEnabled ? "Face detection enabled" : "Face detection disabled");
    } else {
      server.send(400, "text/plain", "Invalid arguments");
    }
  });

  server.on("/setServoPosition", []() {
    if (server.hasArg("position")) {
      int position = server.arg("position").toInt();
      Serial.print("Received servo position: ");
      Serial.println(position);
      setServoPosition(position);
      server.send(200, "text/plain", "Servo position set to " + String(position));
    } else {
      server.send(400, "text/plain", "Invalid arguments");
    }
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println("IP address: " + WiFi.localIP().toString());
  setupMotorControl();
  cameraServo.attach(servoPin);
  setupWebServer();
}

void loop() {
  server.handleClient();
}
