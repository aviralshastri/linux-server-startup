#include "SETUP.h"


const char* SETUP::login = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <script>
        tailwind.config = {
            darkMode: 'class',
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
        }
    </script>
    <style type="text/css">
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap');
        
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
            font-family: 'Inter', sans-serif;
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
            <div class="bg-card border border-border rounded-lg shadow-lg p-6 space-y-6">
                <div class="space-y-2 text-center">
                    <h1 class="text-3xl font-bold">Login</h1>
                    <p class="text-muted-foreground">Enter your credentials to access device.<br/>(default password is written on the device display.)</p>
                </div>
                <div class="space-y-4">
                    <div class="space-y-2">
                        <label for="userid" class="text-sm font-medium leading-none peer-disabled:cursor-not-allowed peer-disabled:opacity-70">User ID</label>
                        <input type="text" id="userid" placeholder="Enter your user ID" class="flex h-10 w-full rounded-md border border-input bg-background px-3 py-2 text-sm ring-offset-background file:border-0 file:bg-transparent file:text-sm file:font-medium placeholder:text-muted-foreground focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:cursor-not-allowed disabled:opacity-50" />
                    </div>
                    <div class="space-y-2">
                        <label for="password" class="text-sm font-medium leading-none peer-disabled:cursor-not-allowed peer-disabled:opacity-70">Password</label>
                        <input type="password" id="password" placeholder="Enter your password" class="flex h-10 w-full rounded-md border border-input bg-background px-3 py-2 text-sm ring-offset-background file:border-0 file:bg-transparent file:text-sm file:font-medium placeholder:text-muted-foreground focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:cursor-not-allowed disabled:opacity-50" />
                    </div>
                    <div class="flex flex-col space-y-4">
                        <button class="inline-flex items-center justify-center rounded-md text-lg font-bold ring-offset-background transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:pointer-events-none disabled:opacity-50 bg-primary text-primary-foreground hover:bg-primary/90 h-10 px-4 py-2 w-full">Login</button>
                        <button 
    class="inline-flex items-center justify-center rounded-md text-sm font-medium ring-offset-background transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:pointer-events-none disabled:opacity-50 bg-secondary text-secondary-foreground hover:bg-secondary/80 h-10 px-4 py-2 w-full"
    onclick="tagModal.classList.remove('hidden')">
<span class="logo-container font-bold mr-2 size-6">
      <svg id="Layer_1" data-name="Layer 1" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 122.88 122.02" class="logo">
        <title>rfid</title>
        <path d="M62.19,48.52a9.83,9.83,0,0,0-.25-19.61h-1a9.83,9.83,0,0,0,0,19.63h1.25ZM24.53,91q6,0,9.29,2.7a8.86,8.86,0,0,1,3.24,7.16q0,5.11-3.17,7.7c-2.11,1.72-5.19,2.58-9.22,2.58l-1.08.56H18.42V122H7.72V91Zm-1.88,13.52a4.41,4.41,0,0,0,2.6-.63,3.27,3.27,0,0,0,0-4.44,4.26,4.26,0,0,0-2.6-.66H18.42v5.73Zm6.47,2.3L38.79,122H26.92l-7.23-13.56,9.43-1.6Zm37.37-7.79H51.75v4.32H63.67v8H51.75V122H40.81V91H66.49v8Zm14.08-8v31H69.63V91Zm19.9,0q7.27,0,11,4t3.71,11.45q0,7.36-3.71,11.43t-11,4.06H85.26V91Zm-1.55,23.19c2,0,3.38-.62,4.06-1.86a12.78,12.78,0,0,0,1-5.84,12.78,12.78,0,0,0-1-5.84c-.68-1.24-2-1.86-4.06-1.86H96.19v15.4ZM41.57,21.77a4.5,4.5,0,0,0-.51-6.2l-.29-.23a4.4,4.4,0,0,0-3.14-.86,4.45,4.45,0,0,0-2.9,1.5,1.79,1.79,0,0,0-.22.25,41.63,41.63,0,0,0-6.45,10.92A31.56,31.56,0,0,0,28,50.43a40.44,40.44,0,0,0,6.49,10.93,4.49,4.49,0,0,0,6.31.65h0a4.5,4.5,0,0,0,.65-6.32h0a31.57,31.57,0,0,1-5-8.36,22.93,22.93,0,0,1-1.65-8.51,23.32,23.32,0,0,1,1.71-8.56,32.39,32.39,0,0,1,5.08-8.49ZM22.4,7.54a1.37,1.37,0,0,0,.19-.23,4.45,4.45,0,0,0-.43-6.1,1.37,1.37,0,0,0-.22-.2,4.48,4.48,0,0,0-6.18.5A67.93,67.93,0,0,0,3.84,19.91,46.8,46.8,0,0,0,0,39,46.8,46.8,0,0,0,4.47,58,68.05,68.05,0,0,0,17,76.05l.11.11a4.48,4.48,0,0,0,6.23.08l.11-.1a4.56,4.56,0,0,0,1.31-3,4.48,4.48,0,0,0-1.22-3.21A59.49,59.49,0,0,1,12.72,54.48,38.08,38.08,0,0,1,9,38.89a38.45,38.45,0,0,1,3.23-15.72A59,59,0,0,1,22.4,7.54ZM81.31,21.77a4.5,4.5,0,0,1,.51-6.2l.28-.23a4.49,4.49,0,0,1,6.05.64,1.27,1.27,0,0,1,.21.25,41.4,41.4,0,0,1,6.46,10.92,31.56,31.56,0,0,1,.07,23.28A40.44,40.44,0,0,1,88.4,61.36a4.49,4.49,0,0,1-6.31.65h0a4.48,4.48,0,0,1-1.63-3,4.55,4.55,0,0,1,1-3.3h0a31.57,31.57,0,0,0,5-8.36,22.93,22.93,0,0,0,1.65-8.51,23.32,23.32,0,0,0-1.71-8.56,32.39,32.39,0,0,0-5.08-8.49ZM100.48,7.54a1.37,1.37,0,0,1-.19-.23,4.45,4.45,0,0,1,.43-6.1,1.37,1.37,0,0,1,.22-.2,4.48,4.48,0,0,1,6.18.5A67.93,67.93,0,0,1,119,19.91,46.8,46.8,0,0,1,122.87,39a46.8,46.8,0,0,1-4.46,19,68.05,68.05,0,0,1-12.52,18l-.11.11a4.48,4.48,0,0,1-6.23.08l-.11-.1a4.56,4.56,0,0,1-1.31-3,4.48,4.48,0,0,1,1.22-3.21,59.49,59.49,0,0,0,10.81-15.42,38.08,38.08,0,0,0,3.76-15.59,38.45,38.45,0,0,0-3.23-15.72A59,59,0,0,0,100.48,7.54Z"/>
      </svg>
    </span>

    <h1 class="text-md">Login</h1>
</button>

                    </div>
                </div>
            </div>
        </div>
    </div>
<div id="tagModal" class="fixed inset-0 hidden bg-black bg-opacity-50 flex items-center justify-center z-50">
    <div class="bg-card border border-border rounded-lg shadow-lg p-6 space-y-4 w-full max-w-sm">
        <div class="modal-header">
  <h5 class="modal-title">
<span class="logo-container text-xl font-bold ">
      <svg id="Layer_1" data-name="Layer 1" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 122.88 122.02" class="logo">
        <title>rfid</title>
        <path d="M62.19,48.52a9.83,9.83,0,0,0-.25-19.61h-1a9.83,9.83,0,0,0,0,19.63h1.25ZM24.53,91q6,0,9.29,2.7a8.86,8.86,0,0,1,3.24,7.16q0,5.11-3.17,7.7c-2.11,1.72-5.19,2.58-9.22,2.58l-1.08.56H18.42V122H7.72V91Zm-1.88,13.52a4.41,4.41,0,0,0,2.6-.63,3.27,3.27,0,0,0,0-4.44,4.26,4.26,0,0,0-2.6-.66H18.42v5.73Zm6.47,2.3L38.79,122H26.92l-7.23-13.56,9.43-1.6Zm37.37-7.79H51.75v4.32H63.67v8H51.75V122H40.81V91H66.49v8Zm14.08-8v31H69.63V91Zm19.9,0q7.27,0,11,4t3.71,11.45q0,7.36-3.71,11.43t-11,4.06H85.26V91Zm-1.55,23.19c2,0,3.38-.62,4.06-1.86a12.78,12.78,0,0,0,1-5.84,12.78,12.78,0,0,0-1-5.84c-.68-1.24-2-1.86-4.06-1.86H96.19v15.4ZM41.57,21.77a4.5,4.5,0,0,0-.51-6.2l-.29-.23a4.4,4.4,0,0,0-3.14-.86,4.45,4.45,0,0,0-2.9,1.5,1.79,1.79,0,0,0-.22.25,41.63,41.63,0,0,0-6.45,10.92A31.56,31.56,0,0,0,28,50.43a40.44,40.44,0,0,0,6.49,10.93,4.49,4.49,0,0,0,6.31.65h0a4.5,4.5,0,0,0,.65-6.32h0a31.57,31.57,0,0,1-5-8.36,22.93,22.93,0,0,1-1.65-8.51,23.32,23.32,0,0,1,1.71-8.56,32.39,32.39,0,0,1,5.08-8.49ZM22.4,7.54a1.37,1.37,0,0,0,.19-.23,4.45,4.45,0,0,0-.43-6.1,1.37,1.37,0,0,0-.22-.2,4.48,4.48,0,0,0-6.18.5A67.93,67.93,0,0,0,3.84,19.91,46.8,46.8,0,0,0,0,39,46.8,46.8,0,0,0,4.47,58,68.05,68.05,0,0,0,17,76.05l.11.11a4.48,4.48,0,0,0,6.23.08l.11-.1a4.56,4.56,0,0,0,1.31-3,4.48,4.48,0,0,0-1.22-3.21A59.49,59.49,0,0,1,12.72,54.48,38.08,38.08,0,0,1,9,38.89a38.45,38.45,0,0,1,3.23-15.72A59,59,0,0,1,22.4,7.54ZM81.31,21.77a4.5,4.5,0,0,1,.51-6.2l.28-.23a4.49,4.49,0,0,1,6.05.64,1.27,1.27,0,0,1,.21.25,41.4,41.4,0,0,1,6.46,10.92,31.56,31.56,0,0,1,.07,23.28A40.44,40.44,0,0,1,88.4,61.36a4.49,4.49,0,0,1-6.31.65h0a4.48,4.48,0,0,1-1.63-3,4.55,4.55,0,0,1,1-3.3h0a31.57,31.57,0,0,0,5-8.36,22.93,22.93,0,0,0,1.65-8.51,23.32,23.32,0,0,0-1.71-8.56,32.39,32.39,0,0,0-5.08-8.49ZM100.48,7.54a1.37,1.37,0,0,1-.19-.23,4.45,4.45,0,0,1,.43-6.1,1.37,1.37,0,0,1,.22-.2,4.48,4.48,0,0,1,6.18.5A67.93,67.93,0,0,1,119,19.91,46.8,46.8,0,0,1,122.87,39a46.8,46.8,0,0,1-4.46,19,68.05,68.05,0,0,1-12.52,18l-.11.11a4.48,4.48,0,0,1-6.23.08l-.11-.1a4.56,4.56,0,0,1-1.31-3,4.48,4.48,0,0,1,1.22-3.21,59.49,59.49,0,0,0,10.81-15.42,38.08,38.08,0,0,0,3.76-15.59,38.45,38.45,0,0,0-3.23-15.72A59,59,0,0,0,100.48,7.54Z"/>
      </svg>
    </span>
    <span class="scan-tag-title ml-3 text-xl font-bold ">Scan Tag</span>
  </h5>
</div>

        <p>Please place the card/tag on the scanner. (only admin cards/tags are eligible.)</p>
        <div class="flex justify-end space-x-2">
            <button id="cancelBtn" class="px-4 py-2 bg-primary text-primary-foreground rounded-md">Cancel</button>
        </div>
    </div>
</div>
<script>
    const tagModal = document.getElementById('tagModal');
    const loginViaTagButton = document.querySelector('button.bg-secondary');
    const cancelBtn = document.getElementById('cancelBtn');

    loginViaTagButton.onclick = function() {
        tagModal.classList.remove('hidden');
    }

    cancelBtn.onclick = function() {
        tagModal.classList.add('hidden');
    }

</script>

</body>
</html>
)rawliteral";

const char* SETUP::home = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuration Page</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css">
    <style>
        body {
            background-color: #000;
            color: #fff;
        }
        .tab {
            cursor: pointer;
            padding: 0.5rem 1rem;
            border-bottom: 2px solid transparent;
            margin-bottom: 0.5rem;
        }
        .tab.active {
            border-bottom: 2px solid #fff;
            color: #fff;
            font-weight: bold;
        }
        .tab.inactive {
            color: #aaa;
        }
        .card {
            background-color: #000;
            border: 1px solid #333;
            margin: 0 auto;
            padding: 1.5rem;
            width: 90%;
            max-width: 600px;
            border-radius: 0.5rem;
        }
        input, select, button {
            background-color: #333;
            color: #fff;
            border: 1px solid #555;
        }
        input:focus, select:focus, button:focus {
            outline: none;
            border-color: #fff;
        }
        .table-row.selected {
            background-color: #444;
        }
        .separator {
            border-top: 1px solid #555;
            margin: 1rem 0;
        }
        .logout-button {
            margin-top: 1rem;
            margin-bottom: 0.5rem;
            transition: background-color 0.3s;
        }
        .logout-button:hover {
            background-color: #c53030;
        }
    </style>
</head>
<body class="p-8">
    <div class="card shadow-lg">
        <div class="flex justify-between border-b mb-2">
            <div class="flex">
                <div class="tab active" onclick="showTab('basic')">Basic Configurations</div>
                <div class="tab inactive" onclick="showTab('rfid')">RFID Configurations</div>
            </div>
            <button class="bg-red-500 rounded-md px-4 py-2 logout-button flex items-center justify-center">
                <svg fill="#ffffff" height="25px" width="25px" version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" 
                    viewBox="0 0 330 330" xml:space="preserve">
                    <g id="XMLID_2_">
                        <path id="XMLID_4_" d="M51.213,180h173.785c8.284,0,15-6.716,15-15s-6.716-15-15-15H51.213l19.394-19.393
                            c5.858-5.857,5.858-15.355,0-21.213c-5.856-5.858-15.354-5.858-21.213,0L4.397,154.391c-0.348,0.347-0.676,0.71-0.988,1.09
                            c-0.076,0.093-0.141,0.193-0.215,0.288c-0.229,0.291-0.454,0.583-0.66,0.891c-0.06,0.09-0.109,0.185-0.168,0.276
                            c-0.206,0.322-0.408,0.647-0.59,0.986c-0.035,0.067-0.064,0.138-0.099,0.205c-0.189,0.367-0.371,0.739-0.53,1.123
                            c-0.02,0.047-0.034,0.097-0.053,0.145c-0.163,0.404-0.314,0.813-0.442,1.234c-0.017,0.053-0.026,0.108-0.041,0.162
                            c-0.121,0.413-0.232,0.83-0.317,1.257c-0.025,0.127-0.036,0.258-0.059,0.386c-0.062,0.354-0.124,0.708-0.159,1.069
                            C0.025,163.998,0,164.498,0,165s0.025,1.002,0.076,1.498c0.035,0.366,0.099,0.723,0.16,1.08c0.022,0.124,0.033,0.251,0.058,0.374
                            c0.086,0.431,0.196,0.852,0.318,1.269c0.015,0.049,0.024,0.101,0.039,0.15c0.129,0.423,0.28,0.836,0.445,1.244
                            c0.018,0.044,0.031,0.091,0.05,0.135c0.16,0.387,0.343,0.761,0.534,1.13c0.033,0.065,0.061,0.133,0.095,0.198
                            c0.184,0.341,0.387,0.669,0.596,0.994c0.056,0.088,0.104,0.181,0.162,0.267c0.207,0.309,0.434,0.603,0.662,0.895
                            c0.073,0.094,0.138,0.193,0.213,0.285c0.313,0.379,0.641,0.743,0.988,1.09l44.997,44.997C52.322,223.536,56.161,225,60,225
                            s7.678-1.464,10.606-4.394c5.858-5.858,5.858-15.355,0-21.213L51.213,180z"/>
                        <path id="XMLID_5_" d="M207.299,42.299c-40.944,0-79.038,20.312-101.903,54.333c-4.62,6.875-2.792,16.195,4.083,20.816
                            c6.876,4.62,16.195,2.794,20.817-4.083c17.281-25.715,46.067-41.067,77.003-41.067C258.414,72.299,300,113.884,300,165
                            s-41.586,92.701-92.701,92.701c-30.845,0-59.584-15.283-76.878-40.881c-4.639-6.865-13.961-8.669-20.827-4.032
                            c-6.864,4.638-8.67,13.962-4.032,20.826c22.881,33.868,60.913,54.087,101.737,54.087C274.956,287.701,330,232.658,330,165
                            S274.956,42.299,207.299,42.299z"/>
                    </g>
                </svg>
                <h1 class="font-semibold ml-2 text-lg text-center">Logout</h1>
            </button>
        </div>
        <div id="basic" class="tab-content">
            <div class="mb-4">
                <input type="text" placeholder="WiFi SSID" class="w-full h-10 rounded-md px-3 mb-3">
                <input type="password" placeholder="WiFi Password" class="w-full h-10 rounded-md px-3 mb-3">
                <input type="text" placeholder="User ID" class="w-full h-10 rounded-md px-3 mb-3">
                <input type="password" placeholder="User Password" class="w-full h-10 rounded-md px-3 mb-3">
                <button class="bg-blue-500 rounded-md px-4 py-2">Save</button>
            </div>
        </div>
        <div id="rfid" class="tab-content hidden">
            <table class="min-w-full border-collapse mb-4">
                <thead>
                    <tr class="bg-slate-700">
                        <th class="border border-gray-600 px-4 py-2 text-left">ID</th>
                        <th class="border border-gray-600 px-4 py-2 text-left">Name</th>
                        <th class="border border-gray-600 px-4 py-2 text-left">Role</th>
                    </tr>
                </thead>
                <tbody id="tagTable">
                    <tr class="table-row cursor-pointer" onclick="selectRow(this)">
                        <td class="border border-gray-600 px-4 py-2">1</td>
                        <td class="border border-gray-600 px-4 py-2">Dummy Tag 1</td>
                        <td class="border border-gray-600 px-4 py-2">Admin</td>
                    </tr>
                    <tr class="table-row cursor-pointer" onclick="selectRow(this)">
                        <td class="border border-gray-600 px-4 py-2">2</td>
                        <td class="border border-gray-600 px-4 py-2">Dummy Tag 2</td>
                        <td class="border border-gray-600 px-4 py-2">User</td>
                    </tr>
                    <tr class="table-row cursor-pointer" onclick="selectRow(this)">
                        <td class="border border-gray-600 px-4 py-2">3</td>
                        <td class="border border-gray-600 px-4 py-2">Dummy Tag 3</td>
                        <td class="border border-gray-600 px-4 py-2">Admin</td>
                    </tr>
                </tbody>
            </table>
            <div class="flex space-x-2 mb-4">
                <button id="deleteButton" class="bg-red-500 rounded-md px-4 py-2" onclick="deleteSelected()">Delete Selected</button>
            </div>
            <div class="separator"></div>
            <span id="tagLimitMessage" class="text-red-500 mb-2 hidden text-center block font-bold text-xl">3 tags already exist.</span>
            <div class="flex mb-4">
                <div class="tab active" onclick="showSubTab('addTags')">Add Tags</div>
                <div class="tab inactive" onclick="showSubTab('updateTags')">Update Tags</div>
            </div>
            <div id="addTags" class="sub-tab-content">
                <button id="scanButton" class="bg-blue-500 rounded-md px-4 py-2 mb-3" onclick="scanTag()">Scan Tag</button>
                <input type="text" placeholder="Tag ID" class="w-full h-10 rounded-md px-3 mb-3" id="addTagId" disabled>
                <input type="text" placeholder="Tag Name" class="w-full h-10 rounded-md px-3 mb-3" id="addTagName">
                <select class="w-full h-10 rounded-md px-3 mb-3" id="addTagRole">
                    <option value="">Select Role</option>
                    <option value="admin">Admin</option>
                    <option value="user">User</option>
                </select>
                <button id="addButton" class="bg-green-500 rounded-md px-4 py-2" onclick="saveTag()">Add</button>
            </div>
            <div id="updateTags" class="sub-tab-content hidden">
                <input type="text" placeholder="Tag ID" class="w-full h-10 rounded-md px-3 mb-3" id="updateTagId" disabled>
                <input type="text" placeholder="Tag Name" class="w-full h-10 rounded-md px-3 mb-3" id="updateTagName">
                <select class="w-full h-10 rounded-md px-3 mb-3" id="updateTagRole">
                    <option value="">Select Role</option>
                    <option value="admin">Admin</option>
                    <option value="user">User</option>
                </select>
            </div>
	                   <button id="updateButton" class="bg-green-500 rounded-md px-4 py-2 hidden" onclick="updateSelected()">Update Selected</button>
        </div>
    </div>

    <script>
        let selectedRow = null;

        function showTab(tabName) {
            document.querySelectorAll('.tab-content').forEach(tab => tab.classList.add('hidden'));
            document.getElementById(tabName).classList.remove('hidden');

            document.querySelectorAll('.tab').forEach(tab => {
                tab.classList.remove('active');
                tab.classList.add('inactive');
            });
            document.querySelector(`.tab[onclick="showTab('${tabName}')"]`).classList.add('active');
            document.querySelector(`.tab[onclick="showTab('${tabName}')"]`).classList.remove('inactive');
        }

        function showSubTab(subTabName) {
            document.querySelectorAll('.sub-tab-content').forEach(tab => tab.classList.add('hidden'));
            document.getElementById(subTabName).classList.remove('hidden');

            document.querySelectorAll('.tab').forEach(tab => {
                if (tab.getAttribute('onclick').includes(subTabName)) {
                    tab.classList.add('active');
                    tab.classList.remove('inactive');
                } else {
                    tab.classList.remove('active');
                    tab.classList.add('inactive');
                }
            });

            if (subTabName === 'updateTags') {
                document.getElementById('updateButton').classList.remove('hidden');
            } else {
                document.getElementById('updateButton').classList.add('hidden');
            }
        }

        function selectRow(row) {
            if (selectedRow === row) {
                selectedRow.classList.remove('selected');
                selectedRow = null;
                clearUpdateFields();
            } else {
                if (selectedRow) {
                    selectedRow.classList.remove('selected');
                }
                selectedRow = row;
                selectedRow.classList.add('selected');
                fillUpdateFields();
            }
        }

        function deleteSelected() {
            if (selectedRow) {
                selectedRow.remove();
                selectedRow = null;
                checkTagLimit();
                clearUpdateFields();
            }
        }

        function updateSelected() {
            if (selectedRow) {
                const id = selectedRow.cells[0].innerText;
                const name = document.getElementById('updateTagName').value;
                const role = document.getElementById('updateTagRole').value;

                if (name && role) {selectedRow.cells[1].innerText = name;
                    selectedRow.cells[2].innerText = role;
                    alert(`Tag Updated: ID: ${id}, Name: ${name}, Role: ${role}`);
                    clearUpdateFields();
                }
            }
        }

        function checkTagLimit() {
            const rows = document.querySelectorAll('#tagTable tr');
            const tagLimitMessage = document.getElementById('tagLimitMessage');
            const addTagsArea = document.getElementById('addTags');
            const scanButton = document.getElementById('scanButton');
            const addButton = document.getElementById('addButton');

            if (rows.length >= 3) {
                tagLimitMessage.classList.remove('hidden');
                addTagsArea.querySelectorAll('input:not([placeholder="Tag ID"]), select').forEach(element => {
                    element.disabled = true;
                });
                scanButton.disabled = true;
                addButton.disabled = true;
            } else {
                tagLimitMessage.classList.add('hidden');
                addTagsArea.querySelectorAll('input:not([placeholder="Tag ID"]), select').forEach(element => {
                    element.disabled = false;
                });
                scanButton.disabled = false;
                addButton.disabled = false;
            }
        }

        function scanTag() {
            alert("Scanning Tag...");
            // Simulating a scanned tag ID
            document.getElementById('addTagId').value = Math.floor(Math.random() * 1000000).toString();
        }

        function saveTag() {
            const id = document.getElementById('addTagId').value;
            const name = document.getElementById('addTagName').value;
            const role = document.getElementById('addTagRole').value;
            const rows = document.querySelectorAll('#tagTable tr');

            if (id && name && role) {
                const newRow = `<tr class="table-row cursor-pointer" onclick="selectRow(this)">
                                    <td class="border border-gray-600 px-4 py-2">${id}</td>
                                    <td class="border border-gray-600 px-4 py-2">${name}</td>
                                    <td class="border border-gray-600 px-4 py-2">${role}</td>
                                </tr>`;
                document.getElementById('tagTable').insertAdjacentHTML('beforeend', newRow);
                checkTagLimit();
                alert(`Tag Added: ID: ${id}, Name: ${name}, Role: ${role}`);
                clearAddFields();
            }
        }

        function fillUpdateFields() {
            if (selectedRow) {
                document.getElementById('updateTagId').value = selectedRow.cells[0].innerText;
                document.getElementById('updateTagName').value = selectedRow.cells[1].innerText;
                document.getElementById('updateTagRole').value = selectedRow.cells[2].innerText.toLowerCase();
            }
        }

        function clearUpdateFields() {
            document.getElementById('updateTagId').value = '';
            document.getElementById('updateTagName').value = '';
            document.getElementById('updateTagRole').value = '';
        }

        function clearAddFields() {
            document.getElementById('addTagId').value = '';
            document.getElementById('addTagName').value = '';
            document.getElementById('addTagRole').value = '';
        }

        function logout() {
            alert("Logged out!");
        }

        checkTagLimit();
    </script>
</body>
</html>
)rawliteral";
