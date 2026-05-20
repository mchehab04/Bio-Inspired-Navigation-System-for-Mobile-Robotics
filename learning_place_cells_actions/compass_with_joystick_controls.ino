#include <DynamixelWorkbench.h>
#include <QMC5883LCompass.h>

// Adjust for OpenCR or OpenCM
#if defined(__OPENCM904__)
  #define DEVICE_NAME "3"
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif  

// Baud rates
#define DXL_BAUDRATE  1000000
#define PC_BAUDRATE   57600

// Dynamixel IDs
#define DXL_ID_1  1
#define DXL_ID_2  2

#define SW1 34
#define SW2 35

// Serial protocol characters
const char INPUTS_SEND_CODE  = 'G';
const char INPUTS_CHECK_CODE = 'g';
const char OUTPUTS_SEND_CODE = 'E'; 
const char OUTPUTS_CHECK_CODE = 'e';

DynamixelWorkbench dxl;
QMC5883LCompass compass;

void establishContact()
{
  while (Serial.available() <= 0)
  {
    //Serial.println("A");   // send initial handshake
    delay(100);
  }
}

void setup()
{
  Serial.begin(PC_BAUDRATE);
  while (!Serial);

  // Initialize Dynamixel communication
  const char *log;
  bool result;
  uint16_t model_number = 0;

  result = dxl.init(DEVICE_NAME, DXL_BAUDRATE, &log);
  //if (!result)
  //{
    //Serial.println("Failed to init Dynamixel port!");
    //Serial.println(log);
    //return;
  //}

  result = dxl.ping(DXL_ID_1, &model_number, &log);
  //if (result)
    //Serial.println("MOTOR 1 DETECTED");
  //else
  //  Serial.println("NO MOTOR 1 DETECTED");
  
  result = dxl.ping(DXL_ID_2, &model_number, &log);
  //if (result)
   // Serial.println("MOTOR 2 DETECTED");
  //else
   // Serial.println("NO MOTOR 2 DETECTED");
  
  
  //Serial.println("Succeeded to init Dynamixel ports!");
  dxl.torqueOff(DXL_ID_1);
  dxl.torqueOff(DXL_ID_2);
  dxl.jointMode(DXL_ID_1, 0, 0);
  dxl.jointMode(DXL_ID_2, 0, 0);
  dxl.torqueOn(DXL_ID_1);
  dxl.torqueOn(DXL_ID_2);

  bool result1 = dxl.wheelMode(DXL_ID_1, 0, &log);
  bool result2 = dxl.wheelMode(DXL_ID_2, 0, &log);

  //if (result1 == false || result2 == false)
  //{
  //  Serial.println(log);
   // Serial.println("Failed to change wheel mode");
  //}
  //else
  //{
  //  Serial.println("Succeed to change wheel mode");
  //}

  // Handshake with Promethe
  compass.init();
  establishContact();
  while (Serial.available() <= 0)
  {
    Serial.println("E");
    delay(100);
  }
}

void loop() {
  if (Serial.available()) {
    static char line[64];
    int b1 = !digitalRead(SW1);  // active LOW → invert
    int b2 = !digitalRead(SW2);

    compass.read();
    float azimuth = compass.getAzimuth();
    // Shift
    azimuth = (azimuth + 180) / 360;

    size_t n = Serial.readBytesUntil('\r', line, sizeof(line)-1);
    line[n] = '\0'; // null terminate

    if (line[0] == 'E') {
      float v1 = 0, v2 = 0, v3 = 0;  // forward, left, right
      int parsed = sscanf(line, "E %f %f %f", &v1, &v2, &v3);

      if (parsed == 3) {
        int32_t leftVel = 0;
        int32_t rightVel = 0;

        // Forward
        if (v1>0) {
          leftVel = 220;
          rightVel = 220;
        }

        // Turn Left
        else if (v2>0) {
          leftVel = 100;
          rightVel = 250;
        }

        // Turn Right
        else if (v3>0) {
          leftVel = 250;
          rightVel = 100;
        }

        // Apply the command
        dxl.goalVelocity(DXL_ID_1, leftVel);
        dxl.goalVelocity(DXL_ID_2, rightVel);

        Serial.println(String(OUTPUTS_CHECK_CODE) + " 0.0 0.0"); // ACK success
      }
    }
    else if(line[0] == 'G'){
      Serial.print(String(INPUTS_CHECK_CODE) + " ");
      Serial.print(b1); Serial.print(" ");
      Serial.print(b2); Serial.print(" ");
      Serial.println(azimuth);
    }
    else if (line[0] == 'I')
    {
      if (line[1] == 'G')
        Serial.println("ig 0.0 0.0 0");
      else if (line[1] == 'E')
        Serial.println("ie 0.0 0.0 0");

      return;
    }
  }
    delay(2);
}