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
int read_probe_port_1 = 2001;

const int READ_PIN_1 = 9;
const int WRITE_PIN_1 = 10;

const char * controller_ip = "192.168.7.5";

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

  // Start UDP listening port
  Udp.begin(write_probe_port_1);
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
      int packetSize = Udp.parsePacket();

      if (packetSize) {
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = Udp.remoteIP();
        for (int i=0; i < 4; i++) {
          Serial.print(remote[i], DEC);
          if (i < 3) {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(Udp.remotePort());

        // read the packet into packetBufffer
        Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        Serial.println(packetBuffer);

        // Parse the packet 
        if (packetBuffer == "1"){
          digitalWrite(WRITE_PIN_1, HIGH);
        }
        else if (packetBuffer == "0"){
          digitalWrite(WRITE_PIN_1, LOW);
        }
        else {
          Serial.println("ERROR: Unknown packet buffer");
        }
      }

      // READ VOLTAGE
      // Send our probe 1 value
      Serial.write("READ VALUE\n");
      int input_value = digitalRead(READ_PIN_1);
      Serial.print(input_value);
      Serial.write("\n");

      // Translate the voltage to binary. 
      int parsed_value = 0;
      if (input_value > 1){
        parsed_value = 1;
      }

      Serial.println("Sending packet");
      Udp.beginPacket(controller_ip, read_probe_port_1);
      Udp.write(parsed_value);
      Udp.endPacket();

    }
    else{
      Serial.println("Ethernet not connected");
    }

    // How long should we delay?
    delay(10);
}