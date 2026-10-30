#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <hp_BH1750.h>
#include <WebServer.h>

Adafruit_BME280 bme;
hp_BH1750 lightSensor;

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
WebServer server(80);

float currentTemperature = 0;
float currentHumidity = 0;
float currentPressure = 0;
float currentLight = 0;

void handleRoot() {
  lightSensor.start();

  float temperatureC = bme.readTemperature();

  currentTemperature = temperatureC * 9.0 / 5.0 + 32.0;
  currentHumidity = bme.readHumidity();
  currentPressure = bme.readPressure() / 100.0F;
  currentLight = lightSensor.getLux();

  String html = "<!DOCTYPE html><html>";
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";

  html += "<title>Microclimate Compare</title>";
  html += "<style>";

  html += "body {";
  html += "font-family: Times New Roman;";
  html += "background: #d1dfd7;";
  html += "color: #003817;";
  html += "max-width: 850px;";
  html += "margin: auto;";
  html += "padding: 25px;";
  html += "}";

  html += "h1 {";
  html += "color: #255538;";
  html += "font-size: 42px;";
  html += "margin-bottom: 8px;";
  html += "}";

  html += "h2 {";
  html += "color: #336b4a;";
  html += "margin-top: 30px;";
  html += "}";

  html += "p {";
  html += "font-size: 18px;";
  html += "}";

  html += "input, select, button {";
  html += "font-size: 16px;";
  html += "padding: 10px;";
  html += "margin: 4px;";
  html += "border-radius: 7px;";
  html += "border: 1px solid #9aac9b;";
  html += "}";

  html += "button {";
  html += "background: #1b5733;";
  html += "color: white;";
  html += "border: none;";
  html += "cursor: pointer;";
  html += "}";

  html += "button:hover {";
  html += "background: #0f3920;";
  html += "}";

  html += "hr {";
  html += "border: none;";
  html += "border-top: 1px solid #b8c7b7;";
  html += "margin-top: 28px;";
  html += "}";

  html += "#history, #comparison {";
  html += "background: white;";
  html += "padding: 15px;";
  html += "border-radius: 12px;";
  html += "margin-top: 15px;";
  html += "box-shadow: 0 2px 8px rgba(0,0,0,0.08);";
  html += "}";

  html += "</style>";
  html += "</head>";

  html += "<body>";
  html += "<h1>Microclimate Compare</h1>";

  html += "<h2>Live Conditions</h2>";



  html += "<p>Temperature: <span id='temperature'>";
  html += String(currentTemperature, 1);
  html += "</span> F</p>";

  html += "<p>Humidity: <span id='humidity'>";
  html += String(currentHumidity, 1);
  html += "</span> %</p>";

  html += "<p>Pressure: <span id='pressure'>";
  html += String(currentPressure, 1);
  html += "</span> hPa</p>";

  html += "<p>Light: <span id='light'>";
  html += String(currentLight, 1);
  html += "</span> lux</p>";



  html += "<hr>";
  html += "<h2>Record Snapshot</h2>";

  html += "<input id='label' type='text' placeholder='Environment name'>";
  html += "<button onclick='recordSnapshot()'>Record Snapshot</button>";

  html += "<p id='message'></p>";

  html += "<hr>";
  html += "<h2>Saved Snapshots</h2>";
  html += "<div id='history'></div>";

  html += "<hr>";
  html += "<h2>Compare Microclimates</h2>";

  html += "<select id='location1'></select>";
  html += "<span> vs. </span>";
  html += "<select id='location2'></select>";

  html += "<button onclick='compareSnapshots()'>Compare</button>";

  html += "<div id='comparison'></div>";

  html += "<script>";

  html += "function recordSnapshot() {";

  html += "const label = document.getElementById('label').value;";

  html += "if (label.trim() === '') {";
  html += "document.getElementById('message').innerText = 'Enter an environment name first.';";
  html += "return;";
  html += "}";

  html += "const snapshot = {";
  html += "label: label,";
  html += "temperature: " + String(currentTemperature, 1) + ",";
  html += "humidity: " + String(currentHumidity, 1) + ",";
  html += "pressure: " + String(currentPressure, 1) + ",";
  html += "light: " + String(currentLight, 1);
  html += "};";

  html += "let snapshots = JSON.parse(localStorage.getItem('microclimateSnapshots')) || [];";
  html += "snapshots.push(snapshot);";
  html += "localStorage.setItem('microclimateSnapshots', JSON.stringify(snapshots));";
  html += "displaySnapshots();";
  html += "document.getElementById('message').innerText = 'Snapshot recorded: ' + label;";

  html += "}";
  html += "function displaySnapshots() {";

  html += "const snapshots = JSON.parse(localStorage.getItem('microclimateSnapshots')) || [];";

  html += "const history = document.getElementById('history');";

  html += "if (snapshots.length === 0) {";
  html += "history.innerHTML = '<p>No snapshots recorded yet.</p>';";
  html += "return;";
  html += "}";

  html += "let output = '';";

  html += "snapshots.forEach((snapshot, index) => {";
  html += "output += '<h3>' + (index + 1) + '. ' + snapshot.label + '</h3>';";
  html += "output += '<p>Temperature: ' + snapshot.temperature + ' F</p>';";
  html += "output += '<p>Humidity: ' + snapshot.humidity + ' %</p>';";
  html += "output += '<p>Pressure: ' + snapshot.pressure + ' hPa</p>';";
  html += "output += '<p>Light: ' + snapshot.light + ' lux</p>';";
  html += "});";

  html += "history.innerHTML = output;";

  html += "const location1 = document.getElementById('location1');";
  html += "const location2 = document.getElementById('location2');";

  html += "location1.innerHTML = '';";
  html += "location2.innerHTML = '';";

  html += "snapshots.forEach((snapshot, index) => {";
  html += "location1.innerHTML += '<option value=\"' + index + '\">' + snapshot.label + '</option>';";
  html += "location2.innerHTML += '<option value=\"' + index + '\">' + snapshot.label + '</option>';";
  html += "});";

  html += "if (snapshots.length > 1) {";
  html += "location2.selectedIndex = 1;";
  html += "}";

  html += "}";
  html += "displaySnapshots();";

  html += "function compareSnapshots() {";

  html += "const snapshots = JSON.parse(localStorage.getItem('microclimateSnapshots')) || [];";

  html += "const index1 = document.getElementById('location1').value;";
  html += "const index2 = document.getElementById('location2').value;";

  html += "if (snapshots.length < 2) {";
  html += "document.getElementById('comparison').innerHTML = '<p>Record at least two snapshots first.</p>';";
  html += "return;";
  html += "}";

  html += "const a = snapshots[index1];";
  html += "const b = snapshots[index2];";

  html += "const tempDiff = (b.temperature - a.temperature).toFixed(1);";
  html += "const humidityDiff = (b.humidity - a.humidity).toFixed(1);";
  html += "const pressureDiff = (b.pressure - a.pressure).toFixed(1);";
  html += "const lightDiff = (b.light - a.light).toFixed(1);";

  html += "let output = '<h3>' + a.label + ' vs. ' + b.label + '</h3>';";

  html += "output += '<p>Temperature difference: ' + tempDiff + ' F</p>';";
  html += "output += '<p>Humidity difference: ' + humidityDiff + ' percentage points</p>';";
  html += "output += '<p>Pressure difference: ' + pressureDiff + ' hPa</p>';";
  html += "output += '<p>Light difference: ' + lightDiff + ' lux</p>';";

  html += "document.getElementById('comparison').innerHTML = output;";

  html += "}";

  html += "function updateLiveData() {";
  html += "fetch('/data')";
  html += ".then(response => response.json())";
  html += ".then(data => {";
  html += "document.getElementById('temperature').innerText = data.temperature;";
  html += "document.getElementById('humidity').innerText = data.humidity;";
  html += "document.getElementById('pressure').innerText = data.pressure;";
  html += "document.getElementById('light').innerText = data.light;";
  html += "});";
  html += "}";

  html += "setInterval(updateLiveData, 1000);";

  html += "</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);

  
}

void handleData() {
  lightSensor.start();

  float temperatureC = bme.readTemperature();

  currentTemperature = temperatureC * 9.0 / 5.0 + 32.0;
  currentHumidity = bme.readHumidity();
  currentPressure = bme.readPressure() / 100.0F;
  currentLight = lightSensor.getLux();

  String json = "{";
  json += "\"temperature\":" + String(currentTemperature, 1) + ",";
  json += "\"humidity\":" + String(currentHumidity, 1) + ",";
  json += "\"pressure\":" + String(currentPressure, 1) + ",";
  json += "\"light\":" + String(currentLight, 1);
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!bme.begin(0x77)) {
    Serial.println("BME280 not found!");
    while (true) {}
  }

  if (!lightSensor.begin(BH1750_TO_GROUND)) {
    Serial.println("BH1750 not found!");
    while (true) {}
  }

  Serial.println("Microclimate Detective started!");

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();

  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}