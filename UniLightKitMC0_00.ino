
#define LOWSPEED 1096L
#define LOWANGLE 1000L
#define MAXSPEED 1880L
#define MAXANGLE 1980L
#define NRMSPEED 1488L
#define NRMANGLE 1488L
#define NEXP 5

int N, Enc0, Enc1;
byte OldC, OldD, OldB;
int PulseSpeed,PulseAngle, PWMLeft,PWMRight,tSpeed,tTurn,Aavr, OldSpeed;
  long Time0,Time1,Time2,Timet;
unsigned char ModeLight;
float k;
int j13,j13C=100,Speed,Turn,Mode;
void setup()
{
  EICRA = 0x00; //  INT0 INT1 Type didn't matter
  EIMSK = 0x00; //       INT0 INT1 disabled

  PCMSK0 = 0x00;
  PCMSK1 = 0x07; // Pin change mask pins C0 C1 C2 ENABLE any change
  PCMSK2 = 0x00;
  PCIFR = 0x02; //PCINT 14-8 Flag   clear
  PCICR = 0x02; //Pins C0 - C5 interruptions enable

  // here we config input  pins for encoder and
  pinMode(A0, INPUT_PULLUP );
  pinMode(A1, INPUT_PULLUP );
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);

//  pinMode (13, OUTPUT);
//
//  pinMode (2, OUTPUT);
//  pinMode(3, OUTPUT);
//
// pinMode (4, OUTPUT);
//  pinMode(5, OUTPUT);

  Serial.begin(115200);
  Serial.println("And now we begin  _6Wheel_Remote Vers 00");
}

void loop()
{
  

if(0)
{
  delay(10);
  Serial.print("  Forward = ");
  Serial.print(ModeLight&0x01);

  Serial.print("  Back = ");
  Serial.print(ModeLight&0x02 );


  
   Serial.print("  Light = ");
  Serial.print(ModeLight&0x04 );

  

  Serial.print("  Left = ");
  Serial.print(ModeLight&0x08 );
  
   Serial.print("  Right = ");
  Serial.print(ModeLight&0x10 );

  Serial.print(" BackFar = ");
  Serial.print(ModeLight&0x20 );
   Serial.print(" Far = ");
  Serial.print(ModeLight&0x40 );

  Serial.print("  Stop = ");
  Serial.println(ModeLight&0x80 );
}

if(0)
{
  delay(10);
  Serial.print("  Speed = ");
  Serial.print(Speed);
  Serial.print(" Turn = ");
  Serial.print(Turn);

 Serial.print("  Mode = ");
  Serial.println(Mode);
//  Serial.print(" Pulse 1 = ");
//  Serial.print (Time1);




}
delay(20);
Serial.write(ModeLight);


}




ISR (PCINT1_vect)// pin change interrupt for A0 to A5
{

  if (  ( (PINC & 0x01) == 0 )     && ((OldC & 0x01) == 1)      )
    {
    Turn= int(micros()-Time0 )-1500;
    if(Turn<-20)
    {
    ModeLight|=0x08;
    ModeLight&= ~0x10;
    }
    else if (Turn>20)
{
    ModeLight&=  ~0x08;
    ModeLight |= 0x10;
    }
else
{
ModeLight&=  ~0x08;
  ModeLight&= ~0x10;
}
    
    }

  if (  ( (PINC & 0x01) == 1 )     && ((OldC & 0x01) == 0)      )
  {
    Time0=micros();
  }

   if (  ( (PINC & 0x02) == 0 )     && ((OldC & 0x02) == 2)      )
    {
    Speed= int(micros()-Time1 )-1500;

      if(Speed>300)
      ModeLight |= 0x40;
      else
      ModeLight &=~0x40;
      

      if(Speed< -300)
      ModeLight |= 0x20;
      else
      ModeLight &=~0x20;

      if(Speed>10)
      ModeLight |=0x01;
      else
      ModeLight &= ~0x01;

 if(Speed < -10)
      ModeLight |=0x02;
      else
      ModeLight &= ~0x02;

      

Aavr=(2* (Speed-OldSpeed)+ (NEXP-1)*Aavr)/(NEXP+1);
if (Aavr<-4  && Speed> -4)
{
ModeLight |= 0x80;
}
else if(Aavr>-4)
{
ModeLight &= ~0x80;
}

OldSpeed=Speed;



    
    }

  if (  ( (PINC & 0x02) == 2 )     && ((OldC & 0x02) == 0)      )
  {
    Time1=micros();
  }

  if (  ( (PINC & 0x04) == 0 )     && ((OldC & 0x04) == 4)      )
    {
    Mode= int(micros()-Time2 )-1500;
    if (Mode <-100)
    ModeLight &= ~0x04;
    else
    ModeLight |= 0x04;
    }

  if (  ( (PINC & 0x04) == 4 )     && ((OldC & 0x04) == 0)      )
  {
    Time2=micros();
  }
  

  OldC = PINC;
}




//ISR (PCINT2_vect) // pin change interrupt for D0 to D7
//{
//
//}



