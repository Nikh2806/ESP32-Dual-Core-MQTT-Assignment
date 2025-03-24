#include <WiFi.h>
#include <PubSubClient.h>
#include <freertos/queue.h>

// Task handles for managing FreeRTOS tasks
TaskHandle_t WiFiAndMQTTTaskHandle;
TaskHandle_t SerialProcessingTaskHandle;

// GPIO pin for the onboard LED
#define LED_PIN 2

// WiFi and MQTT configurations
const char* WIFI_SSID = "Galaxy M205669";  // WiFi SSID
const char* WIFI_PASSWORD = "1234567788";  // WiFi password
const char* MQTT_SERVER = "test.mosquitto.org";  // MQTT broker address
const char* MQTT_TOPIC = "esp32/serialData";  // MQTT topic for publishing data

// Queue to store data received from UART
QueueHandle_t uartDataQueue;

// WiFi and MQTT clients
WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);

// Function to connect to WiFi
void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  Serial.println("Connected to WiFi.");
}

// Function to connect to the MQTT broker
void connectToMQTT() {
  mqttClient.setServer(MQTT_SERVER, 1883);
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("ESP32Clienttest2")) {
      Serial.println("Connected to MQTT broker.");
      digitalWrite(LED_PIN, HIGH);
    } else {
      Serial.print("Failed to connect to MQTT. State: ");
      Serial.println(mqttClient.state());
      digitalWrite(LED_PIN, LOW);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }
}

// Task to manage WiFi and MQTT operations (runs on core 0)
void WiFiAndMQTTTaskCore0(void* parameters) {
  connectToWiFi();

  while (true) {
    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(LED_PIN, HIGH);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      digitalWrite(LED_PIN, LOW);
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      connectToMQTT();
      Serial.println("MQTT connected");

      while (true) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        char receivedData[100];
        if (xQueueReceive(uartDataQueue, &receivedData, 0)) {
          mqttClient.publish(MQTT_TOPIC, String(receivedData).c_str(), true);
          Serial.println("Publishing data");
          mqttClient.loop();
          vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    } else {
      connectToWiFi();
    }
  }
}

// Task to process serial input data (runs on core 1)
void SerialProcessingTaskCore1(void* parameters) {
  Serial.begin(9600);
  Serial.println("UART Initialized");
  delay(1000);

  while (true) {
    if (Serial.available()) {
      String serialData = Serial.readStringUntil('\n');
      serialData.trim();

      if (serialData.length() > 0) {
        Serial.print("Received: ");
        Serial.println(serialData);

        // Send the data to the queue for further processing
        xQueueSend(uartDataQueue, serialData.c_str(), portMAX_DELAY);
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Configure LED pin as output

  // Create a queue to store serial data
  uartDataQueue = xQueueCreate(10, sizeof(char) * 100);

  // Create task to handle serial input (core 1)
  xTaskCreatePinnedToCore(
    SerialProcessingTaskCore1,
    "SerialProcessingTask",
    4096,
    NULL,
    1,
    &SerialProcessingTaskHandle,
    1
  );
  vTaskDelay(100 / portTICK_PERIOD_MS);

  // Create task to handle WiFi and MQTT operations (core 0)
  xTaskCreatePinnedToCore(
    WiFiAndMQTTTaskCore0,
    "WiFiAndMQTTTask",
    4096,
    NULL,
    1,
    &WiFiAndMQTTTaskHandle,
    0
  );
  vTaskDelay(100 / portTICK_PERIOD_MS);
}

void loop() {
  // Main loop intentionally left empty, put your main code here, to run repeatedly
}
