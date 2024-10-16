#include <WiFi.h>
#include <WebServer.h>
#include <PAGES.h>
#include <GENERALS.h>
#include <RFID.h>
#include <PIN_MANAGER.h>
#include <I2C_LCD.h>

RFID rfid;
PIN_MANAGER pin_manager;
GENERALS generals;

String ssid = "ESP32-SERVER-CONTROLLER";
String password = generals.getUniqueId();
String loginId = "admin";
String loginPassword = "admin";

WebServer server(80);

bool isAuthenticated = false;
IPAddress connectedClientIP;
unsigned long lastActivityTime = 0;
const unsigned long TIMEOUT_DURATION = 60000;

void checkAndHandleTimeout()
{
  if (isAuthenticated && (millis() - lastActivityTime > TIMEOUT_DURATION))
  {
    Serial.println("Session timed out. Logging out.");
    handleLogout();
  }
}

void updateLastActivityTime()
{
  lastActivityTime = millis();
}

bool isClientAllowed()
{
  if (!isAuthenticated)
  {
    return true;
  }

  if (server.client().remoteIP() == connectedClientIP)
  {
    updateLastActivityTime();
    return true;
  }

  return false;
}

void handleRoot()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Another device is already connected. Please try again later.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  String html = PAGES::login;
  digitalWrite(pin_manager.process_led_green, HIGH);
  server.send(200, "text/html", html);
  delay(1000);
  digitalWrite(pin_manager.process_led_green, LOW);
}

void handleLogin()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Another device is already connected. Please try again later.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  if (server.hasArg("userid") && server.hasArg("password"))
  {
    String userid = server.arg("userid");
    String userpassword = server.arg("password");
    if (userid == loginId && userpassword == loginPassword)
    {
      isAuthenticated = true;
      connectedClientIP = server.client().remoteIP();
      updateLastActivityTime();
      server.sendHeader("Location", "/configuration");
      server.send(302, "text/plain", "Redirecting to configuration...");
    }
    else
    {
      isAuthenticated = false;
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(401, "text/plain", "Authentication Failed!");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
      handleRoot();
    }
  }
  else
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleConfiguration()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (isAuthenticated)
  {
    String html = PAGES::main;

    digitalWrite(pin_manager.process_led_green, HIGH);
    server.send(200, "text/html", html);
    delay(1000);
    digitalWrite(pin_manager.process_led_green, LOW);
  }
  else
  {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "Redirecting to login...");
  }
}

void getAllTags()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("Fetching all tags...");
  String tags = generals.list_all_tags();
  String jsonResponse = "[";
  int startIndex = 0;
  int endIndex;

  while ((endIndex = tags.indexOf('\n', startIndex)) != -1)
  {
    String tagEntry = tags.substring(startIndex, endIndex);
    int colonIndex = tagEntry.indexOf(':');
    if (colonIndex != -1)
    {
      String id = tagEntry.substring(0, colonIndex);
      String tagInfo = tagEntry.substring(colonIndex + 2);
      jsonResponse += "{\"id\": \"" + id + "\", \"info\": " + tagInfo + "},";
    }
    startIndex = endIndex + 1;
  }

  if (jsonResponse.endsWith(","))
  {
    jsonResponse = jsonResponse.substring(0, jsonResponse.length() - 1);
  }
  jsonResponse += "]";
  digitalWrite(pin_manager.process_led_green, HIGH);
  server.send(200, "application/json", jsonResponse);
  delay(1000);
  digitalWrite(pin_manager.process_led_green, LOW);
  Serial.println(jsonResponse);
}

void handleAdd()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("Add called!");
  if (server.hasArg("id") && server.hasArg("name") && server.hasArg("role"))
  {
    String tag_id = server.arg("id");
    String name = server.arg("name");
    String role = server.arg("role");
    Serial.println(tag_id);
    Serial.println(name);
    Serial.println(role);
    if (generals.tag_exists(tag_id))
    {
      Serial.println("exist");
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(400, "text/plain", "Already exist!");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
    }
    else
    {
      generals.add_tag(tag_id, name, role);

      digitalWrite(pin_manager.process_led_green, HIGH);
      server.send(200, "text/plain", "Tag added successfully");
      delay(1000);
      digitalWrite(pin_manager.process_led_green, LOW);
    }
  }
  else
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleDelete()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("Delete called!");
  if (server.hasArg("id"))
  {
    String tag_id = server.arg("id");
    if (generals.tag_exists(tag_id))
    {
      generals.remove_tag(tag_id);
      digitalWrite(pin_manager.process_led_green, HIGH);
      server.send(200, "text/plain", "Tag deleted successfully");
      delay(1000);
      digitalWrite(pin_manager.process_led_green, LOW);
    }
    else
    {
      Serial.println("does not exist");
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(400, "text/plain", "Tag does not exist!");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
    }
  }
  else
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleEdit()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("Edit called!");
  if (server.hasArg("id") && (server.hasArg("name") || server.hasArg("role")))
  {
    String tag_id = server.arg("id");
    Serial.print("Tag ID: ");
    Serial.println(tag_id);

    if (!generals.tag_exists(tag_id))
    {
      Serial.println("Tag does not exist");
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(404, "text/plain", "Tag not found!");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
      return;
    }
    if (server.hasArg("name"))
    {
      String name = server.arg("name");
      Serial.print("New Name: ");
      Serial.println(name);
      generals.update_tag_name(tag_id, name);
    }
    if (server.hasArg("role"))
    {
      String role = server.arg("role");
      Serial.print("New Role: ");
      Serial.println(role);
      generals.update_tag_role(tag_id, role);
    }

    digitalWrite(pin_manager.process_led_green, HIGH);
    server.send(200, "text/plain", "Tag edited successfully");
    delay(1000);
    digitalWrite(pin_manager.process_led_green, LOW);
  }
  else
  {
    Serial.println("Invalid Request: Missing required fields");
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleSaveUserConfig()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("User config called!");
  if (server.hasArg("userid") && server.hasArg("userpassword"))
  {
    String userid = server.arg("userid");
    String userpassword = server.arg("userpassword");

    Serial.println("Saving user ID: " + userid);
    Serial.println("Saving user password: " + userpassword);
    bool saveUserIdSuccess = generals.writeSettingsKey("UI", userid);
    bool saveUserPasswordSuccess = generals.writeSettingsKey("UP", userpassword);

    if (saveUserIdSuccess && saveUserPasswordSuccess)
    {
      digitalWrite(pin_manager.process_led_green, HIGH);
      server.send(200, "text/plain", "User config saved successfully");
      Serial.println("User config saved successfully");
    }
    else
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(500, "text/plain", "Failed to save user config");
      Serial.println("Failed to save user config");
    }

    delay(1000);
    digitalWrite(pin_manager.process_led_green, LOW);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
  else
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleSaveWifiConfig()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("Wifi config called!");
  if (server.hasArg("wifissid") && server.hasArg("wifipassword"))
  {
    String wifissid = server.arg("wifissid");
    String wifipassword = server.arg("wifipassword");

    Serial.println("Saving WiFi SSID: " + wifissid);
    Serial.println("Saving WiFi Password: " + wifipassword);

    bool saveWifiSSIDSuccess = generals.writeSettingsKey("WI", wifissid);
    bool saveWifiPasswordSuccess = generals.writeSettingsKey("WP", wifipassword);

    if (saveWifiSSIDSuccess && saveWifiPasswordSuccess)
    {
      digitalWrite(pin_manager.process_led_green, HIGH);
      server.send(200, "text/plain", "WiFi config saved successfully");
      Serial.println("WiFi config saved successfully");
    }
    else
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(500, "text/plain", "Failed to save WiFi config");
      Serial.println("Failed to save WiFi config");
    }

    delay(1000);
    digitalWrite(pin_manager.process_led_green, LOW);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
  else
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleSaveAPConfig()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("AP config called!");
  if (server.hasArg("apid") && server.hasArg("appassword"))
  {
    String apid = server.arg("apid");
    String appassword = server.arg("appassword");

    Serial.println("Saving AP ID: " + apid);
    Serial.println("Saving AP Password: " + appassword);
    bool saveAPIDSuccess = generals.writeSettingsKey("API", apid);
    bool saveAPPasswordSuccess = generals.writeSettingsKey("APP", appassword);

    if (saveAPIDSuccess && saveAPPasswordSuccess)
    {
      digitalWrite(pin_manager.process_led_green, HIGH);
      server.send(200, "text/plain", "AP config saved successfully");
      Serial.println("AP config saved successfully");
    }
    else
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(500, "text/plain", "Failed to save AP config");
      Serial.println("Failed to save AP config");
    }

    delay(1000);
    digitalWrite(pin_manager.process_led_green, LOW);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
  else
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(400, "text/plain", "Invalid Request");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
}

void handleAddScanTag()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Unauthorized or another device is connected.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  checkAndHandleTimeout();

  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  unsigned long startTime = millis();
  String id = rfid.scan_tag();
  digitalWrite(pin_manager.process_led_blue, HIGH);
  while (id == "")
  {
    if (digitalRead(pin_manager.center_button) == LOW)
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(400, "text/plain", "Scan stopped in between.");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
      break;
    }
    if (millis() - startTime >= 20000)
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(400, "text/plain", "Timeout error: No scan within 20 seconds.");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
      break;
    }
    id = rfid.scan_tag();
  }
  digitalWrite(pin_manager.process_led_blue, LOW);
  if (id != "")
  {
    Serial.println(id);
    Serial.println("Add Scan Tag Called!");
    digitalWrite(pin_manager.process_led_green, HIGH);
    server.send(200, "text/plain", id);
    delay(1000);
    digitalWrite(pin_manager.process_led_green, LOW);
  }
  Serial.println("Scan Stopped");
}

void handleLoginScanTag()
{
  if (!isClientAllowed())
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(403, "text/plain", "Another device is already connected. Please try again later.");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  Serial.println("Login Scan Tag Called!");
  unsigned long startTime = millis();
  String id = rfid.scan_tag();
  digitalWrite(pin_manager.process_led_blue, HIGH);
  while (id == "")
  {
    if (digitalRead(pin_manager.center_button) == LOW)
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(400, "text/plain", "Scan stopped in between.");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
      break;
    }
    if (millis() - startTime >= 20000)
    {
      digitalWrite(pin_manager.process_led_red, HIGH);
      server.send(400, "text/plain", "Timeout error: No scan within 20 seconds.");
      delay(1000);
      digitalWrite(pin_manager.process_led_red, LOW);
      break;
    }
    id = rfid.scan_tag();
  }
  digitalWrite(pin_manager.process_led_blue, LOW);
  if (id == "230B4EFB")
  {
    isAuthenticated = true;
    connectedClientIP = server.client().remoteIP();
    updateLastActivityTime();
    server.sendHeader("Location", "/configuration", true);
    server.send(302, "text/plain", "Redirecting to /configuration");
  }
  else if (id != "")
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Invalid Card!");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
  }
  Serial.println("Scan Stopped");
}

void getConfig()
{
  if (!isAuthenticated)
  {
    digitalWrite(pin_manager.process_led_red, HIGH);
    server.send(401, "text/plain", "Unauthorized");
    delay(1000);
    digitalWrite(pin_manager.process_led_red, LOW);
    return;
  }

  String WP = generals.readSettingsKey("WP");
  String WI = generals.readSettingsKey("WI");
  String UI = generals.readSettingsKey("UI");
  String UP = generals.readSettingsKey("UP");
  String APP = generals.readSettingsKey("APP");
  String API = generals.readSettingsKey("API");

  String jsonResponse = String("{") + "\"WI\": \"" + WI + "\"," + "\"WP\": \"" + WP + "\"," + "\"UI\": \"" + UI + "\"," + "\"UP\": \"" + UP + "\"," + "\"APP\": \"" + APP + "\"," + "\"API\": \"" + API + "\"" + "}";
  digitalWrite(pin_manager.process_led_green, HIGH);
  server.send(200, "application/json", jsonResponse);
  delay(1000);
  digitalWrite(pin_manager.process_led_green, LOW);
}

void handleLogout()
{
  isAuthenticated = false;
  connectedClientIP = IPAddress(0, 0, 0, 0);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting to login...");
}

void setup()
{
  delay(4000);
  Serial.begin(115200);

  rfid.begin();
  pin_manager.initPins();
  digitalWrite(pin_manager.startup_led_red, HIGH);

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
  server.on("/getAllTags", HTTP_GET, getAllTags);
  server.on("/getConfig", HTTP_GET, getConfig);
  server.on("/saveUserConfig", HTTP_POST, handleSaveUserConfig);
  server.on("/saveAPConfig", HTTP_POST, handleSaveAPConfig);
  server.on("/saveWifiConfig", HTTP_POST, handleSaveWifiConfig);
  server.on("/add", HTTP_POST, handleAdd);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/edit", HTTP_POST, handleEdit);
  server.on("/addScanTag", HTTP_GET, handleAddScanTag);
  server.on("/loginScanTag", handleLoginScanTag);

  server.begin();
  Serial.println("Server started");
  delay(1000);
  digitalWrite(pin_manager.startup_led_red, LOW);
  delay(500);
  digitalWrite(pin_manager.startup_led_blue, HIGH);
}

void loop()
{
  server.handleClient();
  checkAndHandleTimeout();
}