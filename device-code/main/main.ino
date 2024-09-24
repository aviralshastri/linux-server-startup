#include <WiFi.h>
#include <WebServer.h>

// Replace these with your hardcoded login credentials
const String correctUserId = "user123";
const String correctPassword = "pass123";

// Create an instance of the web server (port 80)
WebServer server(80);

// HTML content for the login page
const char* loginPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login - ESP32</title>
    <style>
        body {
            background: linear-gradient(135deg, #6a11cb 0%, #2575fc 100%);
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100vh;
            font-family: 'Arial', sans-serif;
            margin: 0;
        }
        .container {
            background: rgba(255, 255, 255, 0.15);
            border-radius: 20px;
            padding: 2.5rem;
            width: 100%;
            max-width: 400px;
            box-shadow: 0 8px 30px rgba(0, 0, 0, 0.5);
            backdrop-filter: blur(10px);
            transition: transform 0.3s;
            display: flex;
            flex-direction: column; /* Align children in a column */
            align-items: center; /* Center children horizontally */
        }
        .container:hover {
            transform: scale(1.02);
        }
        h1 {
            text-align: center;
            color: #ffffff;
            margin-bottom: 1.5rem;
            font-size: 2rem;
            letter-spacing: 1px;
        }
        .btn-primary {
            background-color: #ff4081; /* Soft pink */
            color: white;
            border: none;
            padding: 12px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 1rem;
            font-weight: bold;
            transition: background-color 0.3s, transform 0.3s;
            width: 100%; /* Button takes full width */
            margin-top: 1.5rem; /* Added margin for spacing */
            max-width: 200px; /* Maximum width for the button */
        }
        .btn-primary:hover {
            background-color: #e91e63; /* Darker pink on hover */
            transform: translateY(-2px);
        }
        label {
            margin-top: 1.5rem;
            display: block;
            color: #ffffff;
            font-weight: 500;
            text-align: left; /* Align label text to the left */
            width: 100%; /* Make labels take full width */
        }
        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 12px;
            margin-top: 0.5rem;
            border: 2px solid #ff4081; /* Match input border to button color */
            border-radius: 5px;
            background-color: #2c2c2c;
            color: white;
            font-size: 1rem;
            transition: border-color 0.3s, box-shadow 0.3s;
        }
        input[type="text"]:focus, input[type="password"]:focus {
            border-color: #ff80ab; /* Light pink focus border */
            box-shadow: 0 0 5px rgba(255, 80, 171, 0.6);
            outline: none;
        }
        @media (max-width: 480px) {
            .container {
                padding: 2rem; /* Adjusted padding for smaller screens */
            }
            h1 {
                font-size: 1.8rem; /* Slightly smaller font size on small screens */
            }
            .btn-primary {
                padding: 10px; /* Adjusted button padding */
            }
            input[type="text"], input[type="password"] {
                padding: 10px; /* Adjusted input padding */
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Login</h1>
        <form id="loginForm" action="/login" method="POST">
            <div>
                <label for="userId">User ID</label>
                <input id="userId" name="userId" type="text" placeholder="Enter your ID" autocomplete="off" required>
            </div>
            <div>
                <label for="password">Password</label>
                <input id="password" name="password" type="password" placeholder="••••••••" required>
            </div>
            <div>
                <button type="submit" class="btn-primary">Log In</button>
            </div>
        </form>
    </div>
</body>
</html>
)=====";

// HTML content for the main page
const char* mainPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Main Page - ESP32</title>
    <style>
        body {
            background-color: #1a1a1a;
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
            font-family: 'Arial', sans-serif;
            backdrop-filter: blur(10px);
        }
        .container {
            background: rgba(255, 255, 255, 0.15);
            border-radius: 20px;
            padding: 2rem;
            width: 90%; /* Responsive width */
            max-width: 600px; /* Max width */
            box-shadow: 0 8px 30px rgba(0, 0, 0, 0.5);
            color: white;
            display: flex;
            flex-direction: column;
            align-items: center; /* Center children horizontally */
        }
        h2 {
            text-align: center;
            color: #ffffff;
            margin-bottom: 1.5rem;
            font-size: 2rem;
            letter-spacing: 1px;
        }
        label {
            margin-top: 1rem;
            display: block;
            color: #ffffff;
            font-weight: 500;
            width: 100%;
            text-align: left; /* Align label text to the left */
        }
        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 12px;
            margin-top: 0.5rem;
            border: 2px solid #4b5563; /* Match input border to button color */
            border-radius: 5px;
            background-color: #2c2c2c;
            color: white;
            font-size: 1rem;
            transition: border-color 0.3s, box-shadow 0.3s;
        }
        input[type="text"]:focus, input[type="password"]:focus {
            border-color: #ff4081; /* Light pink focus border */
            box-shadow: 0 0 5px rgba(255, 80, 171, 0.6);
            outline: none;
        }
        .btn-primary {
            background-color: #ff4081; /* Soft pink */
            color: white;
            border: none;
            padding: 12px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 1rem;
            font-weight: bold;
            transition: background-color 0.3s, transform 0.3s;
            width: 100%; /* Button takes full width */
            margin-top: 1.5rem; /* Added margin for spacing */
        }
        .btn-primary:hover {
            background-color: #e91e63; /* Darker pink on hover */
            transform: translateY(-2px);
        }
        @media (max-width: 480px) {
            .container {
                padding: 1.5rem; /* Adjusted padding for smaller screens */
            }
            h2 {
                font-size: 1.8rem; /* Slightly smaller font size on small screens */
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>Configure Your Settings</h2>
        <form id="settingsForm">
            <div>
                <label for="ssid">WiFi SSID</label>
                <input id="ssid" name="ssid" type="text" placeholder="Enter WiFi SSID" required>
            </div>
            <div>
                <label for="wifiPassword">WiFi Password</label>
                <input id="wifiPassword" name="wifiPassword" type="password" placeholder="Enter WiFi Password" required>
            </div>
            <div>
                <label for="newUserId">New User ID</label>
                <input id="newUserId" name="newUserId" type="text" placeholder="Enter New User ID" required>
            </div>
            <div>
                <label for="newPassword">New Password</label>
                <input id="newPassword" name="newPassword" type="password" placeholder="Enter New Password" required>
            </div>
            <div>
                <button type="button" id="scanBtn" class="btn-primary">Scan Tag</button>
            </div>
        </form>
    </div>
</body>
</html>
)=====";

// Function to serve the login page
void handleLoginPage() {
    server.send(200, "text/html", loginPage);
}

// Function to serve the main page (after successful login)
void handleMainPage() {
    if (server.hasArg("userId") && server.hasArg("password")) {
        String userId = server.arg("userId");
        String password = server.arg("password");
        
        // Check if credentials are correct
        if (userId == correctUserId && password == correctPassword) {
            server.send(200, "text/html", mainPage);
        } else {
            // Redirect to login if credentials are incorrect
            server.sendHeader("Location", "/");
            server.send(302);
        }
    } else {
        server.sendHeader("Location", "/");
        server.send(302);
    }
}

// Setup function to configure the ESP32
void setup() {
    Serial.begin(115200);
    delay(3000);

    // Initialize WiFi in Access Point mode
    WiFi.softAP("esp32-server-controller", "1tounlimiteddata");
    Serial.println("Access Point started");
    Serial.println("IP Address: ");
    Serial.println(WiFi.softAPIP());

    // Route to serve the login page
    server.on("/", handleLoginPage);

    // Route to handle the login form and redirect to main page
    server.on("/login", HTTP_POST, handleMainPage);

    // Start the web server
    server.begin();
    Serial.println("Server started");
}

void loop() {
    // Handle incoming client requests
    server.handleClient();
}