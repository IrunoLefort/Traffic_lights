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

const int controller_1 = 9;
const int controller_2 = 10;

int digit1, digit2; // initialize individual digit to controll each segment

int start_count_1, start_count_2;

void setup() {
  Serial.begin(9600);
  Serial.println("Nbr de vehicules:");
  for (int i = 2; i < 9; i++)
  {
    pinMode(i, OUTPUT); // declare 0-9 th pin as output
  }
  pinMode(controller_1, OUTPUT); //declare 7 seg Digit1_horizontal pin as output
  pinMode(controller_2, OUTPUT);//declare 7 seg Digit2_horizontal pin as output
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
void counter(int start_of_countdown, int end_of_countdown=0) {
  if(digitalRead(check)){
    for (int j = start_of_countdown; j>=end_of_countdown; j--)//
    {
      //some latence during receiving data
      if(j == 0){
          digitalWrite(controller_1, LOW);
          digitalWrite(controller_2, LOW);
          dis(0);
      }
      else{
        digit2 = j / 10;
        digit1 = j % 10;
        for ( int k = 0; k<25; k++)//
        {
          digitalWrite(controller_1, HIGH);
          digitalWrite(controller_2, LOW);
          dis(digit1);
          delay(10);
          digitalWrite(controller_2, HIGH);
          digitalWrite(controller_1, LOW);
          dis(digit2);
          delay(10);
        }
      }
    }
  }
  else{
    traffic_stop();
    delay(10);
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


void receveing_data(){
  Serial.print('R');  //asking data from pic
  String data;
  while(Serial.available() == 0){}
      data = Serial.readStringUntil(",");
      Serial.println(data);
      data.trim();
      int vehicle_count1 = data.toInt();
      
      data = Serial.readStringUntil("\r\n");
      data.trim();
      Serial.println(data);
      int vehicle_count2 = data.toInt();
      
      if(vehicle_count1<10){
        start_count_1 = 5;
      }
      else if(vehicle_count1<20){
        start_count_1 = 8;
      }
      else{
        start_count_1 = 12;
      }
      if(vehicle_count2<10){
        start_count_2 = 5;
      }
      else if(vehicle_count2<20){
        start_count_2 = 8;
      }
      else{
        start_count_2 = 12;
      }
    
}

void traffic_stop(){
  //indicates an operating problem of the traffic light
  for(int i=0;i<2;i++){
      for(int j=0;j<3;j++){
        digitalWrite(lights[i][j], HIGH);
      }
    }
  digitalWrite(controller_1, HIGH);
  digitalWrite(controller_2, HIGH);
}

void traffic_light() {
  if(digitalRead(check)){
    start_count_1 = 6;
    start_count_2 = 6;
    receveing_data();
    green_on(lights[0], lights[1]);
    counter(start_count_1, 3);
    yellow_on(lights[0], lights[1]);
    counter(2, 0);
    receveing_data();
    green_on(lights[1], lights[0]);
    counter(start_count_2, 3);
    yellow_on(lights[1], lights[0]);
    counter(2, 0);
    }
  else{ 
    traffic_stop();
    delay(10);
  }
}
