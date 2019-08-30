/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','0','+'},
  {'=','D','E'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 24, 26}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}

  char customKey=0;
  int Number=0;
  int Num1=0;
  int Num2=0;
  int Keypress=0;
  char Action=0;


void loop()
{
Serial.println("Welcome");
while (customKey!='=')
{

  customKey = customKeypad.getKey();

if (customKey!=0){
    //Serial.println(customKey);
    Keypress=customKey-48;
    //Serial.println(Keypress);

    if (Keypress >=0 && Keypress <10)
    {
      if (Number==0)
      Number=Keypress;
      else
      Number=(Number*10)+Keypress;
      Serial.println(Number);
    }

    if (customKey=='+'||customKey=='-'||customKey=='*'||customKey=='÷'&&Num1==0)
    {
    Action=customKey;
    Serial.println(Action);
    Num1=Number;
    Number=0;
    Keypress=0;
    customKey=0;
    }

    if (customKey=='+'||customKey=='-'||customKey=='*'||customKey=='÷'&&Num1!=0)
    {
      customKey='=';
    }


    
  
  }



  }


  
    Serial.println(customKey);
    Num2=Number;
    if (Action=='+')
    Serial.println(Num1+Num2);
    if (Action=='-')
    Serial.println(Num1-Num2);
    if (Action=='*')
    Serial.println(Num1*Num2);
    if (Action=='÷')
    Serial.println(Num1/Num2);
    delay(50000);


}
