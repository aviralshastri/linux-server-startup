#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP32_Access_Point";
const char *password = "12345678";

WebServer server(80);

const char *loginUser = "admin";
const char *loginPassword = "admin";

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
      .table-ele {
        word-wrap: break-word;
        white-space: normal;
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

      .modal-overlay {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        display: none;
        background-color: rgba(0, 0, 0, 0.5);
        justify-content: center;
        align-items: center;
        z-index: 1000;
      }

      .modal-content {
        background-color: #181f39;
        color: white;
        padding: 30px;
        border-radius: 10px;
        box-shadow: 0 4px 20px rgba(0, 0, 0, 0.2);
        max-width: 350px;
        width: 100%;
      }

      .modal-title {
        font-size: 24px;
        margin-top: 0;
        margin-bottom: 15px;
      }

      .modal-description {
        font-size: 16px;
        line-height: 1.5;
        margin-bottom: 25px;
      }

      .modal-buttons {
        display: flex;
        justify-content: flex-end;
      }

      .modal-button {
        padding: 10px 20px;
        border: none;
        border-radius: 5px;
        font-size: 16px;
        cursor: pointer;
        transition: background-color 0.3s;
      }

      .cancel-button {
        background-color: rgb(4, 199, 4);
        color: white;
        margin-right: 10px;
      }

      .cancel-button:hover {
        background-color: rgb(2, 157, 2);
      }

      .delete-button {
        background-color: #f44336;
        color: white;
      }

      .delete-button:hover {
        background-color: #d32f2f;
      }

      .edit-modal-form-group {
        margin-bottom: 15px;
        text-align: left;
      }

      .edit-modal-form-group label {
        display: block;
        font-size: 14px;
        margin-bottom: 5px;
      }

      .edit-modal-form-group input,
      .edit-modal-form-group select {
        width: 100%;
        padding: 10px;
        font-size: 14px;
        border: 1px solid #ccc;
        border-radius: 5px;
      }

      .edit-modal-actions {
        margin-top: 20px;
        display: flex;
        justify-content: space-between;
      }

      .edit-modal-actions button {
        padding: 10px 20px;
        background-color: #007bff;
        color: white;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        width: 45%;
      }

      .edit-modal-actions button:hover {
        background-color: #0056b3;
      }

      .add-cancel-button {
        background-color: #f44336;
        color: white;
        margin-right: 10px;
      }

      .add-cancel-button:hover {
        background-color: #d32f2f;
      }

      .add-done-button {
        background-color: rgb(4, 199, 4);
        color: white;
      }

      .add-done-button:hover {
        background-color: rgb(2, 157, 2);
      }
      .add-scan-button {
        margin-top: 10px;
      }

      .toast {
        position: fixed;
        bottom: 30px;
        left: 50%;
        transform: translateX(-50%);
        background-color: rgba(51, 51, 51, 0.9);
        color: #fff;
        padding: 12px 24px;
        border-radius: 8px;
        box-shadow: 0 4px 15px rgba(0, 0, 0, 0.15);
        z-index: 1000;
        transition: all 0.3s ease;
        opacity: 0;
        visibility: hidden;
        display: flex;
        align-items: center;
        max-width: 90%;
        backdrop-filter: blur(5px);
      }

      .toast-icon {
        margin-right: 12px;
        font-size: 20px;
      }

      .toast-message {
        font-family: "Arial", sans-serif;
        font-size: 14px;
        line-height: 1.4;
      }

      .toast-close {
        margin-left: 12px;
        cursor: pointer;
        opacity: 0.7;
        transition: opacity 0.2s ease;
      }

      .toast-close:hover {
        opacity: 1;
      }

      @keyframes toast-in {
        from {
          transform: translateX(-50%) translateY(100%);
          opacity: 0;
        }
        to {
          transform: translateX(-50%) translateY(0);
          opacity: 1;
        }
      }

      @keyframes toast-out {
        from {
          transform: translateX(-50%) translateY(0);
          opacity: 1;
        }
        to {
          transform: translateX(-50%) translateY(100%);
          opacity: 0;
        }
      }

      .toast.animate-in {
        animation: toast-in 0.3s ease forwards;
      }

      .toast.animate-out {
        animation: toast-out 0.3s ease forwards;
      }

      @keyframes toast-in {
        from {
          transform: translateX(-50%) translateY(100%);
          opacity: 0;
        }
        to {
          transform: translateX(-50%) translateY(0);
          opacity: 1;
        }
      }

      @keyframes toast-out {
        from {
          transform: translateX(-50%) translateY(0);
          opacity: 1;
        }
        to {
          transform: translateX(-50%) translateY(100%);
          opacity: 0;
        }
      }

      .toast.animate-in {
        animation: toast-in 0.3s ease forwards;
      }

      .toast.animate-out {
        animation: toast-out 0.3s ease forwards;
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

        .toast {
          padding: 8px 8px;
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
          <div class="input-group">
            <label for="wifi-ssid">SSID</label>
            <input
              autocomplete="off"
              type="text"
              id="wifi-ssid"
              name="wifi-ssid"
              placeholder="Enter your Wi-Fi SSID"
            />
          </div>
          <div class="input-group">
            <label for="wifi-password">Wi-Fi Password</label>
            <input
              autocomplete="off"
              type="password"
              id="wifi-password"
              name="wifi-password"
              placeholder="Enter your Wi-Fi password"
            />
          </div>
          <button class="saveWifiConfig">Save Wi-Fi Settings</button>
        </div>
        <div class="content" id="tab2">
          <h2>User Configuration</h2>
          <div class="input-group">
            <label for="user-id">User ID</label>
            <input
              autocomplete="off"
              type="text"
              id="user-id"
              name="user-id"
              placeholder="Enter your User ID"
            />
          </div>
          <div class="input-group">
            <label for="user-password">User Password</label>
            <input
              autocomplete="off"
              type="password"
              id="user-password"
              name="user-password"
              placeholder="Enter your User password"
            />
          </div>
          <button class="saveUserConfig">Save User Configuration</button>
          <div class="divider"></div>
          <h2 style="margin-top: 30px">Access-Point Configuration</h2>
          <div class="input-group">
            <label for="ap-ssid">AP SSID</label>
            <input
              autocomplete="off"
              type="text"
              id="ap-ssid"
              name="ap-ssid"
              placeholder="Enter AP SSID"
            />
          </div>
          <div class="input-group">
            <label for="ap-password">AP Password</label>
            <input
              autocomplete="off"
              type="password"
              id="ap-password"
              name="ap-password"
              placeholder="Enter AP password"
            />
          </div>
          <button class="saveAPConfig">Save AP Configuration</button>
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
            <button class="add-btn" id="add-tag">Add Tag</button>
            <div class="edit-delete">
              <button class="edit-btn" id="edit-tag">Edit</button>
              <button class="delete-btn" id="delete-tag">Delete</button>
            </div>
          </div>
        </div>
      </div>
      <div class="mobile-layout">
        <div class="tab active" data-tab="tab1">Wi-Fi Configuration</div>
        <div class="content" id="mobile-tab1">
          <h2>Wi-Fi Configuration</h2>
          <div class="input-group">
            <label for="mobile-wifi-ssid">SSID</label>
            <input
              autocomplete="off"
              type="text"
              id="mobile-wifi-ssid"
              name="wifi-ssid"
              placeholder="Enter your Wi-Fi SSID"
            />
          </div>
          <div class="input-group">
            <label for="mobile-wifi-password">Wi-Fi Password</label>
            <input
              autocomplete="off"
              type="password"
              id="mobile-wifi-password"
              name="wifi-password"
              placeholder="Enter your Wi-Fi password"
            />
          </div>
          <button class="saveWifiConfig">Save Wi-Fi Settings</button>
        </div>
        <div class="tab" data-tab="tab2">User Configuration</div>
        <div class="content" id="mobile-tab2">
          <h2>User Configuration</h2>
          <div class="input-group">
            <label for="mobile-user-id">User ID</label>
            <input
              autocomplete="off"
              type="text"
              id="mobile-user-id"
              name="user-id"
              placeholder="Enter your User ID"
            />
          </div>
          <div class="input-group">
            <label for="mobile-user-password">User Password</label>
            <input
              autocomplete="off"
              type="password"
              id="mobile-user-password"
              name="user-password"
              placeholder="Enter your User password"
            />
          </div>
          <button class="saveUserConfig">Save User Configuration</button>
          <div class="divider"></div>
          <h2>Access-Point Configuration</h2>
          <div class="input-group">
            <label for="mobile-ap-ssid">AP SSID</label>
            <input
              autocomplete="off"
              type="text"
              id="mobile-ap-ssid"
              name="ap-ssid"
              placeholder="Enter AP SSID"
            />
          </div>
          <div class="input-group">
            <label for="mobile-ap-password">AP Password</label>
            <input
              autocomplete="off"
              type="password"
              id="mobile-ap-password"
              name="ap-password"
              placeholder="Enter AP password"
            />
          </div>
          <button class="saveAPConfig">Save AP Configuration</button>
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
            <button class="add-btn" id="add-tag-mobile">Add Tag</button>
            <div class="edit-delete">
              <button class="edit-btn" id="edit-tag-mobile">Edit</button>
              <button class="delete-btn" id="delete-tag-mobile">Delete</button>
            </div>
          </div>
        </div>
      </div>
    </div>
    <div id="modal" class="modal-overlay">
      <div class="modal-content" id="modal-main-content"></div>
    </div>
    <div class="toast" id="toast"></div>

    <script>
      const invalidIDChars = /[^a-zA-Z0-9 _]/;
      const invalidPasswordChars = /['"]/;

      let selectedtag = { id: "", name: "", role: "" };
      const toast = document.getElementById("toast");
      const modalContent = document.getElementById("modal-main-content");
      const modal = document.getElementById("modal");

      const addButton = document.getElementById("add-tag");
      addButton.addEventListener("click", async function () {
        modalContent.innerHTML = `
            <h2 class="modal-title">Add Item</h2>
                    <div class="edit-modal-form-group">
                          <label for="addId">ID</label>
                          <input autocomplete="off" type="text" id="addId" name="name" placeholder="Scan a tag" required disabled>
                          <button type="button" id="scanButton" class="add-scan-button">Scan</button>
                      </div>
                    <div class="edit-modal-form-group">
                          <label for="editName">Name</label>
                          <input autocomplete="off" type="text" id="editName" name="name" placeholder="Enter name" required>
                      </div>
                      <div class="edit-modal-form-group">
                          <label for="editRole">Role</label>
                          <select id="editRole" name="role">
                              <option value="admin">Admin</option>
                              <option value="user">User</option>
                          </select>
                      </div>
              <div class="modal-buttons">
                <button class="modal-button add-cancel-button" onclick="closeModal()">
                  Cancel
                </button>
                <button
                  class="modal-button add-done-button"
                  onclick="confirmAdding()"
                >
                  Done
                </button>
              </div>
            `;
        modal.style.display = "flex";
        //sendRFIDRequest("/add");
      });

      const deleteButton = document.getElementById("delete-tag");
      deleteButton.addEventListener("click", async function () {
        if (!selectedtag.id) {
          showToast("Please select a tag first");
          return;
        }
        modalContent.innerHTML = `
            <h2 class="modal-title">Delete Item</h2>
              <p class="modal-description">
                Are you sure you want to delete tag with ID:${selectedtag.id}? This action
                cannot be undone.
              </p>
              <div class="modal-buttons">
                <button class="modal-button cancel-button" onclick="closeModal()">
                  Cancel
                </button>
                <button
                  class="modal-button delete-button"
                  onclick="confirmDeletion()"
                >
                  Delete
                </button>
              </div>
            `;
        modal.style.display = "flex";
        //sendRFIDRequest("/delete");
      });

      const editButton = document.getElementById("edit-tag");
      editButton.addEventListener("click", async function () {
        if (!selectedtag.id) {
          showToast("Please select a tag first");
          return;
        }
        modalContent.innerHTML = `
            <h2 class="modal-title">Edit Item</h2>
            <form action="/edit" method="POST">
                      <div class="edit-modal-form-group">
                          <label for="editName">Name</label>
                          <input autocomplete="off" type="text" id="editName" name="name" placeholder="Enter name" required>
                      </div>
                      <div class="edit-modal-form-group">
                          <label for="editRole">Role</label>
                          <select id="editRole" name="role">
                              <option value="admin">Admin</option>
                              <option value="user">User</option>
                          </select>
                      </div>
                      <div class="edit-modal-actions">
                          <button type="button" onclick="closeModal()" >Cancel</button>
                          <button type="submit">Save Changes</button>
                      </div>
                  </form>
            `;
        let name = document.getElementById("editName");
        let role = document.getElementById("editRole");
        name.value = selectedtag.name;
        role.value = selectedtag.role.toLowerCase();
        modal.style.display = "flex";
        //sendRFIDRequest("/edit");
      });

      const addButtonMobile = document.getElementById("add-tag-mobile");
      addButtonMobile.addEventListener("click", async function () {
        modalContent.innerHTML = `
            <h2 class="modal-title">Add Item</h2>
                    <div class="edit-modal-form-group">
                          <label for="addId">ID</label>
                          <input autocomplete="off" type="text" id="addId" name="name" placeholder="Scan a tag" required disabled>
                          <button type="button" id="scanButton" class="add-scan-button">Scan</button>
                      </div>
                    <div class="edit-modal-form-group">
                          <label for="editName">Name</label>
                          <input autocomplete="off" type="text" id="editName" name="name" placeholder="Enter name" required>
                      </div>
                      <div class="edit-modal-form-group">
                          <label for="editRole">Role</label>
                          <select id="editRole" name="role">
                              <option value="admin">Admin</option>
                              <option value="user">User</option>
                          </select>
                      </div>
              <div class="modal-buttons">
                <button class="modal-button add-cancel-button" onclick="closeModal()">
                  Cancel
                </button>
                <button
                  class="modal-button add-done-button"
                  onclick="confirmAdding()"
                >
                  Done
                </button>
              </div>
            `;
        modal.style.display = "flex";
        //sendRFIDRequest("/add");
      });

      const deleteButtonMobile = document.getElementById("delete-tag-mobile");
      deleteButtonMobile.addEventListener("click", async function () {
        if (!selectedtag.id) {
          showToast("Please select a tag first");
          return;
        }
        modalContent.innerHTML = `
            <h2 class="modal-title">Delete Item</h2>
              <p class="modal-description">
                Are you sure you want to delete tag with ID:${selectedtag.id}? This action
                cannot be undone.
              </p>
              <div class="modal-buttons">
                <button class="modal-button cancel-button" onclick="closeModal()">
                  Cancel
                </button>
                <button
                  class="modal-button delete-button"
                  onclick="confirmDeletion()"
                >
                  Delete
                </button>
              </div>
            `;
        modal.style.display = "flex";
        //sendRFIDRequest("/delete");
      });
      const editButtonMobile = document.getElementById("edit-tag-mobile");
      editButtonMobile.addEventListener("click", async function () {
        if (!selectedtag.id) {
          showToast("Please select a tag first");
          return;
        }
        modalContent.innerHTML = `
            <h2 class="modal-title">Edit Item</h2>
            <form action="/edit" method="POST">
                      <div class="edit-modal-form-group">
                          <label for="editName">Name</label>
                          <input autocomplete="off" type="text" id="editName" name="name" placeholder="Enter name" required>
                      </div>
                      <div class="edit-modal-form-group">
                          <label for="editRole">Role</label>
                          <select id="editRole" name="role">
                              <option value="admin">Admin</option>
                              <option value="user">User</option>
                          </select>
                      </div>
                      <div class="edit-modal-actions">
                          <button type="button" onclick="closeModal()" >Cancel</button>
                          <button type="submit">Save Changes</button>
                      </div>
                  </form>
            `;
        let name = document.getElementById("editName");
        let role = document.getElementById("editRole");
        name.value = selectedtag.name;
        role.value = selectedtag.role.toLowerCase();
        modal.style.display = "flex";
        modal.style.display = "flex";
        //sendRFIDRequest("/edit");
      });

      async function sendRFIDRequest(url) {
        const formData = new FormData();
        formData.append("id", selectedtag.id);
        formData.append("name", selectedtag.name);
        formData.append("role", selectedtag.role);

        try {
          const response = await fetch(url, {
            method: "POST",
            body: formData,
          });

          if (response.ok) {
            console.log("Request sent successfully");
            alert("Operation completed successfully");
          } else {
            console.error("Failed to send request");
            alert("Operation failed");
          }
        } catch (error) {
          console.error("Error:", error);
          alert("An error occurred");
        }
      }

      function populateTable() {
        const tags = {
          avb2ad32a: { name: "Tag 1", role: "Admin" },
          a321fdsff: { name: "Tag 2", role: "User" },
        };

        const tableBodyMobile = document.getElementById("mobile-table-body");
        Object.entries(tags).forEach(([id, tag]) => {
          const row = document.createElement("tr");
          row.id = id;
          row.innerHTML = `
            <td class="table-ele">${id}</td>
            <td class="table-ele">${tag.name}</td>
            <td class="table-ele">${tag.role}</td>`;
          row.addEventListener("click", () => {
            if (selectedRow === row) {
              row.style.backgroundColor = "";
              selectedtag["id"] = "";
              selectedtag["name"] = "";
              selectedtag["role"] = "";
              selectedRow = null;
            } else {
              if (selectedRow) {
                selectedRow.style.backgroundColor = "";
              }
              row.style.backgroundColor = "#2b2d38";
              selectedtag["id"] = row.id;
              selectedtag["name"] = tags[row.id]["name"];
              selectedtag["role"] = tags[row.id]["role"];
              selectedRow = row;
            }
          });

          tableBodyMobile.appendChild(row);
        });

        const tableBody = document.getElementById("table-body");
        let selectedRow = null;

        Object.entries(tags).forEach(([id, tag]) => {
          const row = document.createElement("tr");
          row.id = id;
          row.innerHTML = `
            <td class="table-ele">${id}</td>
            <td class="table-ele">${tag.name}</td>
            <td class="table-ele">${tag.role}</td>`;
          row.addEventListener("click", () => {
            if (selectedRow === row) {
              row.style.backgroundColor = "";
              selectedtag["id"] = "";
              selectedtag["name"] = "";
              selectedtag["role"] = "";
              selectedRow = null;
            } else {
              if (selectedRow) {
                selectedRow.style.backgroundColor = "";
              }
              row.style.backgroundColor = "#2b2d38";
              selectedtag["id"] = row.id;
              selectedtag["name"] = tags[row.id]["name"];
              selectedtag["role"] = tags[row.id]["role"];
              selectedRow = row;
            }
          });

          tableBody.appendChild(row);
        });
      }

      const desktopTabs = document.querySelectorAll(".desktop-layout .tab");
      const desktopContents = document.querySelectorAll(
        ".desktop-layout .content"
      );
      const mobileTabs = document.querySelectorAll(".mobile-layout .tab");
      const mobileContents = document.querySelectorAll(
        ".mobile-layout .content"
      );

      async function sendConfigRequest(url, id, password) {
        const formData = new FormData();
        if (url === "/saveWifiConfig") {
          formData.append("wifissid", id);
          formData.append("wifipassword", password);
        } else if (url === "/saveUserConfig") {
          formData.append("userid", id);
          formData.append("userpassword", password);
        } else if (url === "/saveAPConfig") {
          formData.append("apid", id);
          formData.append("appassword", password);
        } else {
          showToast("Invalid config path!");
          return;
        }

        try {
          const response = await fetch(url, {
            method: "POST",
            body: formData,
          });

          if (response.ok) {
            console.log("Request sent successfully");
            alert("Operation completed successfully");
          } else {
            console.error("Failed to send request");
            alert("Operation failed");
          }
        } catch (error) {
          console.error("Error:", error);
          alert("An error occurred");
        }
      }

      const saveUserConfigs = document.querySelectorAll(".saveUserConfig");
      saveUserConfigs.forEach((saveUserConfig) => {
        saveUserConfig.addEventListener("click", handleSaveUserConfig);
      });
      const saveAPConfigs = document.querySelectorAll(".saveAPConfig");
      saveAPConfigs.forEach((saveAPConfig) => {
        saveAPConfig.addEventListener("click", handleSaveAPConfig);
      });
      const saveWifiConfigs = document.querySelectorAll(".saveWifiConfig");
      saveWifiConfigs.forEach((saveWifiConfig) => {
        saveWifiConfig.addEventListener("click", handleSaveWifiConfig);
      });

      function handleSaveUserConfig() {
        const userid =
          document.getElementById("user-id").value === ""
            ? document.getElementById("mobile-user-id").value
            : document.getElementById("user-id").value;
        const userpassword =
          document.getElementById("user-password").value === ""
            ? document.getElementById("mobile-user-password").value
            : document.getElementById("user-password").value;

        if (userid === "") {
          showToast("User ID cannot be empty");
          return;
        }
        if (userpassword === "") {
          showToast("User Password cannot be empty");
          return;
        }

        if (invalidIDChars.test(userid)) {
          showToast(
            "User ID contains invalid characters! (allowed special characters: underscore and space)"
          );
          return;
        }
        if (invalidPasswordChars.test(userpassword)) {
          showToast("User Password contains invalid characters: \" , '");
          return;
        }

        sendConfigRequest("/saveUserConfig", userid, userpassword);
      }

      function handleSaveAPConfig() {
        const apssid =
          document.getElementById("ap-ssid").value === ""
            ? document.getElementById("mobile-ap-ssid").value
            : document.getElementById("ap-ssid").value;
        const appassword =
          document.getElementById("ap-password").value === ""
            ? document.getElementById("mobile-ap-password").value
            : document.getElementById("ap-password").value;

        if (apssid === "") {
          showToast("AP SSID cannot be empty");
          return;
        }
        if (appassword === "") {
          showToast("AP Password cannot be empty");
          return;
        }

        if (invalidIDChars.test(apssid)) {
          showToast(
            "AP SSID contains invalid characters! (allowed special characters: underscore and space)"
          );
          return;
        }
        if (invalidPasswordChars.test(appassword)) {
          showToast("AP Password contains invalid characters: \" , '");
          return;
        }

        sendConfigRequest("/saveAPConfig", apssid, appassword);
      }

      function handleSaveWifiConfig() {
        const wifissid =
          document.getElementById("wifi-ssid").value === ""
            ? document.getElementById("mobile-wifi-ssid").value
            : document.getElementById("wifi-ssid").value;
        const wifipassword =
          document.getElementById("wifi-password").value === ""
            ? document.getElementById("mobile-wifi-password").value
            : document.getElementById("wifi-password").value;

        if (wifissid === "") {
          showToast("Wifi SSID cannot be empty");
          return;
        }
        if (wifipassword === "") {
          showToast("Wifi Password cannot be empty");
          return;
        }

        if (invalidIDChars.test(wifissid)) {
          showToast(
            "Wifi SSID contains invalid characters! (allowed special characters: underscore and space)"
          );
          return;
        }
        if (invalidPasswordChars.test(wifipassword)) {
          showToast("Wifi Password contains invalid characters: \" , '");
          return;
        }

        sendConfigRequest("/saveWifiConfig", wifissid, wifipassword);
      }

      function closeModal() {
        modal.style.display = "none";
        modalContent.innerHTML = "<h2>No valid button clicked.<h2/>";
      }

      function closeToast() {
        toast.style.visibility = "hidden";
        toast.style.opacity = "0";
      }

      function showToast(message) {
        toast.innerHTML = `
              <span class="toast-icon"><svg xmlns="http://www.w3.org/2000/svg" x="0px" y="0px" width="25" height="25" viewBox="0 0 48 48">
      <path fill="#2196f3" d="M44,24c0,11.045-8.955,20-20,20S4,35.045,4,24S12.955,4,24,4S44,12.955,44,24z"></path><path fill="#fff" d="M22 22h4v11h-4V22zM26.5 16.5c0 1.379-1.121 2.5-2.5 2.5s-2.5-1.121-2.5-2.5S22.621 14 24 14 26.5 15.121 26.5 16.5z"></path>
      </svg></span>
            <span class="toast-message" id="toast-message">${message}</span>`;
        toast.style.visibility = "visible";
        toast.style.opacity = "1";
        setTimeout(closeToast, 5000);
      }

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

      populateTable();
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
  if (server.hasArg("id") && server.hasArg("name") && server.hasArg("role")) {
    String tag_id = server.arg("id");
    String name = server.arg("name");
    String role = server.arg("role");
    Serial.println(tag_id);
    Serial.println(name);
    Serial.println(role);
    server.send(200, "text/plain", "Tag deleted successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleEdit() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("Edit called!");
  if (server.hasArg("id") && server.hasArg("name") && server.hasArg("role")) {
    String tag_id = server.arg("id");
    String name = server.arg("name");
    String role = server.arg("role");
    Serial.println(tag_id);
    Serial.println(name);
    Serial.println(role);
    server.send(200, "text/plain", "Tag edited successfully");
  } else {
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
    server.send(200, "text/plain", "Tag edited successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleSaveWifiConfig() {
  if (!isAuthenticated) {
    return;
  }
  Serial.println("Wifi config called!");
  if (server.hasArg("wifissid") && server.arg("wifipassword")) {
    String wifissid = server.arg("wifissid");
    String wifipassword = server.arg("wifipassword");
    Serial.println(wifissid);
    Serial.println(wifipassword);
    server.send(200, "text/plain", "Tag edited successfully");
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
    server.send(200, "text/plain", "Tag edited successfully");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

void handleLogout() {
  isAuthenticated = false;
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting to login...");
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Created");
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  server.on("/", handleRoot);
  server.on("/configuration", handleConfiguration);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", handleLogout);
  server.on("/saveUserConfig", HTTP_POST, handleSaveUserConfig);
  server.on("/saveAPConfig", HTTP_POST, handleSaveAPConfig);
  server.on("/saveWifiConfig", HTTP_POST, handleSaveWifiConfig);
  server.on("/add", HTTP_POST, handleAdd);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/edit", HTTP_POST, handleEdit);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
