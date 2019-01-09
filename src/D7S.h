#include <Arduino.h>
#include <Wire.h>

#define WireD7S Wire

// D7S address on the I2C bus.
#define D7S_ADDRESS 0x55

//d7s state
typedef enum d7s_status
{
   NORMAL_MODE = 0x00,
   NORMAL_MODE_NOT_IN_STANBY = 0x01,
   INITIAL_INSTALLATION_MODE = 0x02,
   OFFSET_ACQUISITION_MODE = 0x03,
   SELFTEST_MODE = 0x04
};

//d7s axis settings
typedef enum d7s_axis_settings
{
   FORCE_YZ = 0x00,
   FORCE_XZ = 0x01,
   FORXE_XY = 0x02,
   AUTO_SWITCH = 0x03,
   SWITCH_AT_INSTALLATION = 0x04
};

//axis state
typedef enum d7s_axis_state
{
   AXIS_YZ = 0x00,
   AXIS_XZ = 0x01,
   AXIS_XY = 0x02
};

//d7s threshold settings
typedef enum d7s_threshold
{
   THRESHOLD_HIGH = 0x00,
   THRESHOLD_LOW = 0x01
};

//class D7S
class D7SClass
{

 public:
   //constructor
   D7SClass();

   //used to initialize Wire
   void begin();

   //return the currect state
   d7s_status getState();

   //change the axis selection mode
   void setAxis(d7s_axis_settings axisMode);

   //get instantaneus SI (during an earthquake) [m/s]
   float getInstantaneusSI();
   //get instantaneus PGA (during an earthquake) [m/s^2]
   float getInstantaneusPGA();

   //initialize the d7s (start the initial installation mode)
   void initialize();

   //return true if the collapse condition is met (it's the sencond bit of _events)
   uint8_t isInCollapse();
   //return true if the shutoff condition is met (it's the first bit of _events)
   uint8_t isInShutoff();
   //reset shutoff/collapse events
   void resetEvents();

   //return true if an earthquake is occuring
   uint8_t isEarthquakeOccuring();

   //is rady state
   uint8_t isReady();

 private:
   //handler array (it cointaint the pointer to the user defined array)
   void (*_handlers[4])();

   //variable to track event (first bit => SHUTOFF, second bit => COLLAPSE)
   uint8_t _events;

   //enable interrupt handling
   uint8_t _interruptEnabled;

   //read 8 bit from the specified register
   uint8_t read8bit(uint8_t regH, uint8_t regL);
   //read 16 bit from the specified register
   uint16_t read16bit(uint8_t regH, uint8_t regL); 

   //write 8 bit to the register specified
   void write8bit(uint8_t regH, uint8_t regL, uint8_t val); 

   //read the event (SHUTOFF/COLLAPSE) from the EVENT register
   void readEvents(); 
};

extern D7SClass D7S;
