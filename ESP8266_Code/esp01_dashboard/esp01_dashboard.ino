#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

// WiFi Access Point credentials
const char* ssid = "RobotMonitor";
const char* password = "12345678";

// Must match Arduino default speed
const int DEFAULT_SPEED = 75;

// Data received from Arduino
String distanceVal = "0";
String modeVal = "A";
String speedVal = "75";

/* ================= WEB PAGE ================= */
const char PAGE[] PROGMEM = R"====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Robot Dashboard</title>
<style>
body{background:black;color:white;font-family:Arial;text-align:center;}
.card{border:2px solid #444;border-radius:10px;padding:15px;margin:10px;}
button{width:180px;height:55px;font-size:20px;margin:10px;}
input[type=range]{width:80%;}
.green{color:lime;}
.red{color:red;}
</style>
</head>

<body>

<h2>Robot Control Dashboard</h2>

<div class="card">
  Mode: <b><span id="mode">AUTO</span></b>
</div>

<div class="card">
  Distance: <b><span id="dist" class="green">--</span> cm</b>
</div>

<div class="card">
  Speed: <b><span id="spd">--</span></b>
  <span id="diff"></span>
  <br><small>(Default: 75)</small>
</div>

<div class="card">
  <input id="slider" type="range" min="30" max="100" value="75"
         oninput="setSpeed(this.value)">
  <br><br>
  <button onclick="setDefault()">Set Default Speed</button>
</div>

<button onclick="sendCmd('S')" style="background:red;color:white;">STOP</button>
<br>
<button onclick="sendCmd('A')" style="background:green;color:white;">CONTINUE</button>

<script>
const DEFAULT_SPEED = 75;

function sendCmd(c){
  fetch('/cmd?c=' + c);
}

function setSpeed(v){
  fetch('/speed?v=' + v);
  updateSpeed(v);
}

function setDefault(){
  fetch('/speed?d=1');
  document.getElementById('slider').value = DEFAULT_SPEED;
  updateSpeed(DEFAULT_SPEED);
}

function updateSpeed(v){
  let diff = v - DEFAULT_SPEED;
  document.getElementById('spd').innerHTML = v;
  document.getElementById('diff').innerHTML =
    diff > 0 ? " (+" + diff + ")" : diff < 0 ? " (" + diff + ")" : "";
}

setInterval(()=>{
  fetch('/status?nocache=' + Date.now())
    .then(r => r.json())
    .then(s => {
      document.getElementById('mode').innerHTML =
        (s.mode === 'A') ? 'AUTO' : 'STOP';

      let d = document.getElementById('dist');
      d.innerHTML = s.distance;
      d.className = (s.distance <= 20) ? 'red' : 'green';

      document.getElementById('slider').value = s.speed;
      updateSpeed(s.speed);
    });
},300);
</script>

</body>
</html>
)====";

/* ================= HANDLERS ================= */

void handleRoot() {
  server.send_P(200, "text/html", PAGE);
}

void handleStatus() {
  String json = "{";
  json += "\"distance\":" + distanceVal + ",";
  json += "\"mode\":\"" + modeVal + "\",";
  json += "\"speed\":" + speedVal;
  json += "}";
  server.send(200, "application/json", json);
}

void handleCmd() {
  if (server.hasArg("c")) {
    Serial.print("@");
    Serial.print(server.arg("c")[0]);   // S or A
  }
  server.send(200, "text/plain", "OK");
}

void handleSpeed() {
  if (server.hasArg("v")) {
    Serial.print("@V");
    Serial.print(server.arg("v"));      // Set speed
  }
  if (server.hasArg("d")) {
    Serial.print("@VD");                // Default speed
  }
  server.send(200, "text/plain", "OK");
}

/* ================= SETUP ================= */

void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/cmd", handleCmd);
  server.on("/speed", handleSpeed);

  server.begin();
}

/* ================= LOOP ================= */

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    int d = line.indexOf("D:");
    int m = line.indexOf(",M:");
    int s = line.indexOf(",SPD:");

    if (d >= 0 && m >= 0 && s >= 0) {
      distanceVal = line.substring(d + 2, m);
      modeVal = line.substring(m + 3, s);
      speedVal = line.substring(s + 5);
    }
  }
  server.handleClient();
}
