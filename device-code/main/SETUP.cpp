#include "SETUP.h"

// Constructor
SETUP::SETUP(const char* ssid, const char* password, const char* username, const char* passwordInput) 
    : apSSID(ssid), apPassword(password), validUsername(username), validPassword(passwordInput), server(80), isLoggedIn(false) {

    // Dummy HTML pages
    loginPage = "<html><body><h2>Login Page</h2>"
                "<form action=\"/login\" method=\"POST\">"
                "<label for=\"username\">Username:</label><br>"
                "<input type=\"text\" id=\"username\" name=\"username\"><br><br>"
                "<label for=\"password\">Password:</label><br>"
                "<input type=\"password\" id=\"password\" name=\"password\"><br><br>"
                "<input type=\"submit\" value=\"Login\"></form></body></html>";

    homePage = "<html><body><h2>Home Page</h2>"
               "<p>Welcome to the home page!</p></body></html>";
}

// Method to start the access point and web server
void SETUP::begin() {
    // Set up the ESP32 as an access point
    WiFi.softAP(apSSID, apPassword);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("Access Point started. IP address: ");
    Serial.println(IP);

    // Setup server routes
    server.on("/", [this]() { handleLogin(); });
    server.on("/home", [this]() { handleHome(); });

    // Start the server
    server.begin();
    Serial.println("Server started");
}

// Method to process client requests
void SETUP::handleClient() {
    server.handleClient();
}

// Private method to serve login page
void SETUP::handleLogin() {
    server.send(200, "text/html", loginPage);
}

// Private method to serve home page
void SETUP::handleHome() {
    if (isLoggedIn) {
        server.send(200, "text/html", homePage);
    } else {
        server.send(401, "text/html", "<html><body><h2>Access Denied</h2><p>You must log in first!</p></body></html>");
    }
}

// Method to handle login and set isLoggedIn flag
void SETUP::checkLogin() {
    String username = server.arg("username");
    String password = server.arg("password");

    // Check if username and password are correct
    if (username == validUsername && password == validPassword) {
        isLoggedIn = true;

        // Print the IP of the user who logged in
        Serial.print("User logged in from IP: ");
        Serial.println(server.client().remoteIP());

        // Redirect to the home page
        server.sendHeader("Location", "/home", true);
        server.send(302, "text/html", "");
    } else {
        // If login fails
        server.send(401, "text/html", "<html><body><h2>Login Failed</h2><p>Invalid credentials</p></body></html>");
    }
}
