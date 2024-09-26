#include "SETUP.h"

// Constructor
SETUP::SETUP(const char* ssid, const char* password, const char* username, const char* passwordInput)
  : apSSID(ssid), apPassword(password), validUsername(username), validPassword(passwordInput), server(80), isLoggedIn(false) {

  loginPage = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Login</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <script>
      tailwind.config = {
        darkMode: "class",
        theme: {
          extend: {
            colors: {
              border: "hsl(var(--border))",
              input: "hsl(var(--input))",
              ring: "hsl(var(--ring))",
              background: "hsl(var(--background))",
              foreground: "hsl(var(--foreground))",
              primary: {
                DEFAULT: "hsl(var(--primary))",
                foreground: "hsl(var(--primary-foreground))",
              },
              secondary: {
                DEFAULT: "hsl(var(--secondary))",
                foreground: "hsl(var(--secondary-foreground))",
              },
              destructive: {
                DEFAULT: "hsl(var(--destructive))",
                foreground: "hsl(var(--destructive-foreground))",
              },
              muted: {
                DEFAULT: "hsl(var(--muted))",
                foreground: "hsl(var(--muted-foreground))",
              },
              accent: {
                DEFAULT: "hsl(var(--accent))",
                foreground: "hsl(var(--accent-foreground))",
              },
              card: {
                DEFAULT: "hsl(var(--card))",
                foreground: "hsl(var(--card-foreground))",
              },
            },
            borderRadius: {
              lg: "var(--radius)",
              md: "calc(var(--radius) - 2px)",
              sm: "calc(var(--radius) - 4px)",
            },
          },
        },
      };
    </script>
    <style type="text/css">
      @import url("https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap");

      :root {
        --background: 240 10% 3.9%;
        --foreground: 0 0% 98%;
        --card: 240 10% 3.9%;
        --card-foreground: 0 0% 98%;
        --popover: 240 10% 3.9%;
        --popover-foreground: 0 0% 98%;
        --primary: 0 0% 98%;
        --primary-foreground: 240 5.9% 10%;
        --secondary: 240 3.7% 15.9%;
        --secondary-foreground: 0 0% 98%;
        --muted: 240 3.7% 15.9%;
        --muted-foreground: 240 5% 64.9%;
        --accent: 240 3.7% 15.9%;
        --accent-foreground: 0 0% 98%;
        --destructive: 0 62.8% 30.6%;
        --destructive-foreground: 0 0% 98%;
        --border: 240 3.7% 15.9%;
        --input: 240 3.7% 15.9%;
        --ring: 240 4.9% 83.9%;
        --radius: 0.5rem;
      }

      body {
        font-family: "Inter", sans-serif;
      }
      .modal-title {
        display: flex;
        align-items: center;
      }

      .logo-container {
        display: inline-block;
        margin-left: 10px;
      }

      .logo {
        width: 24px;
        height: 24px;
        fill: white;
      }
    </style>
  </head>
  <body class="bg-background text-foreground">
    <div class="min-h-screen flex items-center justify-center">
      <div class="w-full max-w-sm mx-auto">
        <form
          action="/login"
          method="POST"
          class="bg-card border border-border rounded-lg shadow-lg p-6 space-y-6"
        >
          <div class="space-y-2 text-center">
            <h1 class="text-3xl font-bold">Login</h1>
            <p class="text-muted-foreground">
              Enter your credentials to access device.<br />(default password is
              written on the device display.)
            </p>
          </div>
          <div class="space-y-4">
            <div class="space-y-2">
              <label
                for="username"
                class="text-sm font-medium leading-none peer-disabled:cursor-not-allowed peer-disabled:opacity-70"
                >User ID</label
              >
              <input
                type="text"
                id="username"
                placeholder="Enter your user ID"
                class="flex h-10 w-full rounded-md border border-input bg-background px-3 py-2 text-sm ring-offset-background file:border-0 file:bg-transparent file:text-sm file:font-medium placeholder:text-muted-foreground focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:cursor-not-allowed disabled:opacity-50"
              />
            </div>
            <div class="space-y-2">
              <label
                for="password"
                class="text-sm font-medium leading-none peer-disabled:cursor-not-allowed peer-disabled:opacity-70"
                >Password</label
              >
              <input
                type="password"
                id="password"
                placeholder="Enter your password"
                class="flex h-10 w-full rounded-md border border-input bg-background px-3 py-2 text-sm ring-offset-background file:border-0 file:bg-transparent file:text-sm file:font-medium placeholder:text-muted-foreground focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:cursor-not-allowed disabled:opacity-50"
              />
            </div>
            <div class="flex flex-col space-y-4">
              <button
                type="submit"
                value="login"
                class="inline-flex items-center justify-center rounded-md text-lg font-bold ring-offset-background transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:pointer-events-none disabled:opacity-50 bg-primary text-primary-foreground hover:bg-primary/90 h-10 px-4 py-2 w-full"
              >
                Login
              </button>
            </div>
          </div>
        </form>
        <div class="flex items-center justify-center w-full mt-2">
          <div class="flex items-center w-full max-w-[300px]">
            <div
              class="w-full h-px bg-gradient-to-r from-gray-200 via-gray-400 to-transparent"
            ></div>
            <span class="px-2 text-gray-500">or</span>
            <div
              class="w-full h-px bg-gradient-to-l from-gray-200 via-gray-400 to-transparent"
            ></div>
          </div>
        </div>
        <div
          class="bg-card border border-border rounded-lg shadow-lg p-6 space-y-6 mt-2"
        >
          <button
            class="inline-flex items-center justify-center rounded-md text-sm font-medium ring-offset-background transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:pointer-events-none disabled:opacity-50 bg-secondary text-secondary-foreground hover:bg-secondary/80 h-10 px-4 py-2 w-full"
            onclick="tagModal.classList.remove('hidden')"
          >
            <span class="logo-container font-bold mr-2 size-6">
              <svg
                id="Layer_1"
                data-name="Layer 1"
                xmlns="http://www.w3.org/2000/svg"
                viewBox="0 0 122.88 122.02"
                class="logo"
              >
                <title>rfid</title>
                <path
                  d="M62.19,48.52a9.83,9.83,0,0,0-.25-19.61h-1a9.83,9.83,0,0,0,0,19.63h1.25ZM24.53,91q6,0,9.29,2.7a8.86,8.86,0,0,1,3.24,7.16q0,5.11-3.17,7.7c-2.11,1.72-5.19,2.58-9.22,2.58l-1.08.56H18.42V122H7.72V91Zm-1.88,13.52a4.41,4.41,0,0,0,2.6-.63,3.27,3.27,0,0,0,0-4.44,4.26,4.26,0,0,0-2.6-.66H18.42v5.73Zm6.47,2.3L38.79,122H26.92l-7.23-13.56,9.43-1.6Zm37.37-7.79H51.75v4.32H63.67v8H51.75V122H40.81V91H66.49v8Zm14.08-8v31H69.63V91Zm19.9,0q7.27,0,11,4t3.71,11.45q0,7.36-3.71,11.43t-11,4.06H85.26V91Zm-1.55,23.19c2,0,3.38-.62,4.06-1.86a12.78,12.78,0,0,0,1-5.84,12.78,12.78,0,0,0-1-5.84c-.68-1.24-2-1.86-4.06-1.86H96.19v15.4ZM41.57,21.77a4.5,4.5,0,0,0-.51-6.2l-.29-.23a4.4,4.4,0,0,0-3.14-.86,4.45,4.45,0,0,0-2.9,1.5,1.79,1.79,0,0,0-.22.25,41.63,41.63,0,0,0-6.45,10.92A31.56,31.56,0,0,0,28,50.43a40.44,40.44,0,0,0,6.49,10.93,4.49,4.49,0,0,0,6.31.65h0a4.5,4.5,0,0,0,.65-6.32h0a31.57,31.57,0,0,1-5-8.36,22.93,22.93,0,0,1-1.65-8.51,23.32,23.32,0,0,1,1.71-8.56,32.39,32.39,0,0,1,5.08-8.49ZM22.4,7.54a1.37,1.37,0,0,0,.19-.23,4.45,4.45,0,0,0-.43-6.1,1.37,1.37,0,0,0-.22-.2,4.48,4.48,0,0,0-6.18.5A67.93,67.93,0,0,0,3.84,19.91,46.8,46.8,0,0,0,0,39,46.8,46.8,0,0,0,4.47,58,68.05,68.05,0,0,0,17,76.05l.11.11a4.48,4.48,0,0,0,6.23.08l.11-.1a4.56,4.56,0,0,0,1.31-3,4.48,4.48,0,0,0-1.22-3.21A59.49,59.49,0,0,1,12.72,54.48,38.08,38.08,0,0,1,9,38.89a38.45,38.45,0,0,1,3.23-15.72A59,59,0,0,1,22.4,7.54ZM81.31,21.77a4.5,4.5,0,0,1,.51-6.2l.28-.23a4.49,4.49,0,0,1,6.05.64,1.27,1.27,0,0,1,.21.25,41.4,41.4,0,0,1,6.46,10.92,31.56,31.56,0,0,1,.07,23.28A40.44,40.44,0,0,1,88.4,61.36a4.49,4.49,0,0,1-6.31.65h0a4.48,4.48,0,0,1-1.63-3,4.55,4.55,0,0,1,1-3.3h0a31.57,31.57,0,0,0,5-8.36,22.93,22.93,0,0,0,1.65-8.51,23.32,23.32,0,0,0-1.71-8.56,32.39,32.39,0,0,0-5.08-8.49ZM100.48,7.54a1.37,1.37,0,0,1-.19-.23,4.45,4.45,0,0,1,.43-6.1,1.37,1.37,0,0,1,.22-.2,4.48,4.48,0,0,1,6.18.5A67.93,67.93,0,0,1,119,19.91,46.8,46.8,0,0,1,122.87,39a46.8,46.8,0,0,1-4.46,19,68.05,68.05,0,0,1-12.52,18l-.11.11a4.48,4.48,0,0,1-6.23.08l-.11-.1a4.56,4.56,0,0,1-1.31-3,4.48,4.48,0,0,1,1.22-3.21,59.49,59.49,0,0,0,10.81-15.42,38.08,38.08,0,0,0,3.76-15.59,38.45,38.45,0,0,0-3.23-15.72A59,59,0,0,0,100.48,7.54Z"
                />
              </svg>
            </span>
            <h1 class="text-md">Login</h1>
          </button>
        </div>
      </div>
    </div>
    <div
      id="tagModal"
      class="fixed inset-0 hidden bg-black bg-opacity-50 flex items-center justify-center z-50"
    >
      <div
        class="bg-card border border-border rounded-lg shadow-lg p-6 space-y-4 w-full max-w-sm"
      >
        <div class="modal-header">
          <h5 class="modal-title">
            <span class="logo-container text-xl font-bold">
              <svg
                id="Layer_1"
                data-name="Layer 1"
                xmlns="http://www.w3.org/2000/svg"
                viewBox="0 0 122.88 122.02"
                class="logo"
              >
                <title>rfid</title>
                <path
                  d="M62.19,48.52a9.83,9.83,0,0,0-.25-19.61h-1a9.83,9.83,0,0,0,0,19.63h1.25ZM24.53,91q6,0,9.29,2.7a8.86,8.86,0,0,1,3.24,7.16q0,5.11-3.17,7.7c-2.11,1.72-5.19,2.58-9.22,2.58l-1.08.56H18.42V122H7.72V91Zm-1.88,13.52a4.41,4.41,0,0,0,2.6-.63,3.27,3.27,0,0,0,0-4.44,4.26,4.26,0,0,0-2.6-.66H18.42v5.73Zm6.47,2.3L38.79,122H26.92l-7.23-13.56,9.43-1.6Zm37.37-7.79H51.75v4.32H63.67v8H51.75V122H40.81V91H66.49v8Zm14.08-8v31H69.63V91Zm19.9,0q7.27,0,11,4t3.71,11.45q0,7.36-3.71,11.43t-11,4.06H85.26V91Zm-1.55,23.19c2,0,3.38-.62,4.06-1.86a12.78,12.78,0,0,0,1-5.84,12.78,12.78,0,0,0-1-5.84c-.68-1.24-2-1.86-4.06-1.86H96.19v15.4ZM41.57,21.77a4.5,4.5,0,0,0-.51-6.2l-.29-.23a4.4,4.4,0,0,0-3.14-.86,4.45,4.45,0,0,0-2.9,1.5,1.79,1.79,0,0,0-.22.25,41.63,41.63,0,0,0-6.45,10.92A31.56,31.56,0,0,0,28,50.43a40.44,40.44,0,0,0,6.49,10.93,4.49,4.49,0,0,0,6.31.65h0a4.5,4.5,0,0,0,.65-6.32h0a31.57,31.57,0,0,1-5-8.36,22.93,22.93,0,0,1-1.65-8.51,23.32,23.32,0,0,1,1.71-8.56,32.39,32.39,0,0,1,5.08-8.49ZM22.4,7.54a1.37,1.37,0,0,0,.19-.23,4.45,4.45,0,0,0-.43-6.1,1.37,1.37,0,0,0-.22-.2,4.48,4.48,0,0,0-6.18.5A67.93,67.93,0,0,0,3.84,19.91,46.8,46.8,0,0,0,0,39,46.8,46.8,0,0,0,4.47,58,68.05,68.05,0,0,0,17,76.05l.11.11a4.48,4.48,0,0,0,6.23.08l.11-.1a4.56,4.56,0,0,0,1.31-3,4.48,4.48,0,0,0-1.22-3.21A59.49,59.49,0,0,1,12.72,54.48,38.08,38.08,0,0,1,9,38.89a38.45,38.45,0,0,1,3.23-15.72A59,59,0,0,1,22.4,7.54ZM81.31,21.77a4.5,4.5,0,0,1,.51-6.2l.28-.23a4.49,4.49,0,0,1,6.05.64,1.27,1.27,0,0,1,.21.25,41.4,41.4,0,0,1,6.46,10.92,31.56,31.56,0,0,1,.07,23.28A40.44,40.44,0,0,1,88.4,61.36a4.49,4.49,0,0,1-6.31.65h0a4.48,4.48,0,0,1-1.63-3,4.55,4.55,0,0,1,1-3.3h0a31.57,31.57,0,0,0,5-8.36,22.93,22.93,0,0,0,1.65-8.51,23.32,23.32,0,0,0-1.71-8.56,32.39,32.39,0,0,0-5.08-8.49ZM100.48,7.54a1.37,1.37,0,0,1-.19-.23,4.45,4.45,0,0,1,.43-6.1,1.37,1.37,0,0,1,.22-.2,4.48,4.48,0,0,1,6.18.5A67.93,67.93,0,0,1,119,19.91,46.8,46.8,0,0,1,122.87,39a46.8,46.8,0,0,1-4.46,19,68.05,68.05,0,0,1-12.52,18l-.11.11a4.48,4.48,0,0,1-6.23.08l-.11-.1a4.56,4.56,0,0,1-1.31-3,4.48,4.48,0,0,1,1.22-3.21,59.49,59.49,0,0,0,10.81-15.42,38.08,38.08,0,0,0,3.76-15.59,38.45,38.45,0,0,0-3.23-15.72A59,59,0,0,0,100.48,7.54Z"
                />
              </svg>
            </span>
            <span class="scan-tag-title ml-3 text-xl font-bold">Scan Tag</span>
          </h5>
        </div>

        <p>
          Please place the card/tag on the scanner. (only admin cards/tags are
          eligible.)
        </p>
        <div class="flex justify-end space-x-2">
          <button
            id="cancelBtn"
            class="px-4 py-2 bg-primary text-primary-foreground rounded-md"
          >
            Cancel
          </button>
        </div>
      </div>
    </div>
    <script>
      const tagModal = document.getElementById("tagModal");
      const loginViaTagButton = document.querySelector("button.bg-secondary");
      const cancelBtn = document.getElementById("cancelBtn");

      loginViaTagButton.onclick = function () {
        tagModal.classList.remove("hidden");
      };

      cancelBtn.onclick = function () {
        tagModal.classList.add("hidden");
      };
    </script>
  </body>
</html>
)=====";

  homePage = R"(
<html>
<head>
<title>Home</title>
</head>
<body>
<h2>Home Page</h2>
<p>Welcome to the home page!</p>
</body>
</html>
)";
}

// Method to start the access point and web server
void SETUP::begin() {
  // Set up the ESP32 as an access point
  WiFi.softAP(apSSID, apPassword);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point started. IP address: ");
  Serial.println(IP);

  // Setup server routes
  server.on("/", [this]() {
    handleLogin();
  });
  server.on("/login", HTTP_POST, [this]() {
    checkLogin();
  });  // Handle login POST request
  server.on("/home", [this]() {
    handleHome();
  });

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
