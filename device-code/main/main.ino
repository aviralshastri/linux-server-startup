#include <WiFi.h>
#include <WebServer.h>
#include <PAGES.h>
#include <GENERALS.h>
#include <RFID.h>
#include <PIN_MANAGER.h>

RFID rfid;
PIN_MANAGER pin_manager;
GENERALS generals;

String ssid = "ESP32-SERVER-CONTROLLER";
String password = generals.getUniqueId();
String loginId = "admin";
String loginPassword = "admin";

WebServer server(80);

bool isAuthenticated = false;

void handleRoot() {
  String html = PAGES::login;
  server.send(200, "text/html", html);
}

void handleLogin() {
  if (server.hasArg("userid") && server.hasArg("password")) {
    String userid = server.arg("userid");
    String userpassword = server.arg("password");
    if (userid == loginId && userpassword == loginPassword) {
      isAuthenticated = true;
      server.sendHeader("Location", "/configuration");
      server.send(302, "text/plain", "Redirecting to configuration...");
    } else {
      Serial.println(userid);
      Serial.println(userpassword);
      Serial.println(loginPassword);
      Serial.println(loginId);
      isAuthenticated = false;
      server.send(401, "text/plain", "Authentication Failed!");
      handleRoot();
    }
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void getAllTags() {
  if (!isAuthenticated) {
    server.send(401, "text/plain", "Unauthorized");
    return;
  }
  Serial.println("Fetching all tags...");
  String tags = generals.list_all_tags();
  String jsonResponse = "[";
  int startIndex = 0;
  int endIndex;

  while ((endIndex = tags.indexOf('\n', startIndex)) != -1) {
    String tagEntry = tags.substring(startIndex, endIndex);
    int colonIndex = tagEntry.indexOf(':');
    if (colonIndex != -1) {
      String id = tagEntry.substring(0, colonIndex);
      String tagInfo = tagEntry.substring(colonIndex + 2);

      jsonResponse += "{\"id\": \"" + id + "\", \"info\": " + tagInfo + "},";
    }

    startIndex = endIndex + 1;
  }

  if (jsonResponse.endsWith(",")) {
    jsonResponse = jsonResponse.substring(0, jsonResponse.length() - 1);
  }
  jsonResponse += "]";
  server.send(200, "application/json", jsonResponse);
  Serial.println(jsonResponse);
}

void handleConfiguration() {
  if (isAuthenticated) {
    String html = PAGES::main;
    server.send(200, "text/html", html);
  } else {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "Redirecting to login...");
  }
}

void handleAdd() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("Add called!");
  if (server.hasArg("id") && server.hasArg("name") && server.hasArg("role")) {
    String tag_id = server.arg("id");
    String name = server.arg("name");
    String role = server.arg("role");
    Serial.println(tag_id);
    Serial.println(name);
    Serial.println(role);
    if (generals.tag_exists(tag_id)) {
      Serial.println("exist");
      server.send(400, "text/plain", "Already exist!");
    } else {
      generals.add_tag(tag_id, name, role);
    }
    server.send(200, "text/plain", "Tag added successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleDelete() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("Delete called!");
  if (server.hasArg("id")) {
    String tag_id = server.arg("id");
    if (generals.tag_exists(tag_id)) {
      generals.remove_tag(tag_id);
      server.send(200, "text/plain", "Tag deleted successfully");
    } else {
      Serial.println("does not exist");
      server.send(400, "text/plain", "Tag does not exist!");
    }
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleEdit() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("Edit called!");
  if (server.hasArg("id") && (server.hasArg("name") || server.hasArg("role"))) {
    String tag_id = server.arg("id");
    Serial.print("Tag ID: ");
    Serial.println(tag_id);
    if (server.hasArg("name")) {
      String name = server.arg("name");
      Serial.print("New Name: ");
      Serial.println(name);
    } else {
      Serial.println("No Name provided");
    }
    if (server.hasArg("role")) {
      String role = server.arg("role");
      Serial.print("New Role: ");
      Serial.println(role);
    } else {
      Serial.println("No Role provided");
    }
    server.send(200, "text/plain", "Tag edited successfully");
  } else {
    Serial.println("Invalid Request: Missing required fields");
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleSaveUserConfig() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("User config called!");
  if (server.hasArg("userid") && server.hasArg("userpassword")) {
    String userid = server.arg("userid");
    String userpassword = server.arg("userpassword");
    Serial.println(userid);
    Serial.println(userpassword);
    server.send(200, "text/plain", "User config saved successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleSaveWifiConfig() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("Wifi config called!");
  if (server.hasArg("wifissid") && server.hasArg("wifipassword")) {
    String wifissid = server.arg("wifissid");
    String wifipassword = server.arg("wifipassword");
    Serial.println(wifissid);
    Serial.println(wifipassword);
    server.send(200, "text/plain", "WiFi config saved successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleSaveAPConfig() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("AP config called!");
  if (server.hasArg("apid") && server.hasArg("appassword")) {
    String apid = server.arg("apid");
    String appassword = server.arg("appassword");
    Serial.println(apid);
    Serial.println(appassword);
    server.send(200, "text/plain", "AP config saved successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleAddScanTag() {
  String id = rfid.scan_tag();
  while (id == "") {
    if (digitalRead(pin_manager.center_button) == LOW) {
      server.send(400, "text/plain", "Scan stopped in between.");
      break;
    }
    id = rfid.scan_tag();
  }
  Serial.println(id);
  Serial.println("Add Scan Tag Called!");
  server.send(200, "text/plain", id);
}

void handleLoginScanTag() {
  Serial.println("Login Scan Tag Called!");
  String id = rfid.scan_tag();
  while (id == "") {
    if (digitalRead(pin_manager.center_button) == LOW) {
      server.send(400, "text/plain", "Scan stopped in between.");
      break;
    }
    id = rfid.scan_tag();
  }
  String correct = "230B4EFB";
  if (id == correct) {
    isAuthenticated = true;
    server.sendHeader("Location", "/configuration", true);
    server.send(302, "text/plain", "Redirecting to /configuration");
  } else {
    server.send(401, "text/plain", "Invalid Card!");
  }
  Serial.println("Scan Stopped");
}

void handleLogout() {
  isAuthenticated = false;
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting to login...");
}

void setup() {

  delay(4000);
  Serial.begin(115200);

  rfid.begin();
  pin_manager.initPins();

  Serial.println(ssid.c_str());
  Serial.println(password.c_str());
  Serial.println(loginId);
  Serial.println(loginPassword);

  WiFi.softAP(ssid.c_str(), password.c_str());
  Serial.println("Access Point Created");
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  server.on("/", handleRoot);
  server.on("/configuration", handleConfiguration);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", handleLogout);
  server.on("/saveUserConfig", HTTP_POST, handleSaveUserConfig);
  server.on("/saveAPConfig", HTTP_POST, handleSaveAPConfig);
  server.on("/saveWifiConfig", HTTP_POST, handleSaveWifiConfig);
  server.on("/getAllTags", HTTP_GET, getAllTags);
  server.on("/add", HTTP_POST, handleAdd);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/edit", HTTP_POST, handleEdit);
  server.on("/addScanTag", HTTP_GET, handleAddScanTag);
  server.on("/loginScanTag", handleLoginScanTag);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}