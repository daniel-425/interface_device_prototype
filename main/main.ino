#include <Ethernet.h>
#include <EthernetUdp.h>

// Setup our MAC address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// Setup our own network information. Eventaully DHCP
IPAddress ip(192, 168 , 7, 100);
IPAddress gateway(192, 168, 7, 1);
IPAddress subnet(255, 255, 255, 0);

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

bool ready = false; 

int write_probe_port_1 = 1001;
int write_probe_port_2 = 1002;
int write_probe_port_3 = 1003;
int read_probe_port_1 = 2001;

const int READ_PIN_1 = 9;
const int WRITE_PIN_1 = 10;
const int WRITE_PIN_2 = 11;
const int WRITE_PIN_3 = 12;

const char * controller_ip = "192.168.7.5";

EthernetUDP udpserver1;
EthernetUDP udpserver2;
EthernetUDP udpserver3;

void setup() {
  // Configure the ethernet shield
  Ethernet.init(10);  // Most Arduino shields

  // Set our pins
  pinMode(READ_PIN_1, INPUT);
  pinMode(WRITE_PIN_1, OUTPUT);

  // Start the Ethernet
  Ethernet.begin(mac, ip);

  // Open Serial communications and wait for port to open:
  Serial.begin(9600);

  // For prototype only
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }

  udpserver1.begin(write_probe_port_1);
  udpserver2.begin(write_probe_port_2);
  udpserver3.begin(write_probe_port_3);

  Udp.begin(read_probe_port_1);

}

void loop() {
    // Check if we hace an ethernet link. 
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      ready = true;
    }
    
    // Send a reply to the IP address and port that sent us the packet we received
    if (true)  {
      // WRITE VOLTAGE 
      // Check if we need to write any voltage 
      int packetSize = udpserver1.parsePacket();

      if (packetSize) {
        Serial.print("Received udp server 1 packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = udpserver1.remoteIP();
        for (int i=0; i < 4; i++) {
          Serial.print(remote[i], DEC);
          if (i < 3) {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(udpserver1.remotePort());

        // read the packet into packetBufffer
        udpserver1.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents2:");
        Serial.println(packetBuffer);
        Serial.println("Length:");
        Serial.println(strlen(packetBuffer));

        // Parse the packet 
        if (packetBuffer[0] == '1'){
          digitalWrite(WRITE_PIN_1, HIGH);
        }
        else if (packetBuffer[0] == '0'){
          digitalWrite(WRITE_PIN_1, LOW);
        }
        else {
          Serial.println("ERROR: Unknown packet buffer");
        }
      }

      packetSize = udpserver2.parsePacket();

      if (packetSize) {
        Serial.print("Received udp server 2 packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = udpserver2.remoteIP();
        for (int i=0; i < 4; i++) {
          Serial.print(remote[i], DEC);
          if (i < 3) {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(udpserver2.remotePort());

        // read the packet into packetBufffer
        udpserver2.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents2:");
        Serial.println(packetBuffer);
        Serial.println("Length:");
        Serial.println(strlen(packetBuffer));

        // Parse the packet 
        if (packetBuffer[0] == '1'){
          digitalWrite(WRITE_PIN_2, HIGH);
        }
        else if (packetBuffer[0] == '0'){
          digitalWrite(WRITE_PIN_2, LOW);
        }
        else {
          Serial.println("ERROR: Unknown packet buffer");
        }
      }

      packetSize = udpserver3.parsePacket();

      if (packetSize) {
        Serial.print("Received udp server 3 packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = udpserver3.remoteIP();
        for (int i=0; i < 4; i++) {
          Serial.print(remote[i], DEC);
          if (i < 3) {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(udpserver3.remotePort());

        // read the packet into packetBufffer
        udpserver3.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents2:");
        Serial.println(packetBuffer);
        Serial.println("Length:");
        Serial.println(strlen(packetBuffer));

        // Parse the packet 
        if (packetBuffer[0] == '1'){
          digitalWrite(WRITE_PIN_3, HIGH);
        }
        else if (packetBuffer[0] == '0'){
          digitalWrite(WRITE_PIN_3, LOW);
        }
        else {
          Serial.println("ERROR: Unknown packet buffer");
        }
      }

      // READ VOLTAGE
      // Send our probe 1 value
      Serial.write("READ VALUE 1\n");
      int input_value = digitalRead(READ_PIN_1);
      Serial.print(input_value);
      Serial.write("\n");

      Serial.println("Sending packet");
      Udp.beginPacket(controller_ip, read_probe_port_1);
      Udp.write(input_value);
      Udp.endPacket();

    }
    else{
      Serial.println("Ethernet not connected");
    }
}
