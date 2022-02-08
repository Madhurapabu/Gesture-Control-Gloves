


//Code for Gesture Controlled Robotic ARM (Arduino Nano & MPU6050)
//Circuit Digest

#include<Wire.h>                //I2C Wire Library 
#include <MPU6050.h>

const int MPU_addr=0x68;        //MPU6050 I2C Address
int16_t axis_X,axis_Y,axis_Z;
int minVal=265;
int maxVal=402;

double x;
double y;
double z;

int c = 0;
double Up_val[2];
int Up_move;
int c_up = 0;
int mov1;

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  Wire.begin();                      //Initilize I2C Communication
  Wire.beginTransmission(MPU_addr);  //Start communication with MPU6050
  Wire.write(0x6B);                  //Writes to Register 6B
  Wire.write(0);                     //Writes 0 into 6B Register to Reset
  Wire.endTransmission(true);        //Ends I2C transmission
}

void loop()
{
if (digitalRead(2) == HIGH){

 
    Wire.beginTransmission(MPU_addr);    
    Wire.write(0x3B);                  //Start with regsiter 0x3B 
    Wire.endTransmission(false);     
    Wire.requestFrom(MPU_addr,14,true);  //Read 14 Registers 
    
    axis_X=Wire.read()<<8|Wire.read();                //Reads the MPU6050 X,Y,Z AXIS Value
    axis_Y=Wire.read()<<8|Wire.read();
    axis_Z=Wire.read()<<8|Wire.read();
    
    int xAng = map(axis_X,minVal,maxVal,-90,90);     // Maps axis values in terms of -90 to +90  
    int yAng = map(axis_Y,minVal,maxVal,-90,90);
    int zAng = map(axis_Z,minVal,maxVal,-90,90);
         
    x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);       //Formula to convert into degree
    y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    
    if(x >20 && x <= 60) 
    {
        mov1 = map(x,0,60,0,90);
        
        if( mov1 > 70 ){
         Serial.println("Left");
         Serial.println("..............");
         //Serial.println((char)176);
         //servo_1.write(mov1););
         //Serial.println("no move");
         delay(200);
        } 
    }
    else if(x >=320 && x <= 360) 
    {
       int mov2 = map(x,360,250,0,180);

       if( mov2 > 50 ){
         Serial.println("Right");
         Serial.println(".......................");
        // Serial.println((char)176);
         //servo_2.write(mov2);
         delay(200);
    }
    } 
  
   if(y >=20 && y <= 60) 
    {
       int mov3 = map(y,0,60,90,180);
       if (mov3 > 160 ){
       Serial.println("Forward");
       Serial.println("..........");
       delay(200);
    } 
    }
   
    else if(y >=350 && y <= 360) 
    {
       int mov3 = map(y,360,300,90,0);

       if (mov3 > 90 ){
       Serial.print("Movement in Right = ");
       Serial.println(mov3);
    } 
    }
   /* if (c_up == 0){
      Up_val[0] = mov1;
      c_up = c_up + 1;
      Serial.println (Up_val[0]);
    }
    if (c_up == 1){
      Up_val[1] = mov1;
      c_up = c_up + 1;
      Serial.println (Up_val[1]);
    }
    if (c_up == 2 ){
      Up_move = Up_val[1] - Up_val[0];
       Up_val[0] = 0;
       Up_val[1] = 0;
       c_up = 0;
    }
    Serial.println(Up_move); */
    delay(20);
}
}
