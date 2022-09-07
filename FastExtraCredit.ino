// Base code.
// 
// *  NOTE: this code will do only three things:
// *    --rotate one wheel, and 
// *    --blink the right front mainboard LED.
// *    
// *  You will need to add more code to
// *  make the car do anything useful. 
// 

#include <ECE3.h>

uint16_t sensorValues[8]; // right -> left, 0 -> 7
 int r = 70;
 int l = 70;
const int left_nslp_pin = 31; // nslp ==> awake & ready for PWM
const int left_dir_pin = 29;
const int left_pwm_pin = 40;
int leftSpd;
int rightSpd;
const int LED_RF = 41;
const int right_nslp_pin = 11; // nslp ==> awake & ready for PWM
const int right_dir_pin = 30;
const int right_pwm_pin = 39;

int wheelSpd = 80;
int distance = 800;
///////////////////////////////////
void setup() {
    // put your setup code here, to run once:
    pinMode(left_nslp_pin, OUTPUT);
    pinMode(left_dir_pin, OUTPUT);
    pinMode(left_pwm_pin, OUTPUT);
    pinMode(right_nslp_pin, OUTPUT);
    pinMode(right_dir_pin, OUTPUT);
    pinMode(right_pwm_pin, OUTPUT);

    digitalWrite(right_nslp_pin, HIGH);
    digitalWrite(left_nslp_pin, HIGH);
    //  digitalWrite(left_nslp_pin,LOW);

    pinMode(LED_RF, OUTPUT);

    ECE3_Init();

    // set the data rate in bits/second for serial data transmission

    resetEncoderCount_left();
    resetEncoderCount_right();
    delay(2000); //Wait 2 seconds before starting 

}

float error_prev = 0;
float error_dif = 0;
float K_d = -6.7586679 *5;
float K_p = -5;
bool DoDonut = false;
bool DidDonut = false;
bool FirstCrossPiece = false;
bool SecondCrossPiece = false;
int counter = 0;
int whitecounter = 0;

void changeWheelSpeeds(int initialLeftSpd, int finalLeftSpd, int initialRightSpd, int finalRightSpd) {
    /*
     *   This function changes the car speed gradually (in about 30 ms) from initial
     *   speed to final speed. This non-instantaneous speed change reduces the load
     *   on the plastic geartrain, and reduces the failure rate of the motors.
     */
    int numSteps = 5;
    int pwmLeftVal = initialLeftSpd; // initialize left wheel speed 
    int pwmRightVal = initialRightSpd;  // initialize right wheel speed 
    int deltaLeft = (finalLeftSpd - initialLeftSpd) / numSteps; // left in(de)crement
    int deltaRight = (finalRightSpd - initialRightSpd) / numSteps;  // right in(de)crement

    for (int k = 0; k < numSteps; k++) {
        pwmLeftVal = pwmLeftVal + deltaLeft;
        pwmRightVal = pwmRightVal + deltaRight;
        analogWrite(left_pwm_pin, pwmLeftVal);
        analogWrite(right_pwm_pin, pwmRightVal);
        delay(60);
    } // end for int k
} // end void changeWheelSpeeds

int average()  //average pulse count
{
    int getL = getEncoderCount_left();
    int getR = getEncoderCount_right();
    //  Serial.print(getL);Serial.print("\t");Serial.println(getR);
    return ((getEncoderCount_left() + getEncoderCount_right()) / 2);
}

void loop()
{   
    if (getEncoderCount_left() < 600) {
      l = 60;
      r = 60;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 600 && getEncoderCount_left() < 2000) {
      l = 180;
      r = 180;
      K_d = -6.7586679 *4;
      K_p = -4;
    }
    if (getEncoderCount_left() > 2000 && getEncoderCount_left() < 3800) {
      l = 85;
      r = 85;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 4000 && getEncoderCount_left() < 5400) {
      l = 180;
      r = 180;
      K_d = -6.7586679 *4;
      K_p = -4;
    }
    if (getEncoderCount_left() > 5400 && getEncoderCount_left() < 6000) {
      l = 100;
      r = 100;
      K_d = -6.7586679 *5;
      K_p = -5;
    }

    if (getEncoderCount_left() > 6000 && getEncoderCount_left() < 6100) {
      l = 85;
      r = 85;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 6100 && getEncoderCount_left() < 6800) {
      l = 70;
      r = 70;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 6800 && getEncoderCount_left() < 7200) { //changed
      l = 90;
      r = 90;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 7200 && getEncoderCount_left() < 8400) { //changed
      l = 190;
      r = 190;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 8400 && getEncoderCount_left() < 10100) {
      l = 90;
      r = 90;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 10100 && getEncoderCount_left() < 11200) {
      l = 190;
      r = 190;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 11200 && getEncoderCount_left() < 11600) {
      l = 120;
      r = 120;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if (getEncoderCount_left() > 11600) {
      l = 80;
      r = 80;
      K_d = -6.7586679 *5;
      K_p = -5;
    }
    if ((((sensorValues[2] + sensorValues[3] + sensorValues[4] + sensorValues[5])/4) < 700) && (whitecounter > 3))
    {
      whitecounter++;
      changeWheelSpeeds(leftSpd, 0, rightSpd, 0);
      while(true)
      {
        analogWrite(left_pwm_pin,0);
        analogWrite(right_pwm_pin,0);
      }
    }
    else
    {
      whitecounter = 0;
    }

    if (((sensorValues[0] + sensorValues[1] + sensorValues[2] + sensorValues[3] + sensorValues[4] + sensorValues[5] + sensorValues[6] + sensorValues[7])/8) > 2000)
    {
      counter++;
      if((FirstCrossPiece == false) && (counter > 2))
      {
       FirstCrossPiece = true;
       counter = 0;
      }

      if((SecondCrossPiece == false) && (counter > 2))
      {
       SecondCrossPiece = true;
       counter = 0;
      }  

      if((DoDonut == false) && (counter > 2))
      {
       changeWheelSpeeds(leftSpd, 0, rightSpd, 0);
       if(DidDonut == true)
         {
          while(true)
          {
            analogWrite(left_pwm_pin,0);
            analogWrite(right_pwm_pin,0);
          }
         }
       digitalWrite(left_dir_pin,LOW);
       digitalWrite(right_dir_pin, HIGH);
       int in = getEncoderCount_left();
         while((getEncoderCount_left() - in) < 240)
         {
          analogWrite(left_pwm_pin,170);
          analogWrite(right_pwm_pin,170);
         }
         changeWheelSpeeds(leftSpd, 0, rightSpd, 0);
         digitalWrite(left_dir_pin,LOW);
         digitalWrite(right_dir_pin,LOW);
         changeWheelSpeeds(0, 70, 0, 70);
         FirstCrossPiece = false;
         SecondCrossPiece = false;
         DoDonut = false;
         DidDonut = true;
         counter = 0;
      }
   }
   else
   {
    counter = 0;  
   }

    ECE3_read_IR(sensorValues); // get sensor readings
    float error = ((sensorValues[7] - sensorValues[0]) * 15 + (sensorValues[6] - sensorValues[1]) * 14 + (sensorValues[5] - sensorValues[2]) * 12 + (sensorValues[4] - sensorValues[3]) * 8) / 8;
    // calculate error
    // either donut, finish, cross piece or phantom crosspiece
    error_dif = error - error_prev;
    error_prev = error;
    leftSpd = l + (K_p * error / 255) + (K_d * error_dif / 255);
    rightSpd = r - (K_p * error / 255) - (K_d * error_dif / 255);
    analogWrite(left_pwm_pin, leftSpd);
    analogWrite(right_pwm_pin, rightSpd);
}
