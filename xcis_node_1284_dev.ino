#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <SPI.h>
#include <XcisMessage.h>

uint8_t DEVICE_TYPE = FLOW_METER;

#define DATAOUT 5//MOSI PB5
#define DATAIN  6//MISO PB6
#define SPICLOCK  7//sck PB7
#define SLAVESELECT 10//PD2
//#define HOLD 9 // Not used

//opcodes
#define WREN  0x06 // write enable command
#define WRDI  0x04 // write disable
#define RDSR  0x05 // read the status register
#define WRSR  0x01 // write the status register 
#define READ  0x03 // read data
#define WRITE 0x02 // write data

byte inBuffer[9]; // eeprom data in

String node_id_default = "04"; // Default Lora ID
unsigned int node_id_mesh_default = 4; // Default Lora ID
#define RH_HAVE_SERIAL

#ifdef ARDUINO_AVR_UNO
String board = "ARDUINO_AVR_UNO";
#define RFM95_CS 10
#define RFM95_RST 5
#define RFM95_INT  2
#elif ARDUINO_AVR_MEGA2560
String board = "ARDUINO_AVR_MEGA2560";
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#elif __AVR_ATmega1284P__
String board = "__AVR_ATmega1284P__";
#define RFM95_CS 4
#define RFM95_RST 1
#define RFM95_INT 2
#else
String board = "XCIS Pro or Pro mini";
#define RFM95_CS 4
#define RFM95_RST 5
#define RFM95_INT  2
#endif

RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHMesh *manager;

XcisMessage xcisMessage;
uint8_t data[32];

// Analog PIN for Battery input
#define BATTERY_VOLTAGE A0
// Analog PIN for Reset switch
#define RESET_SW A3
// Analog PIN for Sensor On Indicator
//#define SENSOR_ON 0 
// Digital PIN for LORA Active
#define LORA 0 // Green LED on dev board
// Digital PIN for Pulse Inpit
#define PULSE 10

uint8_t pulseCount = 0;
uint8_t accumulatedPulses = 0;
bool pulseFlag = false;
String pulseFlag_s = "0";
uint32_t accumulatedDataToken;
int inputState = 0;
int lastInputState = 0;
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = 0; // true if still waiting for delay to finish
unsigned long delayStartPulse = 0;
unsigned long delayRainCheck = 0;
int defaultSwitchPress = 0;
unsigned int address = 0; // EEPROM Address
byte value = 0; // Initialsed Byte

void setup()
{
   Serial.begin(115200);
   randomSeed(analogRead(0)); // Seed the random number generator

   initUID();
   read_manufacturer();
   read_device_code();
   read_Status_Reg();
   read_ID_new();
   
   
   pinMode(RESET_SW,INPUT); // A3 Reset Switch
   //pinMode(PULSE,INPUT); // Pulse input
  
   //pinMode(SENSOR_ON,OUTPUT);// A1, PC11 output LOW = ON SENSOR
   //digitalWrite(SENSOR_ON,1); 
   
   pinMode(LORA,OUTPUT); 
   digitalWrite(LORA,0);// Green LED off
    
   pinMode(RFM95_RST, OUTPUT); // LORA RESET
   digitalWrite(RFM95_RST, 1);
    // manual reset
    digitalWrite(RFM95_RST, 0);
    delay(10);
    digitalWrite(RFM95_RST, 1);
    delay(10);

    setLoraDefault();
    //setLoraInitialised();
    
    if (checkLoraInitialised() == false)
    {
      Serial.println("Lora ID set to default");
      manager = new RHMesh(rf95, node_id_mesh_default); // Node ID - NEED TO SET THIS
    }
    else if (checkLoraInitialised() == true)
    {
      Serial.println("Lora ID set to chip ID");
      // Need to get the CHIP ID since the gateway knows this node by the CHIP ID
      //manager = new RHMesh(rf95, getCHIPID()); // Node ID - NEED TO SET THIS
      manager = new RHMesh(rf95, getChipID()); // Node ID - NEED TO SET THIS
    }  
    if (!manager->init()) {
      Serial.println(F("Radio init failed"));
    } else {
    Serial.println("Radio initialised");
    }
    Serial.print("Radio device version:");
    Serial.println(rf95.getDeviceVersion());
    //Serial.print("Registers:");
    //Serial.println(rf95.printRegisters());
    
    delayStart = millis();   // start delay
    delayStartPulse = millis();   // start delay
    delayRainCheck = millis();
    delayRunning = true; // not finished yet
  
    Serial.print(F("Ready"));
    Serial.print(" Board:");
    Serial.println(board);
    //Serial.print("LoraID:");
    //Serial.println(getChipID_string());
    //digitalWrite(SENSOR_ON,1); 
   digitalWrite(LORA,0);// Green LED OFF
}

void loop()
{
  /* 
   inputState = digitalRead(PULSE);
   if (inputState != lastInputState)
   {
     if (inputState == 1)
     {
       Serial.println("Got pulse");
       countPulses();
     }
   }
   lastInputState = inputState;
   */
   
  // check if delay has timed out after 10sec == DELAY_TIME
  if (delayRunning && ((millis() - delayStartPulse) >= 90000))// 15 mins - normal value add a zero back for normal ops
  {
    //delayStartPulse += 900000; // 15 mins - normal value
    delayStartPulse += 90000; // 15 mins - normal value - add a zero back for normal ops
    storePulseCount();
  }

  // check if delay has timed out after 10sec == DELAY_TIME
  if (delayRunning && ((millis() - delayRainCheck) >= 60000))//unsigned long DELAY_TIME = 06 s
  {
    delayRainCheck += 60000; // 60 secs
    checkRaining();
    //read_ID_new();
  }
  
  // parse for a packet, and call onReceive with the result:
  if (onReceive())
  {
    String message = "Got Message!";   // send a message
  }

}
