#include <Keypad.h>
const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'/','*','-','C'},
  {'7','8','9','+'},
  {'4','5','6','Z'},
  {'1','2','3','='},
  {'0','E','.','~'}
};
byte rowPins[ROWS] = {22, 24, 26, 28, 30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {23, 25, 27, 29}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  Serial1.begin(19200);
  Serial1.print("page 0");
  SendCommand();
  int Ram=0;
  delay(500);
  tone(7,880,300);
  delay(500);
  Serial1.print("page 1");
  SendCommand();
  while (Ram<98)
  {
    Serial1.print("RamCT.val=");
    Serial1.print(Ram);
    SendCommand();
    Ram=Ram+8;
    delay(700);
  }
  tone(7,880,150);
  delay(400);
  tone(7,880,150);
  delay(300);
  Serial1.print("page 2");
  SendCommand();
  delay(3000);
  Serial1.print("page 3");
  SendCommand();
}














char customKey=0;



void loop()
{
  customKey = customKeypad.getKey();
  if (customKey)
  {
    if(customKey=='1')
    {
      calculator();
      Serial1.print("page 3");
      SendCommand();
    }
    if(customKey=='2')
    {
      timer();
      Serial1.print("page 3");
      SendCommand();
    }
  }

 delay(100);

  
}
























//-------------------------------------------------CALCULATOR-------------------------------------------------

int calculator()
{
  delay(50);
  Serial1.print("page 4");
  SendCommand();
  String LineData[2][5]={
    {"0","0","0","0","0"},
    {"0","0","0","0","0"}
  };
  double ANS=0;
  int Line=0;
  Start:
  customKey=0;
  double Number=0;
  double Num1=0;
  double Num2=0;
  double Keypress=0;
  char Action=0;
  bool Decimal=false;
  int DecimalPlace=10;
  bool isNegative=false;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  while (customKey!='=')
  {

  customKey = customKeypad.getKey();

  if (customKey)
      {
      Keypress=customKey-48;


      if (Keypress >=0 && Keypress <10&&Decimal==false)
       {
          if (Number==0)
          {
          Number=Keypress;
          }
          else
          {
          Number=(Number*10)+Keypress;
          }
          Serial1.print("In");
          Serial1.print(Line);
          Serial1.print(".txt=");
          Serial1.write(0x22);
          if(isNegative==true)
            {
            Serial1.print("-");
            }
          if(Num1!=0)
            {
              Serial1.print(Num1,newPrecision(Num1));
              Serial1.print(Action);
            }
          Serial1.print(Number,newPrecision(Number));
          Serial1.write(0x22);
          SendCommand();
        }
      
      if (Keypress >=0 && Keypress <10&&Decimal==true)
        {
          Number=Number+Keypress/DecimalPlace;
          DecimalPlace=DecimalPlace*10;
          Serial1.print("In");
          Serial1.print(Line);
          Serial1.print(".txt=");
          Serial1.write(0x22);
          if(isNegative==true)
            {
            Serial1.print("-");
            }
          if(Num1!=0)
            {
              Serial1.print(Num1,newPrecision(Num1));
              Serial1.print(Action);
            }
          Serial1.print(Number,newPrecision(Number));
          Serial1.write(0x22);
          SendCommand();
        }
        
//-------------------------------------------------------------------------------------------------------------------------------------------------------

      if (customKey=='E')
      {
        return 0;
      }
      if (customKey=='.')
        {
          Decimal=true;
        }

      if (customKey=='~')
        {
          isNegative=true;
        }

      if (customKey=='C')
        {
          calculator();
        }
      
      if ((customKey=='+'||customKey=='-'||customKey=='*'||customKey=='/')&&Num1!=0)
        {
          customKey='=';
        }



      if ((customKey=='+'||customKey=='-'||customKey=='*'||customKey=='/')&&Num1==0)
        {
          Action=customKey;
        if (Number==0&&ANS!=0)
        {
          Num1=ANS;
        }
        else
        {
        Num1=Number;
        Serial1.print("In");
        Serial1.print(Line);
        Serial1.print(".txt=");
        Serial1.write(0x22);
        if(isNegative==true)
        {
          Serial1.print("-");
        }
        Serial1.print(Num1,newPrecision(Num1));
        Serial1.print(Action);
        Serial1.write(0x22);
        SendCommand();
        }
        
        if (isNegative==true)
        {
          Num1=Num1*-1;
          isNegative=false;
        }
        
        //Serial.println(Action);
        Number=0;
        Keypress=0;
        Decimal=false;
        DecimalPlace=10;
        }

//------------------------------------------------------------------------------------------------------------------------------------------------
      
      
      }



    }


  
//Serial.println(customKey);
Num2=Number;

if(isNegative==true)
  {
  Num2=Num2*-1;
  }
  
  if (Action=='+')
  ANS=Num1+Num2;
  if (Action=='-')
  ANS=Num1-Num2;
  if (Action=='*')
  ANS=Num1*Num2;
  if (Action=='/')
  ANS=Num1/Num2;
  
delay(400);
if(Line<4)
{
Serial1.print("Div");
Serial1.print(Line);
Serial1.print(".pco=64866");
SendCommand();
delay(30);
}
Serial1.print("Ans");
Serial1.print(Line);
Serial1.print(".txt=");
Serial1.write(0x22);
Serial1.print(ANS,newPrecision(ANS));
Serial1.write(0x22);
SendCommand();


int Length=0;
  Serial1.print("get In");
  Serial1.print(Line);
  Serial1.print(".txt");
  SendCommand();
  delay(50);
  while (Serial1.available() > 0)
  {
    LineData[0][Line]=Serial1.readString();
    //delay(50);
    
  }
  Length=LineData[0][Line].length();
  LineData[0][Line].remove(Length-3,3);
  LineData[0][Line].remove(0,1);

  Serial1.print("get Ans");
  Serial1.print(Line);
  Serial1.print(".txt");
  SendCommand();
  delay(50);
  while (Serial1.available() > 0)
  {
    LineData[1][Line]=Serial1.readString();
    //delay(50);
  }
  Length=LineData[1][Line].length();
  LineData[1][Line].remove(Length-3,3);
  LineData[1][Line].remove(0,1);

if(Line==4)
{
int Count=4;
while(Count!=0)
{
Serial1.print("In");
Serial1.print(Count-1);
Serial1.print(".txt=");
Serial1.write(0x22);
Serial1.print(LineData[0][Count]);
Serial1.write(0x22);
SendCommand();
delay(50);
Serial1.print("Ans");
Serial1.print(Count-1);
Serial1.print(".txt=");
Serial1.write(0x22);
Serial1.print(LineData[1][Count]);
Serial1.write(0x22);
SendCommand();
delay(50);
if(Count==4)
{
Serial1.print("In4");
Serial1.print(".txt=");
Serial1.write(0x22);
Serial1.write(0x22);
SendCommand();
delay(50);
Serial1.print("Ans4");
Serial1.print(".txt=");
Serial1.write(0x22);
Serial1.write(0x22);
SendCommand();
}
Count=Count-1;
}
delay(30);
Serial1.print("Div4");
Serial1.print(".pco=64866");
SendCommand();
Count=1;
while(Count!=5)
{
  LineData[0][Count-1]=LineData[0][Count];
  LineData[1][Count-1]=LineData[1][Count];
  Count=Count+1;
}
LineData[0][4]="";
LineData[1][4]="";
Line=3;
}


    Line=Line+1;
    Serial1.print("In");
    Serial1.print(Line);
    Serial1.print(".txt=");
    Serial1.write(0x22);
    Serial1.print("0");
    Serial1.write(0x22);
    SendCommand();
    goto Start;

}

//-------------------------------------TIMER--------------------------------------------------------


int timer()
{
  Serial1.print("page 5");
  SendCommand();
  int hours=0;
  int minutes=0;
  int seconds=0;
  int Keypress=0;
  int Pointer=0;
  int Number=0;
  customKey=0;
  bool FlipFlop=false;
while(customKey!='E')
{
customKey = customKeypad.getKey();
if(customKey)
{
  Keypress=customKey-48;
  if (Keypress >=0 && Keypress <10)
  {
       if (Number==0)
       {
       Number=Keypress;
       }
       else
       {
       Number=(Number*10)+Keypress;
       }    
       if(Pointer==0){
       Serial1.print("Hours");}
       if(Pointer==1){
       Serial1.print("Minutes");}
       if(Pointer==2){
       Serial1.print("Seconds");
       }
       Serial1.print(".val=");
       Serial1.print(Number);
       SendCommand();
  }
}
  if(customKey=='=')
  {
    
    if(Pointer==2)
    {
    seconds=Number;
    Pointer=Pointer+1;
    Serial1.print("Seconds.pco=64866");
    SendCommand();
    Serial1.print("t0.pco=0");
    SendCommand();
    Serial1.print("EnterBox.txt=");
    Serial1.write(0x22);
    Serial1.print("Time Remaining:");
    Serial1.write(0x22);
    SendCommand();    
    }
    
    if(Pointer==1)
    {
    minutes=Number;
    Pointer=Pointer+1;
    Serial1.print("Minutes.pco=64866");
    SendCommand();
    Serial1.print("EnterBox.txt=");
    Serial1.write(0x22);
    Serial1.print("Enter Seconds:");
    Serial1.write(0x22);
    SendCommand();
    Serial1.print("Seconds.pco=63488");
    SendCommand();    
    }
    
    if(Pointer==0)
    {
    hours=Number;
    Pointer=Pointer+1;
    Serial1.print("Hours.pco=64866");
    SendCommand();
    Serial1.print("EnterBox.txt=");
    Serial1.write(0x22);
    Serial1.print("Enter Minutes:");
    Serial1.write(0x22);
    SendCommand();
    Serial1.print("Minutes.pco=63488");
    SendCommand();
    }
  Serial.println(Pointer);
  customKey=0;
   Number=0;
  }

  if(customKey=='C')
  {
    timer();
  }

  if(Pointer==3&&(hours>0||minutes>0||seconds>-1))
  {

    if(hours!=0&&minutes==0&&seconds<0)
    {
      hours=hours-1;
      minutes=minutes+59;
      seconds=seconds+60;
    }

    
    if(seconds<0&&minutes!=0)
    {
      minutes=minutes-1;
      seconds=seconds+60;
    }
       Serial1.print("Hours.val=");
       Serial1.print(hours);
       SendCommand();
       Serial1.print("Minutes.val=");
       Serial1.print(minutes);
       SendCommand();
       Serial1.print("Seconds.val=");
       Serial1.print(seconds);
       SendCommand();
       if(FlipFlop==true)
       {
       seconds=seconds-1;
       }
       FlipFlop=!FlipFlop;
       delay(500);
  }

  if(hours==0&&minutes==0&&seconds==-1&&Pointer==3)
  {
    if(FlipFlop==true)
    {
      tone(7,1200,150);
      Serial1.print("Hours.pco=63488");
      SendCommand();
      Serial1.print("Col0.pco=63488");
      SendCommand();
      Serial1.print("Minutes.pco=63488");
      SendCommand();
      Serial1.print("Col1.pco=63488");
      SendCommand();
      Serial1.print("Seconds.pco=63488");
      SendCommand();            
      delay(500);
    }
    if(FlipFlop==false)
    {
      tone(7,1200,150);
      Serial1.print("Hours.pco=64866");
      SendCommand();
      Serial1.print("Col0.pco=64866");
      SendCommand();
      Serial1.print("Minutes.pco=64866");
      SendCommand();
      Serial1.print("Col1.pco=64866");
      SendCommand();
      Serial1.print("Seconds.pco=64866");
      SendCommand();                  
      delay(500);
    }
    FlipFlop=!FlipFlop;
  }

}
  return 0;
}












//----------------------------------FUNCTIONS-------------------------------------------------------------
int newPrecision(double num)
{
  int m=0;
  while ((num-int(num))>.00000001&&m<8)
{
num=num*10;
m=m+1;
//Serial.println(m);
}
return(m);
}


int SendCommand()
{
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  return 0;
}






































// timers TC0 TC1 TC2   channels 0-2 ids 0-2  3-5  6-8     AB 0 1
// use TC1 channel 0 
#define TONE_TIMER TC1
#define TONE_CHNL 0
#define TONE_IRQ TC3_IRQn

// TIMER_CLOCK4   84MHz/128 with 16 bit counter give 10 Hz to 656KHz
//  piano 27Hz to 4KHz

static uint8_t pinEnabled[PINS_COUNT];
static uint8_t TCChanEnabled = 0;
static boolean pin_state = false ;
static Tc *chTC = TONE_TIMER;
static uint32_t chNo = TONE_CHNL;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

// frequency (in hertz) and duration (in milliseconds).

void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
    const uint32_t rc = VARIANT_MCK / 256 / frequency; 
    tone_pin = ulPin;
    toggle_count = 0;  // strange  wipe out previous duration
    if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
     else toggle_count = -1;

    if (!TCChanEnabled) {
      pmc_set_writeprotect(false);
      pmc_enable_periph_clk((uint32_t)TONE_IRQ);
      TC_Configure(chTC, chNo,
        TC_CMR_TCCLKS_TIMER_CLOCK4 |
        TC_CMR_WAVE |         // Waveform mode
        TC_CMR_WAVSEL_UP_RC ); // Counter running up and reset when equals to RC
  
      chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  // RC compare interrupt
      chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
       NVIC_EnableIRQ(TONE_IRQ);
                         TCChanEnabled = 1;
    }
    if (!pinEnabled[ulPin]) {
      pinMode(ulPin, OUTPUT);
      pinEnabled[ulPin] = 1;
    }
    TC_Stop(chTC, chNo);
                TC_SetRC(chTC, chNo, rc);    // set frequency
    TC_Start(chTC, chNo);
}

void noTone(uint32_t ulPin)
{
  TC_Stop(chTC, chNo);  // stop timer
  digitalWrite(ulPin,LOW);  // no signal on pin
}

// timer ISR  TC1 ch 0
void TC3_Handler ( void ) {
  TC_GetStatus(TC1, 0);
  if (toggle_count != 0){
    // toggle pin  TODO  better
    digitalWrite(tone_pin,pin_state= !pin_state);
    if (toggle_count > 0) toggle_count--;
  } else {
    noTone(tone_pin);
  }
}
