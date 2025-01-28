//display 7 segment 2 digits cathode

const int check = 13;
// pin lights
const int red_vertical=A0;
const int yellow_vertical=A1;
const int green_vertical=A2;
const int red_horizontal=A3;
const int yellow_horizontal=A4;
const int green_horizontal=A5;

const int lights[2][3] = {
  {red_vertical, yellow_vertical, green_vertical},
  {red_horizontal, yellow_horizontal, green_horizontal}
};

//display
//Prepare binary array for all 7 segment to turn on 7 segment at position of a,b,c,d,e,f,g
int digit[10] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111};

const int controller_1h = 9;
const int controller_2h = 10;
const int controller_1v = 11;
const int controller_2v = 12;

int digit1, digit2; // initialize individual digit to controll each segment

int controllers1[2] = {controller_1h, controller_2h};
int controllers2[2] = {controller_1v, controller_2v};


void setup() {
  for (int i = 2; i < 9; i++)
  {
    pinMode(i, OUTPUT); // declare 0-9 th pin as output
  }
  pinMode(controller_1h, OUTPUT); //declare 7 seg Digit1_horizontal pin as output
  pinMode(controller_2h, OUTPUT);//declare 7 seg Digit2_horizontal pin as output
  pinMode(controller_1v, OUTPUT); //declare 7 seg Digit1_vertical pin as output
  pinMode(controller_2v, OUTPUT);//declare 7 seg Digit2_vertical pin as output
  for (int i=0;i<2;i++){
    for(int j=0;j<3;j++){
      pinMode(lights[i][j], OUTPUT);
    }
  }
  traffic_stop();
  delay(500);
}

void loop(){
  traffic_light();
  delay(10);
}

//counter
void counter(int segment1[], int segment2[], int start_of_countdown, int end_of_countdown=0) {
  for (int j = start_of_countdown; j>=end_of_countdown; j--)//
  {
    digit2 = j / 10;
    digit1 = j % 10;
    for ( int k = 0; k<50; k++)//
    {
      digitalWrite(segment1[0], HIGH);
      digitalWrite(segment1[1], LOW);
      digitalWrite(segment2[0], HIGH);
      digitalWrite(segment2[1], LOW);
      dis(digit1);
      delay(10);
      digitalWrite(segment1[1], HIGH);
      digitalWrite(segment1[0], LOW);
      digitalWrite(segment2[1], HIGH);
      digitalWrite(segment2[0], LOW);
      dis(digit2);
      delay(10);
    }
  }
}

void dis(int num)
{
  for (int i = 2; i < 9; i++)
  {
    digitalWrite(i, bitRead(digit[num], i - 2));
  }
}

//lights
void red_on(int light1[3], int light2[3]){
  digitalWrite(light1[0], HIGH);
  digitalWrite(light1[1], LOW);
  digitalWrite(light1[2], LOW);
  digitalWrite(light2[2], HIGH);
  digitalWrite(light2[1], LOW);
  digitalWrite(light2[0], LOW);
}

void green_on(int light1[3], int light2[3]){
  digitalWrite(light1[2], HIGH);
  digitalWrite(light1[1], LOW);
  digitalWrite(light1[0], LOW);
  digitalWrite(light2[0], HIGH);
  digitalWrite(light2[1], LOW);
  digitalWrite(light2[2], LOW);
}

void yellow_on(int light1[3], int light2[3]){
  digitalWrite(light1[0], LOW);
  digitalWrite(light1[2], LOW);
  digitalWrite(light2[0], HIGH);
  digitalWrite(light2[1], LOW);
  digitalWrite(light2[2], LOW);
  digitalWrite(light1[1], HIGH);
}

void traffic_stop(){
  //indicates an operating problem of the traffic light
  for(int i=0;i<2;i++){
      for(int j=0;j<3;j++){
        digitalWrite(lights[i][j], HIGH);
      }
    }
  digitalWrite(controller_1v, LOW);
  digitalWrite(controller_2v, LOW);
  digitalWrite(controller_1h, LOW);
  digitalWrite(controller_2h, LOW);
}

void traffic_light() {
  if(digitalRead(check)){
    green_on(lights[0], lights[1]);
    counter(controllers1, controllers2, 10, 3);
    yellow_on(lights[0], lights[1]);
    counter(controllers1, controllers2, 2, 0);
    green_on(lights[1], lights[0]);
    counter(controllers2, controllers1, 5, 3);
    yellow_on(lights[1], lights[0]);
    counter(controllers2, controllers1, 2, 0);
    }
  else{ 
    traffic_stop();
    delay(10);
  }
}