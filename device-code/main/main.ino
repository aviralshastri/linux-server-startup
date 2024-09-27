#include <WiFi.h>
#include <WebServer.h>
#include <SETUP.h>

const char* ssid = "ESP32_Access_Point";
const char* password = "12345678";

WebServer server(80);

bool isAuthenticated = false;

const char* loginPage = SETUP::login;

const char* homePage = SETUP::home;

void setup() {
  Serial.begin(115200);
  
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/home", handleHome);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  Serial.println("Handling root request");
  if (isAuthenticated) {
    Serial.println("User is authenticated, redirecting to home");
    server.sendHeader("Location", "/home");
    server.send(303);
  } else {
    Serial.println("User is not authenticated, sending login page");
    server.send(200, "text/html", loginPage);
  }
}

void handleLogin() {
  Serial.println("Handling login request");
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == "admin" && server.arg("password") == "admin") {
      isAuthenticated = true;
      Serial.println("Login successful, redirecting to home");
      server.sendHeader("Location", "/home");
      server.send(303);
    } else {
      Serial.println("Invalid credentials");
      server.send(401, "text/plain", "Invalid credentials");
    }
  } else {
    Serial.println("Bad login request");
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleHome() {
  Serial.println("Handling home request");
  if (!isAuthenticated) {
    Serial.println("User is not authenticated, redirecting to root");
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    Serial.println("User is authenticated, sending home page");
    server.send(200, "text/html", homePage);
  }
}

void handleNotFound() {
  Serial.println("Handling 404 request");
  server.send(404, "text/plain", "404: Not found");
}