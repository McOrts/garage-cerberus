// TTN Configuration
// LoRaWAN NwkSKey, network session key provided by TTN Console (https://console.thethingsnetwork.org) in Device settings form:
static const PROGMEM u1_t NWKSKEY[16] = {0x8F,0xDA,0xD7,0xD3,0xEF,0x2B,0xB9,0xCD,0x2C,0x43,0x8C,0x03,0x8B,0x91,0x53,0xBA};
// LoRaWAN AppSKey, application session key provided by TTN Console (https://console.thethingsnetwork.org) in Device settings form:
static const u1_t PROGMEM APPSKEY[16] = {0xE5,0x0A,0x7F,0xE0,0xA0,0xF8,0xA8,0x38,0xF6,0x37,0x6F,0xB8,0xBC,0x54,0xE8,0x2A};
// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x260115D2 ; // <-- Change this address for every node!

// Other params
const int update_time_alive = 150000;
const int PhotoCell = 2; 
const int Buzzer = 15;
