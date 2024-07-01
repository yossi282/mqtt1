#include <WiFi.h>
#include <PubSubClient.h>



// Replace the next variables with your SSID/Password combination
const char* ssid = "Syergie Indo Prima";
const char* password = "cahayaharapanhati";
const char* mqtt_server ="broker.emqx.io";//"mqtt.ardumeka.com";;
int mqtt_port = 1883;// 11219; 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


// LED Pin
int led1_pin = 5;

int analog;


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(led1_pin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  
 
 

  if (String(topic) == "button_husni") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("ON");
      digitalWrite(led1_pin, HIGH);

    }
    else if(messageTemp == "off"){
      Serial.println("OFF");
      digitalWrite(led1_pin, LOW);

    }
  }

    


}



void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("kirikIoT")) {
      Serial.println("connected");

      client.subscribe("button_husni");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();



  long now = millis();
  if (now - lastMsg > 500) {
    lastMsg = now;
    
    //kirim data 
    analog = analogRead(35);   
    char analog_send[8];
    dtostrf(analog, 1, 2, analog_send);
    
    client.publish("potensiometer_husni", analog_send);
    

  }
}
