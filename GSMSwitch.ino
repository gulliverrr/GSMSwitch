int SMS_location_number;
const unsigned int MAX_INPUT = 165; // 160 characters for SMS plus a few extra 
static unsigned int input_pos = 0;

char MyMaster[ ] = "+441234567890";
const int RESTART_DELAY = 10000; // stay off for 10sec when restarting
const int MISSED_CALL_DELAY = 10000; // hang up after MISSED_CALL_DELAY seconds

const boolean RELAY_1_DEFAULT = LOW; // set relay level to this state on startup or after the restart. LOW means relay contacts are closed
const boolean RELAY_2_DEFAULT = LOW; // set relay level to this state on startup or after the restart. LOW means relay contacts are closed
const boolean RELAY_3_DEFAULT = LOW; // set relay level to this state on startup or after the restart. LOW means relay contacts are closed
const boolean RELAY_4_DEFAULT = LOW; // set relay level to this state on startup or after the restart. LOW means relay contacts are closed
const boolean TEST_DEFAULT = HIGH; // set relay level to this state on startup or after the restart. LOW means relay contacts are closed

const unsigned int IGT = 2;
const unsigned int RELAY_1 = 3;
const unsigned int RELAY_2 = 4;
const unsigned int RELAY_3 = 5;
const unsigned int RELAY_4 = 6;
const unsigned int TEST = 13;

void setup() 
{
  pinMode(IGT, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(TEST, OUTPUT);

  initialise();
  
  delay(5000); // Wait to GSM board to start  
  Serial1.begin(9600); // Starts addtional serial port for GSM
  while (!Serial1);

  Serial.begin(9600);
  //  while (!Serial);

  delay(1000);
  
  GSM_Startup();
  Serial.println("System ready");
  delay(1000);
}


void loop()   
{
  readTC35i();
}


void readTC35i()
{
  static char input_line [MAX_INPUT];
  if (Serial1.available () > 0)
  {
    while (Serial1.available () > 0)
    {
      char inByte = Serial1.read ();
      switch (inByte)
      {
        case '\n': // end of text
          input_line [input_pos] = 0; // terminating null byte
        
          // terminator reached! process input_line now
          process_data (input_line);
        
          // reset buffer for next time
          input_pos = 0;  
          break;
        
        case '\r': // discard carriage return
          break;
        
        default:
          // keep adding if not full... allow for terminating null byte
          if (input_pos < (MAX_INPUT - 1))
            input_line [input_pos++] = inByte;
          break;
      }
    }
  }
}


void process_data (char * data)
{
  Serial.println(data); // display the data

  if(strstr(data, "+CMGR:") && strstr(data, "+44XXXXXXXXXX"))
  {  
    // Only phone number +44XXXXXXXXXX can do whats inside this if
  }

  if(strstr(data, "smsdelete"))
  {
    delete_All_SMS();
  }

  if(strstr(data, "^SMGO: 2"))
  { // SIM card FULL
    delete_All_SMS(); // delete all SMS
  }

  if(strstr(data, "+CMTI:"))
  { // An SMS has arrived
    char* copy = data + 12; // Read from position 12 until a non ASCII number to get the SMS location
    SMS_location_number = (byte) atoi(copy); // Convert the ASCII number to an int
    Serial1.print("AT+CMGR=");
    Serial1.println(SMS_location_number);
  }

  if(strstr(strlwr(data), "ring"))
  {
    // Incoming call, answer it
    Serial.println("Incoming call. Answering it in 1 sec...");
    delay(1000);
    Serial1.println("ATA");
    Serial.println("Incoming call, answered. Hanging up in 5 sec...");
    delay(5000);
    Serial1.println("ATH");
    Serial.println("Incoming call, answered and hung up.");
  }
  
  if(strstr(strlwr(data), "test on"))
  {
    TEST_ON();
    delete_one_SMS();
    CallMyMaster();
  }

  if(strstr(strlwr(data), "test off"))
  {
    TEST_OFF();
    delete_one_SMS();
    CallMyMaster();
  }

  if(strstr(strlwr(data), "test restart"))
  {
    TEST_RESTART();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "1 on"))
  {
    RELAY_1_ON();
    delete_one_SMS();
    CallMyMaster();
  }

  if(strstr(strlwr(data), "1 off"))
  {
    RELAY_1_OFF();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "1 restart"))
  {
    RELAY_1_RESTART();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "2 on"))
  {
    RELAY_2_ON();
    delete_one_SMS();
    CallMyMaster();
  }

  if(strstr(strlwr(data), "2 off"))
  {
    RELAY_2_OFF();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "2 restart"))
  {
    RELAY_2_RESTART();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "3 on"))
  {
    RELAY_3_ON();
    delete_one_SMS();
    CallMyMaster();
  }

  if(strstr(strlwr(data), "3 off"))
  {
    RELAY_3_OFF();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "3 restart"))
  {
    RELAY_3_RESTART();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "4 on"))
  {
    RELAY_4_ON();
    delete_one_SMS();
    CallMyMaster();
  }

  if(strstr(strlwr(data), "4 off"))
  {
    RELAY_4_OFF();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "4 restart"))
  {
    RELAY_4_RESTART();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "all on"))
  {
    ALL_ON();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "all off"))
  {
    ALL_OFF();
    delete_one_SMS();
    CallMyMaster();
  }
  
  if(strstr(strlwr(data), "all restart"))
  {
    ALL_RESTART();
    delete_one_SMS();
    CallMyMaster();
  }
}

void delete_one_SMS()
{
  Serial.println("Deleting SMS 1");
  Serial1.print("AT+CMGD=");
  Serial1.println("1");
  delay(1000);
}

void delete_All_SMS()
{
  for(int i = 1; i <= 10; i++) {
    Serial1.print("AT+CMGD=");
    Serial1.println(i);
    Serial.write("Deleting SMS ");
    Serial.println(i);
    delay(500);
  }
}

void GSM_Startup()
{
  digitalWrite(IGT, LOW);
  delay(100);
  digitalWrite(IGT, HIGH);

  Serial1.print("AT+CMGF=1\r");
  delay(100);
  Serial1.print("AT+CNMI=2,1,0,0,1\r");
  delay(100);  
  delete_All_SMS();
}

void initialise()
{
  digitalWrite(RELAY_1, RELAY_1_DEFAULT);
  digitalWrite(RELAY_2, RELAY_2_DEFAULT);
  digitalWrite(RELAY_3, RELAY_3_DEFAULT);
  digitalWrite(RELAY_4, RELAY_4_DEFAULT);
  digitalWrite(TEST, TEST_DEFAULT);
}

void TEST_ON()
{
  digitalWrite(TEST, HIGH);
  Serial.println("TEST is on");
  delay(100);
}

void TEST_OFF()
{
  digitalWrite(TEST, LOW);
  Serial.println("TEST is off");
  delay(100); 
}

void TEST_RESTART()
{
  digitalWrite(TEST, !TEST_DEFAULT);
  delay(RESTART_DELAY);  
  digitalWrite(TEST, TEST_DEFAULT);
  Serial.println("TEST restarted");
}

void RELAY_1_ON()
{
  digitalWrite(RELAY_1, LOW);
  Serial.println("RELAY_1 is on");
  delay(100); 
}

void RELAY_1_OFF()
{
  digitalWrite(RELAY_1, HIGH);
  Serial.println("RELAY_1 is off");
  delay(100);
}

void RELAY_1_RESTART()
{
  digitalWrite(RELAY_1, !RELAY_1_DEFAULT);
  delay(RESTART_DELAY);  
  digitalWrite(RELAY_1, RELAY_1_DEFAULT);
  Serial.println("RELAY_1 restarted");
}

void RELAY_2_ON()
{
  digitalWrite(RELAY_2, LOW);
  Serial.println("RELAY_2 is on");
  delay(100);
}

void RELAY_2_OFF()
{
  digitalWrite(RELAY_2, HIGH);
  Serial.println("RELAY_2 is off");
  delay(100); 
}

void RELAY_2_RESTART()
{
  digitalWrite(RELAY_2, !RELAY_2_DEFAULT);
  delay(RESTART_DELAY);  
  digitalWrite(RELAY_2, RELAY_2_DEFAULT);
  Serial.println("RELAY_2 restarted");
}

void RELAY_3_ON()
{
  digitalWrite(RELAY_3, LOW);
  Serial.println("RELAY_3 is on");
  delay(100); 
}

void RELAY_3_OFF()
{
  digitalWrite(RELAY_3, HIGH);
  Serial.println("RELAY_3 is off");
  delay(100); 
}

void RELAY_3_RESTART()
{
  digitalWrite(RELAY_3, !RELAY_3_DEFAULT);
  delay(RESTART_DELAY);  
  digitalWrite(RELAY_3, RELAY_3_DEFAULT);
  Serial.println("RELAY_3 restarted");
}

void RELAY_4_ON()
{
  digitalWrite(RELAY_4, LOW);
  Serial.println("RELAY_4 is on");
  delay(100); 
}

void RELAY_4_OFF()
{
  digitalWrite(RELAY_4, HIGH);
  Serial.println("RELAY_4 is off");
  delay(100); 
}


void RELAY_4_RESTART()
{
  digitalWrite(RELAY_4, !RELAY_4_DEFAULT);
  delay(RESTART_DELAY);  
  digitalWrite(RELAY_4, RELAY_4_DEFAULT);
  Serial.println("RELAY_4 restarted");
}

void ALL_ON()
{
  RELAY_1_ON();
  RELAY_2_ON();
  RELAY_3_ON();
  RELAY_4_ON();
}

void ALL_OFF()
{
  RELAY_1_OFF();
  RELAY_2_OFF();
  RELAY_3_OFF();
  RELAY_4_OFF();
}

void ALL_RESTART()
{
  RELAY_1_RESTART();
  RELAY_2_RESTART();
  RELAY_3_RESTART();
  RELAY_4_RESTART();
}

void CallMyMaster()
{
  Serial.println("Calling My Master.");
  Serial1.print("ATDT"); // Call
  Serial1.print(MyMaster); // This number
  Serial1.println(";"); // Do it!
  Serial.println("Dialling...");
  delay(MISSED_CALL_DELAY);
  Serial.println("Hunging up...");
  Serial1.println("ATH"); // end call
  Serial.println("Hung up!");
}
