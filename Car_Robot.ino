#include <ATX2.h>  // ATX2 Board
#include <Wire.h>

#include <MPU6050_light.h>

MPU6050 mpu(Wire);
unsigned long timer = 0;

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw = 10;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
int yawbefore = 10 ;
int decline = 0;
//unsigned long currentTime = 0;
//unsigned long nextTime = 0;


// ค่าสี blue g = 266 r = 972 b = 992
// ค่าสี red g = 219 r = 665 b = 997
// ค่าสี yellow g = 813 r = 955 b = 985
// ค่าสี green g =881 r = 976 b = 992
int black = 360;

int csr,csl,csf;
int g,r,y,b;
void setup() {
  XIO();  // ATX2 initialize
  Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  delay(20);
  OK();
  glcdClear();
  
  mpu.begin();
  mpu.calcGyroOffsets();  
}

void loop() {

    // === Read gyro data === //
    Serial.println(Gyro());


  //servo(1,90);
  //delay(3000);
  //servo(1,150);
  //delay(3000);
  //ck_sensor();
  //encoder_glcd();
  //ck_color();
  //delay(1000);
  go_1_block();
  //glcd(5,0,"%d",Gyro());
  //glcd(0,0,"%d",in(28));
  //delay(50);
}

int Gyro(){
  mpu.update();  
  if((millis()-timer)>10)                         // print data every 10ms
  {                                           
                       
    // Serial.print("P : ");
    // Serial.print(mpu.getAngleX());
    //Serial.print(" R : ");
    //Serial.print(mpu.getAngleY());
    //Serial.print(" Y : ");
    // Serial.println(mpu.getAngleZ());
    decline = mpu.getAngleY();
    timer = millis();  
  }
  return decline;
}

void go_1_block()
{
  resetenc();
  csf = 0;
  glcd(0,0,"aw= %d",yaw);
  glcd(1,0,"re= %d",yawbefore);
  glcd(2,0,"%d",yaw-yawbefore);

  motor_all(40,40);
    while(encoder(2)<60 && encoder(3)<60)
  {


       //motor_st(45,25,1);
       ck_color2();

        if(analog(8)<black && analog(9) < black)
    {
        g,b,y,r = 0;
      AO();
      csf =1;
      
      motor_all(-40,-40);
      delay(350);
      AO();
      resetenc();
      ck_black3();
      ck_side();
      ck_color();
     //beep()
      AO();
      goto finished4;
    } 

    
    /*if(gy > 105 || gy < 75)
    {
      beep();
      glcd(1,1,"gy = %d",gy);
      //motor_all(70,70);
      delay(600);
      AO();
    }
    */


  }
  //encoder_glcd();
  //car_straight(64);
      if(Gyro() >= 48)
    {
      beep();
      motor_all(70,70);
      delay(500);
      AO();
      resetEnc();
      while(encoder(2)< 150|| encoder(3)<150)
      {
        
        motor_all(20,20);
        if(analog(8)<black && analog(9)<black)
        {
          csf = 1;
          AO();
          motor_all(-20,-20);
          delay(300);
          ck_black3();
          ck_side_slow(15);
        }
      }
      motor_all(20,20);
      
      goto finished4;
    }
  ck_side();
  yawbefore = yaw;
  finished4:;

  
  
}


void cube1()
{
  servo(2,30);
  delay(400);
  servo(1,0);
  delay(400);
  servo(1,75);
  servo(2,90);
}

void cube2()
{
  servo(2,120);
  delay(400);
  servo(1,180);
  delay(400);
  servo(1,75);
  servo(2,90);
}

void cube3()
{
  servo(3,30);
  delay(400);
  servo(4,0);
  delay(400);
  servo(3,90);
  servo(4,90);
}

void cube4()
{
  servo(3,140);
  delay(400);
  servo(4,180);
  delay(400);
  servo(3,90);
  servo(4,90);
}

void ck_color2()
{
     if(encoder(2) > 15 && encoder(3) > 15 && b == 1 || y == 1 || r ==1 || g == 1)
   {
    AO();
    motor_all(30,30);
    delay(300);
    AO();
        if(b == 1)
    {
      glcdFillScreen(GLCD_BLUE);
      delay(300);
      glcdFillScreen(GLCD_BLACK);
      cube1();
      b = 0;
      goto finishcolor;
    }
        if(y == 1)
    {
      glcdFillScreen(GLCD_YELLOW);
      delay(300);
      glcdFillScreen(GLCD_BLACK);
      y = 0;
      cube2();
      goto finishcolor;
    }
        if(r == 1)
    {
      cube4();
      glcdFillScreen(GLCD_RED);
      delay(300);
      glcdFillScreen(GLCD_BLACK);
      r = 0;
      goto finishcolor;
    }
        if(g == 1)
    {
      glcdFillScreen(GLCD_GREEN);
      delay(300);
      glcdFillScreen(GLCD_BLACK);
      g = 0;
      cube3();
      goto finishcolor;
    }
    finishcolor:;
    beep();
    b,r,y,g = 0;
    motor_all(35,35);
    delay(100);
   }
}


void ck_color()
{
  
// ค่าสี blue g = 609 r = 150 b = 720
// ค่าสี red g = 232 r = 964 b = 699
// ค่าสี yellow g = 989 r = 450 b = 500
// ค่าสี green g =990 r = 167 b = 879
// green  = analog5 
// red    = analog6
// blue   = analog7

//  int RedR = ;
//  int RedG = ;
//  int RedB = ;
//  int RedR_min = RedR - 100;
//  int RedR_max = RedR + 100;
//  int RedG_min = RedG - 100;
//  int RedG_max = RedG + 100;
//  int RedB_min = RedB - 100;
//  int RedB_max = RedB + 100;

  int GreenR = 186;
  int GreenG = 990;
  int GreenB = 884;
  int GreenR_min = GreenR - 100;
  int GreenR_max = GreenR + 100;
  int GreenG_min = GreenG - 100;
  int GreenG_max = GreenG + 100;
  int GreenB_min = GreenB - 100;
  int GreenB_max = GreenB + 100;

  int BlueR = 150;
  int BlueG = 650;
  int BlueB = 750;
  int BlueR_min = BlueR - 100;
  int BlueR_max = BlueR + 100;
  int BlueG_min = BlueG - 100;
  int BlueG_max = BlueG + 100;
  int BlueB_min = BlueB - 100;
  int BlueB_max = BlueB + 100;

  int YellowR = 478;
  int YellowG = 950;
  int YellowB = 500;
  int YellowR_min = YellowR - 100;
  int YellowR_max = YellowR + 100;
  int YellowG_min = YellowG - 100;
  int YellowG_max = YellowG + 100;
  int YellowB_min = YellowB - 100;
  int YellowB_max = YellowB + 100;

  
  
  if(BlueG_min < analog(5) && analog(5) < BlueG_max &&  BlueR_min < analog(6) && analog(6) < BlueR_max &&  BlueB_min < analog(7) && analog(7) < BlueB_max)
  {
    glcdFillScreen(GLCD_BLUE);
    delay(300);
    b = 1;
    glcdFillScreen(GLCD_BLACK);
  }
//    if(RedG_min < analog(5) && analog(5) < RedG_max &&  RedR_min < analog(6) && analog(6) < RedR_max &&  RedB_min < analog(7) && analog(7) < RedB_max)
//  {
//    glcdFillScreen(GLCD_RED);
//    delay(300);
//    r = 1;
//    glcdFillScreen(GLCD_BLACK);
//  }
    if(YellowG_min < analog(5) && analog(5) < YellowG_max &&  YellowR_min < analog(6) && analog(6) < YellowR_max &&  YellowB_min < analog(7) && analog(7) < YellowB_max)
  {
    glcdFillScreen(GLCD_YELLOW);
    delay(300);
    y = 1;
    glcdFillScreen(GLCD_BLACK);
  }
    if(GreenG_min < analog(5) && analog(5) < GreenG_max &&  GreenR_min < analog(6) && analog(6) < GreenR_max &&  GreenB_min < analog(7) && analog(7) < GreenB_max)
  {
    glcdFillScreen(GLCD_GREEN);
    delay(300);
    g = 1;
    glcdFillScreen(GLCD_BLACK);
  }
  
}

void ck_side() // analog1 = l , analogr = 0 dark = 1 white = 0
{ 
  int i=0;
  csr=0;
  csl=0;
  resetenc();
  
  while(analog(0)>black)
  {
    
    motor_sideright(40);
    if(encoder(2) > 30 && encoder(3) > 30 )
    {
      AO();
      delay(50);
      resetenc();
      AO();
      delay(50);
      while(encoder(2) < 30 && encoder(3) < 30)
      {
        motor_sideleft(40);
      }
      goto csr_finish;
    }
  }
  AO();
  csr =1;
  resetenc();
  while(encoder(2)< 16 && encoder(3)<16)
  {
    motor_sideleft(40);
  }
  csr_finish:;
  AO();
  delay(50);
  resetenc();
  delay(10);
  if(csr == 0 || csf ==1)
  { 
    resetenc();
 /* while(encoder(2) <40 && encoder(3)<40)
  {
    motor_all(45,45);
    if(analog(8)<400 || analog(9)<400)
    {
      AO();
      delay(10);
      ck_black3();
      csf =1;
      goto csf_finish;
    }
  }  */
    while(analog(1)>black)
  {
    motor_sideleft(40);
    if(encoder(2) > 35 && encoder(3) >35 )
    {
      AO();
      delay(10);
      resetenc();
      while(encoder(2) < 35 && encoder(3) < 35)
      {
        motor_sideright(40);
      }
      goto csl_finish;
    }
  }
  AO();
  csl =1;
  resetenc();
  while(encoder(2)<18 && encoder(3)<18)
  {
   motor_sideright(40);
  }
  delay(50);
  csl_finish:;
  AO();
  delay(50);
  resetenc();
  delay(10);
  
  } //white = 0 dark =1
    if(csl == 1 && csr == 1 && csf == 1)
    {
      turnaround();
      ckr_black();
      goto finished3;
    }
      if(csl == 0 && csr == 1 && csf == 1)
  {
    turnleft();
    ckr_black();
  }
  if(csr == 0 && csl == 0)
  {
    turnright();
  }
  if(csl == 1 && csr ==0)
  { 
    turnright();
    ckr_black();
  }

  finished3:;




  
  
}


void ck_side_slow(int speedy) // analog1 = l , analogr = 0 dark = 1 white = 0
{ 
  int i=0;
  csr=0;
  csl=0;
  resetenc();
  
  while(analog(0)>black)
  {
    
    motor_sideright(speedy);
    if(encoder(2) > 30 && encoder(3) > 30 )
    {
      AO();
      delay(50);
      resetenc();
      AO();
      delay(50);
      while(encoder(2) < 30 && encoder(3) < 30)
      {
        motor_sideleft(speedy);
      }
      goto csr_finish;
    }
  }
  AO();
  csr =1;
  resetenc();
  while(encoder(2)< 16 && encoder(3)<16)
  {
    motor_sideleft(speedy);
  }
  csr_finish:;
  AO();
  delay(50);
  resetenc();
  delay(10);
  if(csr == 0 || csf ==1)
  { 
    resetenc();
 /* while(encoder(2) <40 && encoder(3)<40)
  {
    motor_all(45,45);
    if(analog(8)<400 || analog(9)<400)
    {
      AO();
      delay(10);
      ck_black3();
      csf =1;
      goto csf_finish;
    }
  }  */
    while(analog(1)>black)
  {
    motor_sideleft(speedy);
    if(encoder(2) > 35 && encoder(3) >35 )
    {
      AO();
      delay(10);
      resetenc();
      while(encoder(2) < 35 && encoder(3) < 35)
      {
        motor_sideright(speedy);
      }
      goto csl_finish;
    }
  }
  AO();
  csl =1;
  resetenc();
  while(encoder(2)<18 && encoder(3)<18)
  {
   motor_sideright(speedy);
  }
  delay(50);
  csl_finish:;
  AO();
  delay(50);
  resetenc();
  delay(10);
  
  } //white = 0 dark =1
    if(csl == 1 && csr == 1 && csf == 1)
    {
      turnaround();
      ckr_black();
      goto finished3;
    }
      if(csl == 0 && csr == 1 && csf == 1)
  {
    turnleft();
    ckr_black();
  }
  if(csr == 0 && csl == 0)
  {
    turnright();
  }
  if(csl == 1 && csr ==0)
  { 
    turnright();
    ckr_black();
  }

  finished3:;




  
  
}
void resetenc()
{
  AO();
  delay(30);
  resetEnc(2);
  resetEnc(3);
  delay(20);
}

void ck_sensor()
{
  setTextSize(1);
  glcd(0,0,"fl = %d",analog(9));
  glcd(1,0,"fr = %d",analog(8));
  glcd(2,0,"cl = %d",analog(1));
  glcd(3,0,"cr = %d",analog(0));
  glcd(4,0,"rl = %d",analog(11));
  glcd(5,0,"rr = %d",analog(10));
  glcd(7,0,"red = %d ",analog(6));
  glcd(6,0,"green = %d",analog(5)); 
  glcd(8,0,"blue = %d",analog(7));
}

void encoder_glcd()
{
  setTextSize(1);
  glcd(0,0," ed_left : %d ",encoder(2));
  glcd(1,0," ed_right : %d ",encoder(3));
}

void motor_sideleft(int spd)
{
  motor(1,-spd);
  motor(2,spd);
  motor(3,spd);
  motor(4,-spd);
}

void motor_sideright(int spd)
{
  motor(1,spd-3);
  motor(2,-spd);
  motor(3,-spd);
  motor(4,spd-3);
}

void motor_all(int mt_l,int mt_r)
{
  motor(1,mt_l);
  motor(2,mt_r+2);
  motor(3,mt_l);
  motor(4,mt_r+2);
}

void motor_st(int mt_start,int mt_sl,int delay_st)
{
  while(mt_start>mt_sl)
  {
    mt_start--;
    motor_all(mt_start,mt_start);
    delay(delay_st);
  }
}

void turn()
{
          if(csr == 0 && csl == 1 && csf ==1)
        {
          turnright();
          ckr_black();
        }
        else if(csr == 0 && csl == 1 && csf ==0)
        {
          turnright();
          ckr_black();
        }
        else if(csr == 0 && csl == 0 && csf == 1)
        {
          turnright();
        }
        else if(csl == 1 && csr == 1 && csf == 1)
        {
          turnaround();
          ckr_black();
        }
}
void turnaround()
{
  AO();
  delay(100);
/*  resetEnc(2);
  resetEnc(3);
while(encoder(2) < 8 || encoder(3) < 8)
  {
    motor_all(50,-50);
  }  */
  motor_all(80,-80);
  delay(600);
  AO();
  //ckr_black();
  AO();
  //motor_all(30,30);
  //delay(100);
  //AO();
  
}

void turnright()
{
  AO();
  delay(100);
/*  resetEnc(2);
  resetEnc(3);
while(encoder(2) < 8 || encoder(3) < 8)
  {
    motor_all(50,-50);
  }  */
  motor_all(80,-80);
  delay(300);
  AO();
  //ckr_black();
  //AO();
  //motor_all(30,30);
  //delay(450);
  //AO();
}

void turnleft()
{
  AO();
  delay(100);
/*  resetEnc(2);
  resetEnc(3);
  while(encoder(2) < 8 || encoder(3) < 8)
  {
    motor_all(-50,50);
  }  */
  motor_all(-80,80);
  delay(250);
  AO();
  //ckr_black();
  //AO();
  //motor_all(30,30);
  //delay(250);
  //AO();

  
}

void ckr_black()
{
  for(int i = 0;i<2;i++){
  motor_all(30,30);
  delay(200);
  AO();
  while(analog(10) > 400 && analog(11) > 400)
  {
    motor_all(-30,-30); 
  }
  if(analog(11) < black)
  {
    while(analog(10)>black)
    {
      motor_all(0,-20);
      if(analog(10) >black && analog(11) >black)
      {
        goto br1;
      }
    }
  }
  br1:;
  if(analog(10) < black)
  {
    while(analog(11)>black)
    {
      motor_all(-20,0);
      if(analog(10) > black && analog(11) >black)
      {
        goto br2;
      }
    }
  }
  br2:;
}
        motor_all(20,20);
        delay(400);
        AO();
}



void ck_black3()
{
  for(int i = 0;i<2;i++)
{
  motor_all(-20,-20);
  delay(150);
  while(analog(9) > black && analog(8) > black)
  {
    motor_all(20,20); 
  }
  if(analog(9) < 400)
  {
    while(analog(8)>black)
    {
      motor_all(0,15);
    }
  }
  if(analog(8) < black)
  {
    while(analog(9)>black)
    {
      motor_all(15,0);
    }
  }
}
        motor_all(-25,-25);
        delay(300);
        AO();
}

void car_straight(int enco_limit)             //+2 เพราะไรไม่รู้แต่มันเดินตรง
{
  AO();
  delay(50);
  int mtor;
  if(encoder(2) < encoder(3))
  {
    mtor = 1;
  }
  if(encoder(2) > encoder(3))
  {
    mtor = 2;
  }
  switch(mtor)
  {
    case 1:
      if(encoder(2) != enco_limit)
  {
    while(encoder(2) > enco_limit)
    {
      motor_all(-10,0);
      delay(5);
    }
    while(encoder(2) < enco_limit)
    {
      motor_all(10,0);
      delay(5);
    }
  }
  delay(50);
  if(encoder(3) != enco_limit)
  {
    while(encoder(3) > enco_limit+2)
    {
    motor_all(0,-10);
    delay(5);
    }
    while(encoder(3) < enco_limit+2)
    {
      motor_all(0,10);
      delay(5);
    }
    break;
  case 2:
  if(encoder(3) != enco_limit)
  {
    while(encoder(3) > enco_limit+2)
    {
    motor_all(0,-10);
    delay(5);
    }
    while(encoder(3) < enco_limit+2)
    {
      motor_all(0,10);
      delay(5);
    }
  }
  delay(50);
  if(encoder(2) != enco_limit)
  {
    while(encoder(2) > enco_limit+2)
    {
      motor_all(-10,0);
      delay(5);
    }
    while(encoder(2) < enco_limit+2)
    {
      motor_all(10,0);
      delay(5);
    }
  }
  break;
  }
  }
}


  
