#include <ezButton.h>

#define rpwm_pin1 10  //kiri
#define lpwm_pin1 9   //kiri

#define rpwm_pin2 5  //kanan
#define lpwm_pin2 6  //kanan

ezButton swkanan_depan_pin(3);
ezButton swkanan_blkg_pin(2);
ezButton swkiri_depan_pin(8);
ezButton swkiri_blkg_pin(12);

int pwm = 100;

void putarkanan(int lpwm_pin, int rpwm_pin) {
  analogWrite(lpwm_pin, pwm);
  analogWrite(rpwm_pin, 0);
}

void putarkiri(int lpwm_pin, int rpwm_pin) {
  analogWrite(lpwm_pin, 0);
  analogWrite(rpwm_pin, pwm);
}
void berhenti(int lpwm_pin, int rpwm_pin) {
  analogWrite(lpwm_pin, 0);
  analogWrite(rpwm_pin, 0);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(rpwm_pin1, OUTPUT);
  pinMode(lpwm_pin1, OUTPUT);
  pinMode(rpwm_pin2, OUTPUT);
  pinMode(lpwm_pin2, OUTPUT);
  Serial.begin(9600);  //start Serial in case we need to print debugging info
  //pinMode(ledPin, OUTPUT);

  swkanan_depan_pin.setDebounceTime(50);
  swkanan_blkg_pin.setDebounceTime(50);
  swkiri_depan_pin.setDebounceTime(50);
  swkiri_blkg_pin.setDebounceTime(50);
  putarkiri(lpwm_pin2, rpwm_pin2);
  putarkiri(lpwm_pin1, rpwm_pin1);
}

int swkanan_depan_val;
int swkanan_blkg_val;
int swkiri_depan_val;
int swkiri_blkg_val;


void loop() {
  // put your main code here, to run repeatedly:
  swkanan_depan_pin.loop();
  swkanan_blkg_pin.loop();
  swkiri_depan_pin.loop();
  swkiri_blkg_pin.loop();
  swkanan_depan_val = swkanan_depan_pin.getState();
  swkanan_blkg_val = swkanan_blkg_pin.getState();
  swkiri_depan_val = swkiri_depan_pin.getState();
  swkiri_blkg_val = swkiri_blkg_pin.getState();

  if (swkanan_depan_val == 1 && swkiri_depan_val == 1) {
    putarkiri(lpwm_pin1, rpwm_pin1);
    putarkiri(lpwm_pin2, rpwm_pin2);
  }

  else if(swkanan_blkg_val == 1 && swkiri_blkg_val == 1){
    putarkanan(lpwm_pin1, rpwm_pin1);
    putarkanan(lpwm_pin2, rpwm_pin2);
  }
}