/*
mkdir express-mqtt-app
cd express-mqtt-app
npm init -y
npm install express mqtt

*/


const express = require('express');
const mqtt = require('mqtt');
const path = require('path');

// Create an Express application
const app = express();
const port = 2800;

// Variable to store the latest sensor data
let latestSensorData = 'data';

// Connect to the MQTT broker
//const mqttClient = mqtt.connect('mqtt://127.0.0.1:1883');
const mqttClient = mqtt.connect('mqtt://broker.emqx.io:1883');

mqttClient.on('connect', () => {
  console.log('Connected to MQTT broker');
  
  // Subscribe to the "sensor" topic
  mqttClient.subscribe('potensiometer_husni', (err) => {
    if (err) {
      console.error('Failed to subscribe to topic "sensor":', err);
    } else {
      console.log('Subscribed to topic "sensor"');
    }
  });
  
});

// Handle incoming messages from the "sensor" topic
mqttClient.on('message', (topic, message) => {
  if (topic === 'potensiometer_husni') {
    console.log(`Received message from ${topic}: ${message.toString()}`);
    // Store the received data
    latestSensorData = message.toString();
	console.log(latestSensorData);
  }
});

mqttClient.on('error', (err) => {
  console.error('MQTT connection error:', err);
});

// Middleware to serve static files
app.use(express.static('public'));

// Middleware to parse JSON bodies
app.use(express.json());

// Route to publish a message to an MQTT topic
app.post('/publish', (req, res) => {
  const { topic, message } = req.body;
  console.log(`Received request to publish topic: ${topic}, message: ${message}`);

  mqttClient.publish(topic, message, (err) => {
    if (err) {
      console.error('Error publishing message:', err);
      res.status(500).send('Error publishing message');
    } else {
      console.log('Message published successfully');
      res.send('Message published successfully');
    }
  });
});

// Route to get the latest sensor data
app.get('/sensor-data', (req, res) => {
  res.json({ data: latestSensorData });
});


// Serve the HTML file
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Start the Express server
app.listen(port, () => {
  console.log(`Express server listening at http://localhost:${port}`);
});