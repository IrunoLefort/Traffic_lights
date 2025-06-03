unsigned char cnt1, cnt2;
char my_data[50], data1[10], data2[10];
char received;
unsigned char temp_PORTB;

void interrupt() {
  if (INTCON.RBIF) {
      unsigned char new_PORTB = PORTB;
      if(((temp_PORTB ^ new_PORTB) & 0x10) != 0){  //interruption on RB4
        if((PORTB & 0x10) != 0) cnt1++;
      }
      else if(((temp_PORTB ^ new_PORTB) & 0x20) != 0){  //interruption on RB5
        if((PORTB & 0x20) != 0) cnt1--;
      }
      else if(((temp_PORTB ^ new_PORTB) & 0x40) != 0){  //interruption on RB6
        if((PORTB & 0x40) != 0) cnt2++;
      }
      else if(((temp_PORTB ^ new_PORTB) & 0x80) != 0){  //interruption on RB7
        if((PORTB & 0x80) != 0) cnt2--;
      }

      temp_PORTB = new_PORTB;
      INTCON.RBIF = 0;
  }
}

void sending_data(){
 my_data[0] = '\0'; // on vide la chaine avant concatenation
 strcat(my_data, data1);
 strcat(my_data, ",");
 strcat(my_data, data2);
 strcat(my_data, "\r\n");
 UART1_Write_Text(my_data);
}

void main(){
  OPTION_REG.INTEDG = 1;  //front montant de RB0
  INTCON = 0x88;      //0b10001000
  TRISB = 0xF0;    //0b11110000;
  cnt1 = 1;
  cnt2 = 1;
  PORTB.RB1 = 0;
  PORTB.RB2 = 0;
  temp_PORTB = PORTB;
  UART1_Init(9600);
  delay_ms(200);
   while(1){
        IntToStr(cnt1, data1);
        IntToStr(cnt2, data2);
        if (UART1_Data_Ready() == 1) {
           received = UART1_Read();
           //UART1_Write_Text(received);
           if(received == 'R'){
              sending_data();
           }
         }
        if(cnt1 > 10) PORTB.RB1 = 1;
        else  PORTB.RB1 = 0;
        if(cnt2 > 10) PORTB.RB2 = 1;
        else  PORTB.RB2 = 0;
        received = '\0';
        delay_ms(100);
   }
}