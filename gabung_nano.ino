#include <ezButton.h>
#include <Servo.h>
Servo myservo;  
int pos = 0;

#define rpwm_pin1 10 //kiri
#define lpwm_pin1 9 //kiri

#define rpwm_pin2 5 //kanan
#define lpwm_pin2 6 //kanan
 
////#define enable 7
//#define potensio_pin1 A1 //kiri 
//#define potensio_pin2 A0 //kanann 


byte pin = 11; // Kanan Pixhawk
byte pin2 = 7; // Kiri Pixhawk

int gas_kanan;
int gas_kiri;


unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 100;  //the value is a number of millisecond


ezButton swkanan_depan_pin(3);
ezButton swkanan_blkg_pin(2);
ezButton swkiri_depan_pin(8);
ezButton swkiri_blkg_pin(12);

int pwm = 255;

void putarkanan(int lpwm_pin, int rpwm_pin)
{
  analogWrite(lpwm_pin, pwm);
  analogWrite(rpwm_pin, 0);
}

void putarkiri(int lpwm_pin, int rpwm_pin)
{
  analogWrite(lpwm_pin, 0);
  analogWrite(rpwm_pin, pwm);
}
void berhenti(int lpwm_pin, int rpwm_pin)
{
  analogWrite(lpwm_pin, 0);
  analogWrite(rpwm_pin, 0);

}

void setup() {
    myservo.attach(4);
  // put your setup code here, to run once:
  pinMode(rpwm_pin1, OUTPUT);
  pinMode(lpwm_pin1, OUTPUT);
  pinMode(rpwm_pin2, OUTPUT);
  pinMode(lpwm_pin2, OUTPUT);
  pinMode(pin, INPUT);
  pinMode(pin2, INPUT);
  Serial.begin(9600);  //start Serial in case we need to print debugging info
  //pinMode(ledPin, OUTPUT);

  swkanan_depan_pin.setDebounceTime(50);
  swkanan_blkg_pin.setDebounceTime(50);
  swkiri_depan_pin.setDebounceTime(50);
  swkiri_blkg_pin.setDebounceTime(50);
  putarkiri(lpwm_pin2, rpwm_pin2);
  putarkiri(lpwm_pin1, rpwm_pin1);

  startMillis = millis();  //initial start time

}

String input = " ", input_tmp = " ";
int state = 0;
int swkanan_depan_val;
int swkanan_blkg_val;
int swkiri_depan_val;
int swkiri_blkg_val;

void initLimit() {
  swkanan_depan_pin.loop();
  swkanan_blkg_pin.loop();
  swkiri_depan_pin.loop();
  swkiri_blkg_pin.loop();
  swkanan_depan_val = swkanan_depan_pin.getState();
  swkanan_blkg_val = swkanan_blkg_pin.getState();
  swkiri_depan_val = swkiri_depan_pin.getState();
  swkiri_blkg_val = swkiri_blkg_pin.getState();
}
//void waitC(int tt) {
//  Serial.print("tunggu");
//  startMillis = currentMillis = millis();
//  while (currentMillis - startMillis <= tt) {
//    currentMillis = millis();
//    initLimit();
//    if (swkanan_depan_val == 0 && swkanan_blkg_val == 0 && swkiri_depan_val == 0 && swkiri_blkg_val == 0)
//      break;
//  }
//  Serial.println("--OK");
//}
int o = 0;

int gas_state_kiri_lama = 0;
int gas_state_kanan_lama = 0;

int gas_state_kiri = 0;
int gas_state_kanan = 0;
int swait=0;

int gas_max=0;
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(o++);
  gas_kanan = pulseIn(pin, HIGH);

  gas_kiri = pulseIn(pin2, HIGH);
// Serial.print(gas_kanan);
// Serial.print("\t");
// Serial.println(gas_kiri);
  gas_max=gas_kanan;
  if(gas_max<gas_kiri)gas_max = gas_kiri;
  int sel_1= gas_max-1450;
  pos = sel_1 * (185.0 / 450.0);
  if(pos<0)pos=0; 
  myservo.write(pos); 
  
  if(gas_max < gas_kiri)gas_max = gas_kiri;
//Serial.print(gas_kanan);
//Serial.print(" || ");
//Serial.println(gas_kiri);

  initLimit();
//Serial.println("test2");
  if ( currentMillis-startMillis >= 300) {
    if (swkanan_depan_val == 1 || swkanan_blkg_val == 1) {
      berhenti(lpwm_pin2, rpwm_pin2);
Serial.println("kanan_berhenti");
    }

    if (swkiri_depan_val == 1 || swkiri_blkg_val == 1) {
      berhenti(lpwm_pin1, rpwm_pin1);

    }
  }
swait=0;
//////////////////////////////kiri//////////////////////////////
  if (gas_kiri < 1515) {
    gas_state_kiri = 1;
  }
  else if (gas_kiri >= 1515) {
    gas_state_kiri = 2;
  }

  if (gas_state_kiri_lama != gas_state_kiri) {
    swait=1;
    if (gas_state_kiri == 1)
      putarkiri(lpwm_pin1, rpwm_pin1);
    else if (gas_state_kiri == 2)
      putarkanan(lpwm_pin1, rpwm_pin1);
  }
  gas_state_kiri_lama = gas_state_kiri;
///////////////////////////////////////////////////////////////////////

//////////////////////////////kanan//////////////////////////////
  if (gas_kanan < 1515) {
    gas_state_kanan = 1;
  }
  else if (gas_kanan >= 1515) { 
    gas_state_kanan = 2;
  }

  if (gas_state_kanan_lama != gas_state_kanan) {
    swait=1;
    if (gas_state_kanan == 1){
      putarkiri(lpwm_pin2, rpwm_pin2);
    //Serial.println("kanan1");
    }
    else if (gas_state_kanan == 2){
      putarkanan(lpwm_pin2, rpwm_pin2);
      //Serial.println("kanan2");
    }
  }
  gas_state_kanan_lama = gas_state_kanan;
///////////////////////////////////////////////////////////////////////

if(swait==1) {
  startMillis = currentMillis = millis();
} currentMillis = millis();


delay(10);
}
