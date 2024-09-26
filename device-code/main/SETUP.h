#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <WebServer.h>

class SETUP {
private:
    WebServer server;
    
    const char* apSSID;
    const char* apPassword;
    const char* validUsername;
    const char* validPassword;

    bool isLoggedIn;  // To track if user is logged in

    // HTML pages
    const char* loginPage;
    const char* homePage;

    // Private methods to serve pages
    void handleLogin();
    void handleHome();

    // Private method to check login status
    void checkLogin();

public:
    // Constructor
    SETUP(const char* ssid, const char* password, const char* username, const char* passwordInput);

    // Public method to start the access point and web server
    void begin();

    // Public method to process server requests
    void handleClient();
};

#endif
