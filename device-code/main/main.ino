#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_Access_Point";
const char* password = "12345678";

WebServer server(80);

const char* loginUser = "admin";
const char* loginPassword = "admin";

bool isAuthenticated = false;

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login-Server Control</title>
    <style>
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol";
            background: linear-gradient(45deg, #1a1a2e 0%, #16213e 100%);
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            padding: 1rem;
        }

        .login-container {
            width: 100%;
            max-width: 400px;
        }

        .login-card {
            background: rgba(255, 255, 255, 0.05);
            border-radius: 16px;
            box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);
            backdrop-filter: blur(5px);
            -webkit-backdrop-filter: blur(5px);
            border: 1px solid rgba(255, 255, 255, 0.1);
            padding: 2rem;
        }

        h2 {
            color: #ffffff;
            font-size: 1.8rem;
            font-weight: 700;
            margin-bottom: 0.5rem;
            text-align: center;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
        }

        .input-group {
            margin-bottom: 1rem;
        }

        label {
            display: block;
            color: #a0a0a0;
            font-size: 0.875rem;
            margin-bottom: 0.25rem;
        }

        input {
            width: 100%;
            padding: 0.5rem 0.75rem;
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid rgba(255, 255, 255, 0.2);
            border-radius: 8px;
            color: #ffffff;
            font-size: 1rem;
        }

        input::placeholder {
            color: rgba(255, 255, 255, 0.3);
        }

        input:focus {
            outline: none;
            background: rgba(255, 255, 255, 0.1);
            box-shadow: 0 0 0 2px rgba(255, 255, 255, 0.2);
        }

        button {
            width: 100%;
            background: rgb(0, 93, 222);
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 0.8rem 1rem;
            font-size: 1rem;
            font-weight: bold;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        button:hover {
            background: rgb(0, 74, 192);
        }

        .rfid-login {
            display: block;
            width: 100%;
            background: rgb(0, 93, 222);
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 0.8rem 1rem;
            font-size: 1rem;
            cursor: pointer;
            transition: background-color 0.3s ease;
            margin-top: 1rem;
            text-align: center;
            text-decoration: none;
        }

        .rfid-login:hover {
            background: rgb(0, 74, 192);
        }

        .divider {
            display: flex;
            align-items: center;
            text-align: center;
            margin: 1rem 0;
            color: #a0a0a0;
        }

        .divider::before,
        .divider::after {
            content: '';
            flex: 1;
            border-bottom: 1px solid rgba(255, 255, 255, 0.2);
        }

        .divider::before {
            margin-right: .5em;
        }

        .divider::after {
            margin-left: .5em;
        }
        .p-text{
            font-size: 0.9rem;
            color: white;
            text-align: center;
            margin-bottom: 1.5rem;
        }
    </style>
</head>
<body>
    <div class="login-container">
        <div class="login-card">
            <h2>Login</h2>
            <p class="p-text">(only autherized devide can access the dashboard)</p>
            <form method="POST" action="/login">
                <div class="input-group">
                    <label for="userid">Username</label>
                    <input type="text" id="userid" name="userid" placeholder="Enter your userid">
                </div>
                <div class="input-group">
                    <label for="password">Password</label>
                    <input type="password" id="password" name="password" placeholder="Enter your password">
                </div>
                <button type="submit">Sign In</button>
            </form>
            <div class="divider">or</div>
            <button class="rfid-login">Login with RFID</button>
        </div>
    </div>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleLogin() {
  if (server.hasArg("userid") && server.hasArg("password")) {
    String user = server.arg("userid");
    String pass = server.arg("password");
    if (user == loginUser && pass == loginPassword) {
      isAuthenticated = true;
      server.sendHeader("Location", "/configuration");
      server.send(302, "text/plain", "Redirecting to configuration...");
    } else {
      isAuthenticated = false;
      server.send(401, "text/plain", "Authentication Failed!");
      handleRoot();
    }
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}


void handleConfiguration() {
  if (isAuthenticated) {
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Configuration-server controller</title>
    <style>
      * {
        box-sizing: border-box;
        margin: 0;
        padding: 0;
      }
      .table-ele {
        word-wrap: break-word;
        white-space: normal;
      }
      body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
          Helvetica, Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji",
          "Segoe UI Symbol";
        background: linear-gradient(45deg, #1a1a2e 0%, #16213e 100%);
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
        padding: 1rem;
      }

      .container {
        width: 100%;
        max-width: 500px;
      }

      .desktop-layout,
      .mobile-layout {
        width: 100%;
      }

      .tabs {
        display: flex;
        background: rgba(255, 255, 255, 0.05);
        border-radius: 16px 16px 0 0;
        overflow: hidden;
      }

      .tab {
        flex: 1;
        padding: 0.9rem;
        background: rgba(255, 255, 255, 0.05);
        text-align: center;
        color: #ffffff;
        cursor: pointer;
        transition: background-color 0.3s ease;
        border-bottom: 1px solid rgba(255, 255, 255, 0.2);
        white-space: nowrap;
        font-size: 1rem;
      }

      .tab.active {
        background: rgb(0, 93, 222);
      }

      .content {
        background: rgba(255, 255, 255, 0.05);
        border-radius: 0 0 16px 16px;
        box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);
        backdrop-filter: blur(5px);
        -webkit-backdrop-filter: blur(5px);
        border: 1px solid rgba(255, 255, 255, 0.1);
        padding: 2rem;
      }

      .desktop-layout .content {
        display: none;
      }

      .desktop-layout .content.active {
        display: block;
      }

      h2 {
        color: #ffffff;
        font-size: 1.5rem;
        font-weight: 700;
        margin-bottom: 1rem;
        text-align: center;
        text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
      }

      .input-group {
        margin-bottom: 1rem;
      }

      label {
        display: block;
        color: #a0a0a0;
        font-size: 0.875rem;
        margin-bottom: 0.25rem;
      }

      input {
        width: 100%;
        padding: 0.5rem 0.75rem;
        background: rgba(255, 255, 255, 0.05);
        border: 1px solid rgba(255, 255, 255, 0.2);
        border-radius: 8px;
        color: #ffffff;
        font-size: 1rem;
      }

      input::placeholder {
        color: rgba(255, 255, 255, 0.3);
      }

      input:focus {
        outline: none;
        background: rgba(255, 255, 255, 0.1);
        box-shadow: 0 0 0 2px rgba(255, 255, 255, 0.2);
      }

      button {
        width: 100%;
        background: rgb(0, 93, 222);
        color: #ffffff;
        border: none;
        border-radius: 8px;
        padding: 0.8rem 1rem;
        font-size: 1rem;
        font-weight: bold;
        cursor: pointer;
        transition: background-color 0.3s ease;
      }

      button:hover {
        background: rgb(0, 74, 192);
      }

      .tags-table {
        width: 100%;
        border-collapse: separate;
        border-spacing: 0;
        margin-top: 1rem;
        background: rgba(255, 255, 255, 0.05);
        border-radius: 8px;
        overflow: hidden;
      }

      .tags-table th,
      .tags-table td {
        padding: 0.75rem 1rem;
        text-align: left;
        color: #ffffff;
        border-bottom: 1px solid rgba(255, 255, 255, 0.2);
        border-right: 1px solid rgba(255, 255, 255, 0.2);
      }

      .tags-table th:last-child,
      .tags-table td:last-child {
        border-right: none;
      }

      .tags-table th {
        background: rgba(255, 255, 255, 0.1);
        font-weight: 600;
      }

      .tags-table td {
        background: rgba(255, 255, 255, 0.05);
      }

      .tags-table tr:last-child td {
        border-bottom: none;
      }

      .table-action {
        display: flex;
        flex-direction: row;
        justify-content: flex-start;
        align-items: center;
      }

      .rfid-buttons {
        margin-top: 10px;
      }
      .rfid-buttons,
      .edit-delete {
        display: flex;
        flex-direction: row;
        gap: 10px;
      }
      .rfid-buttons button {
        padding: 10px 20px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        font-size: 16px;
      }

      .add-btn {
        background-color: #28f009eb;
      }

      .add-btn:hover {
        background-color: #20e202ce;
      }

      .edit-btn {
        background-color: #ebc404f0;
      }

      .edit-btn:hover {
        background-color: #ebc504ce;
      }

      .delete-btn {
        background-color: rgb(255, 68, 68);
      }

      .delete-btn:hover {
        background-color: rgba(253, 18, 18, 0.878);
      }

      .mobile-layout .tab {
        width: 100%;
        border-radius: 0;
      }

      .mobile-layout .content {
        display: none;
        border-radius: 0;
        margin-bottom: 1px;
      }

      .mobile-layout .tab.active + .content {
        display: block;
      }

      @media screen and (min-width: 601px) {
        .mobile-layout {
          display: none;
        }
      }

      @media screen and (max-width: 600px) {
        .desktop-layout {
          display: none;
        }

        .mobile-layout {
          display: block;
        }

        h2 {
          font-size: 1.2rem;
        }

        .tags-table {
          font-size: 0.9rem;
        }

        .tags-table th,
        .tags-table td {
          padding: 0.5rem;
        }
        .divider {
          border-top: 2px solid #ffffff;
          margin: 20px 0;
          width: 100%;
          opacity: 0.5;
        }
      }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="desktop-layout">
        <div class="tabs">
          <div class="tab active" data-tab="tab1">Wi-Fi Configuration</div>
          <div class="tab" data-tab="tab2">User Configuration</div>
          <div class="tab" data-tab="tab3">RFID Configuration</div>
        </div>
        <div class="content active" id="tab1">
          <h2>Wi-Fi Configuration</h2>
          <form>
            <div class="input-group">
              <label for="ssid">SSID</label>
              <input
                type="text"
                id="ssid"
                name="ssid"
                placeholder="Enter your Wi-Fi SSID"
              />
            </div>
            <div class="input-group">
              <label for="wifi-password">Wi-Fi Password</label>
              <input
                type="password"
                id="wifi-password"
                name="wifi-password"
                placeholder="Enter your Wi-Fi password"
              />
            </div>
            <button type="submit">Save Wi-Fi Settings</button>
          </form>
        </div>
        <div class="content" id="tab2">
          <h2>User Configuration</h2>
          <form>
            <div class="input-group">
              <label for="user-id">User ID</label>
              <input
                type="text"
                id="user-id"
                name="user-id"
                placeholder="Enter your User ID"
              />
            </div>
            <div class="input-group">
              <label for="user-password">User Password</label>
              <input
                type="password"
                id="user-password"
                name="user-password"
                placeholder="Enter your User password"
              />
            </div>
            <button type="submit">Save User Configuration</button>
          </form>
          <div class="divider"></div>
          <h2 style="margin-top: 30px">Access-Point Configuration</h2>
          <form>
            <div class="input-group">
              <label for="ap-ssid">AP SSID</label>
              <input
                type="text"
                id="ap-ssid"
                name="ap-ssid"
                placeholder="Enter AP SSID"
              />
            </div>
            <div class="input-group">
              <label for="ap-password">AP Password</label>
              <input
                type="password"
                id="ap-password"
                name="ap-password"
                placeholder="Enter AP password"
              />
            </div>
            <button type="submit">Save AP Configuration</button>
          </form>
        </div>
        <div class="content" id="tab3">
          <h2>RFID Configuration</h2>
          <table class="tags-table">
            <thead>
              <tr>
                <th class="table-ele">ID</th>
                <th class="table-ele">Name</th>
                <th class="table-ele">Role</th>
              </tr>
            </thead>
            <tbody id="table-body"></tbody>
          </table>
          <div class="rfid-buttons">
            <button class="add-btn">Add Tag</button>
            <div class="edit-delete">
              <button class="edit-btn">Edit</button>
              <button class="delete-btn">Delete</button>
            </div>
          </div>
        </div>
      </div>
      <div class="mobile-layout">
        <div class="tab active" data-tab="tab1">Wi-Fi Configuration</div>
        <div class="content" id="mobile-tab1">
          <h2>Wi-Fi Configuration</h2>
          <form>
            <div class="input-group">
              <label for="mobile-ssid">SSID</label>
              <input
                type="text"
                id="mobile-ssid"
                name="ssid"
                placeholder="Enter your Wi-Fi SSID"
              />
            </div>
            <div class="input-group">
              <label for="mobile-wifi-password">Wi-Fi Password</label>
              <input
                type="password"
                id="mobile-wifi-password"
                name="wifi-password"
                placeholder="Enter your Wi-Fi password"
              />
            </div>
            <button type="submit">Save Wi-Fi Settings</button>
          </form>
        </div>
        <div class="tab" data-tab="tab2">User Configuration</div>
        <div class="content" id="mobile-tab2">
          <h2>User Configuration</h2>
          <form>
            <div class="input-group">
              <label for="mobile-user-id">User ID</label>
              <input
                type="text"
                id="mobile-user-id"
                name="user-id"
                placeholder="Enter your User ID"
              />
            </div>
            <div class="input-group">
              <label for="mobile-user-password">User Password</label>
              <input
                type="password"
                id="mobile-user-password"
                name="user-password"
                placeholder="Enter your User password"
              />
            </div>
            <button type="submit">Save User Info</button>
          </form>
          <div class="divider"></div>
          <h2>Access-Point Configuration</h2>
          <form>
            <div class="input-group">
              <label for="ap-ssid">AP SSID</label>
              <input
                type="text"
                id="ap-ssid"
                name="ap-ssid"
                placeholder="Enter AP SSID"
              />
            </div>
            <div class="input-group">
              <label for="ap-password">AP Password</label>
              <input
                type="password"
                id="ap-password"
                name="ap-password"
                placeholder="Enter AP password"
              />
            </div>
            <button type="submit">Save AP Configuration</button>
          </form>
        </div>
        <div class="tab" data-tab="tab3">RFID Configuration</div>
        <div class="content" id="mobile-tab3">
          <h2>RFID Configuration</h2>
          <table class="tags-table">
            <thead>
              <tr>
                <th class="table-ele">ID</th>
                <th class="table-ele">Name</th>
                <th class="table-ele">Role</th>
              </tr>
            </thead>
            <tbody id="mobile-table-body"></tbody>
          </table>
          <div class="rfid-buttons">
            <button class="add-btn">Add Tag</button>
            <div class="edit-delete">
              <button class="edit-btn">Edit</button>
              <button class="delete-btn">Delete</button>
            </div>
          </div>
        </div>
      </div>
    </div>
    <script>
      const tableBodyMobile = document.getElementById("mobile-table-body");
      const tableBody = document.getElementById("table-body");
      const tags = [
        { id: 1, name: "Tag 1", role: "Admin" },
        { id: 2, name: "Tag 2", role: "User" },
      ];
      tags.forEach((tag) => {
        const row = document.createElement("tr");
        row.innerHTML = `
        <td class="table-ele">${tag.id}</td>
        <td class="table-ele">${tag.name}</td>
        <td class="table-ele">${tag.role}</td>`;
        tableBodyMobile.appendChild(row);
      });
      tags.forEach((tag) => {
        const row = document.createElement("tr");
        row.innerHTML = `
        <td class="table-ele">${tag.id}</td>
        <td class="table-ele">${tag.name}</td>
        <td class="table-ele">${tag.role}</td>`;
        tableBody.appendChild(row);
      });

      const desktopTabs = document.querySelectorAll(".desktop-layout .tab");
      const desktopContents = document.querySelectorAll(
        ".desktop-layout .content"
      );
      const mobileTabs = document.querySelectorAll(".mobile-layout .tab");
      const mobileContents = document.querySelectorAll(
        ".mobile-layout .content"
      );

      function setupTabs(tabs, contents) {
        tabs.forEach((tab) => {
          tab.addEventListener("click", () => {
            tabs.forEach((t) => t.classList.remove("active"));
            tab.classList.add("active");

            const target = tab.getAttribute("data-tab");
            contents.forEach((content) => {
              content.classList.remove("active");
              if (content.id.includes(target)) {
                content.classList.add("active");
              }
            });
          });
        });
      }

      setupTabs(desktopTabs, desktopContents);
      setupTabs(mobileTabs, mobileContents);
    </script>
  </body>
</html>

)rawliteral";

    server.send(200, "text/html", html);
  } else {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "Redirecting to login...");
  }
}

// Function to handle logout
void handleLogout() {
  isAuthenticated = false;
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting to login...");
}

// Set up the web server
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password); 
  Serial.println("Access Point Created");
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  server.on("/", handleRoot);                      
  server.on("/login", HTTP_POST, handleLogin);    
  server.on("/configuration", handleConfiguration);  
  server.on("/logout", handleLogout);               

  server.begin();  // Start the server
  Serial.println("Server started");
}

void loop() {
  server.handleClient();  // Handle client requests
}
