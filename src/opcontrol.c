
#include "main.h"
#define LEFT 2
#define RIGHT 4
#define SHOULDER 7
#define ELBOW 5
#define CLAW 9
#define WRIST 8

#define ELBOWLIMIT 6
#define SHOULDERLIMIT 7
void operatorControl() {
   int loopCount = 0; //just a loop counter
	 int power, turn; //steering variables
   //clc variables
   int shoulderTarget = 0;
   int elbowTarget = 0;

   //ultrasonic variables
   int ultraRange = 0;
   int ultraFollow = 20;
   //line following variables
   int l;
   int c;
   int r;
   while(1) {


      //drive base control
      power = joystickGetAnalog(1, 2); // vertical axis on left joystick
      turn  = joystickGetAnalog(1, 1); // horizontal axis on left joystick
	    motorSet(LEFT,power + turn);
	    motorSet(RIGHT,power - turn);


			// simple claw and wrist
      motorSet(CLAW,joystickGetAnalog(1, 4));
      motorSet(WRIST,joystickGetAnalog(1, 3));


			// control shoulder
      if(joystickGetDigital(1, 6, JOY_UP)) {
        shoulderTarget=encoderGet(shoulderEncoder);
        motorSet(SHOULDER,127);
	    }
      else if(joystickGetDigital(1, 6, JOY_DOWN)) {
        shoulderTarget=encoderGet(shoulderEncoder);
        motorSet(SHOULDER,-127);
      }
      else{
        motorSet(SHOULDER,(shoulderTarget-encoderGet(shoulderEncoder))*2);
      }


      // control elbow
      if(joystickGetDigital(1, 5, JOY_UP)) {
        elbowTarget=encoderGet(elbowEncoder);
        motorSet(ELBOW,127);
      }
      else if(joystickGetDigital(1, 5, JOY_DOWN)) {
        elbowTarget=encoderGet(elbowEncoder);
        motorSet(ELBOW,-127);
      }
      else{
        motorSet(ELBOW,(elbowTarget-encoderGet(elbowEncoder))*2);
      }


      //ultrasonic 2d following

      if(joystickGetDigital(1, 8, JOY_RIGHT)){
        while(joystickGetDigital(1, 8, JOY_RIGHT)){

          delay(20);

          ultraRange=ultrasonicGet(noiseMaker);
          if(ultraRange<0 || ultraRange>70){
            motorSet(LEFT,50);
            motorSet(RIGHT,50);
          }
          else if(ultraRange>(ultraFollow+4)){
            motorSet(LEFT,35);
            motorSet(RIGHT,-35);
          }
          else if(ultraRange<(ultraFollow-4)){
            motorSet(LEFT,-35);
            motorSet(RIGHT,35);
          }

          else{
            motorStopAll();
          }
          if(joystickGetDigital(1, 7, JOY_UP)){ultraFollow++;}
          if(joystickGetDigital(1, 7, JOY_DOWN)){ultraFollow--;}
          printf("%d\n",ultraRange);
        }
      }

      //line following

      if(joystickGetDigital(1, 8, JOY_LEFT)){
        while(joystickGetDigital(1, 8, JOY_LEFT)){

          l=analogReadCalibrated(5);
          c=analogReadCalibrated(7);
          r=analogReadCalibrated(6);
          printf("%d ",l/1000);
          printf("%d ",c/1000);
          printf("%d \n",r/1000);

          if(c>l && c>r){
            motorSet(LEFT,-30);
            motorSet(RIGHT,30);
          }

          else if(l>c && l >r){
            motorSet(LEFT,-30);
            motorSet(RIGHT,-30);

          }

          else if(r > c && r >l){
            motorSet(LEFT,30);
            motorSet(RIGHT,30);
          }

          else{
            motorSet(LEFT,30);
            motorSet(RIGHT,-30);
          }
        }
      }
      // Homeing routine
      if(joystickGetDigital(1,8, JOY_UP)){
	       motorSet(SHOULDER,-50);
	       while(digitalRead(SHOULDERLIMIT)){}
	       encoderReset(shoulderEncoder);
	       motorSet(SHOULDER,50);
	       while(encoderGet(shoulderEncoder)< 215){}
	       encoderReset(shoulderEncoder);
	       motorSet(SHOULDER,0);


	       motorSet(ELBOW,-50);
	       while(digitalRead(ELBOWLIMIT)){}
	       encoderReset(elbowEncoder);
	       motorSet(ELBOW,50);
	       while(encoderGet(elbowEncoder)<295){}
	       encoderReset(elbowEncoder);
	       motorSet(ELBOW,0);
         elbowTarget=encoderGet(elbowEncoder);
         shoulderTarget=encoderGet(shoulderEncoder);
       }


       loopCount = loopCount + 1 ;
      //debug
       if(loopCount%25==0){
         printf("shoulder power: %d\n", (shoulderTarget-encoderGet(shoulderEncoder))*2);
         printf("shoulder encoder: %d\n", encoderGet(shoulderEncoder));
         printf("elbow power: %d\n", (elbowTarget-encoderGet(elbowEncoder))*2);
         printf("elbow encoder: %d\n", encoderGet(elbowEncoder));

         if(!digitalRead(SHOULDERLIMIT)){
	        printf("shoulder limit switch IS depressed \n");
         }
         else{
          printf("shoulder limit switch is NOT depressed \n");
         }
         if(!digitalRead(ELBOWLIMIT)){
          printf("elbow limit switch IS depressed \n");
         }
         else{
          printf("elbow limit switch is NOT depressed \n");
         }
         printf("\n");
         printf("%d\n",ultrasonicGet (noiseMaker));
         printf("\n");
       }

       delay(20);
     }
   }
