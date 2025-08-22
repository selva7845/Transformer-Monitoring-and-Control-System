#include <pic.h>
#include <string.h>

#define RS RB0
#define RW RB1
#define EN RB2
#define LED RB3
#define C0 RB5
#define C1 RB6
#define C2 RB7
#define R0 RC0
#define R1 RC1
#define R2 RC2
#define R3 RC3

int a=0,b=0,c=0,d=0,e=0,f=0,g=0;
int a1=0,b1=0,c1=0,d1=0,e1=0,f1=0,g1=0;
void lcd_init();
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_puts(const char *str);
void get_key();
unsigned int digit_count = 0;
char number[6] = "";  
unsigned char passing = 0; 

void delay(unsigned int x) {
    while(x--);
}

void lcd_cmd(unsigned char cmd) {
    PORTD = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    delay(500);
    EN = 0;
}

void lcd_data(unsigned char data) {
    PORTD = data;
    RS = 1;
    RW = 0;
    EN = 1;
    delay(500);
    EN = 0;
}

void lcd_init() {
    lcd_cmd(0x01);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
}

void lcd_puts(const char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void data(const char *n) {
    while (*n) {
        TXREG = *n++;
        delay(5000);
    }
}

void get_key() 
{
    if (digit_count >= 5) return;

    C0 = 1; C1 = 0; C2 = 0;
    if (R0 == 1) { while (R0 == 1); lcd_data('1'); number[digit_count++] = '1'; }
    if (R1 == 1) { while (R1 == 1); lcd_data('4'); number[digit_count++] = '4'; }
    if (R2 == 1) { while (R2 == 1); lcd_data('7'); number[digit_count++] = '7'; }
    if (R3 == 1) { while (R3 == 1); lcd_data('*'); number[digit_count++] = '*'; }

    C0 = 0; C1 = 1; C2 = 0;
    if (R0 == 1) { while (R0 == 1); lcd_data('2'); number[digit_count++] = '2'; }
    if (R1 == 1) { while (R1 == 1); lcd_data('5'); number[digit_count++] = '5'; }
    if (R2 == 1) { while (R2 == 1); lcd_data('8'); number[digit_count++] = '8'; }
    if (R3 == 1) { while (R3 == 1); lcd_data('0'); number[digit_count++] = '0'; }

    C0 = 0; C1 = 0; C2 = 1;
    if (R0 == 1) { while (R0 == 1); lcd_data('3'); number[digit_count++] = '3'; }
    if (R1 == 1) { while (R1 == 1); lcd_data('6'); number[digit_count++] = '6'; }
    if (R2 == 1) { while (R2 == 1); lcd_data('9'); number[digit_count++] = '9'; }
    if (R3 == 1) { while (R3 == 1); lcd_data('#'); number[digit_count++] = '#'; }
}

void lcd_num() {
    lcd_cmd(0xC0);
    digit_count = 0;
    for (int i = 0; i < 6; i++) {
        number[i] = '\0';
    }
    while (digit_count < 5) {
        get_key();
    }
    number[5] = '\0';
}

void main()
{
 const char pass1[] = "1234*";
 const char pass2[] = "5577#";
 const char pass3[] = "7755#";

 TRISA = 0X03; PORTA = 0X00;
 TRISB = 0X00; PORTB = 0X00;
 TRISC = 0X0f; PORTC = 0X0f;
 TRISD = 0X00; PORTD = 0X00;
 ADCON0 = 0X40;
 ADCON1 = 0X8E;

 TXSTA = 0X24;
 RCSTA = 0X90;
 SPBRG = 25;

 lcd_init();
 LED = 1;

while (1) 
{
   
//1
CHS2=0;CHS1=0;CHS0=0;  
ADON=1;
ADCON0=ADCON0|(0X04);
delay(1000);

a=ADRESH<<8;
delay(1000);
a=a+ADRESL;
delay(1000);

b=a/1000;
c=a%1000;
d=c/100;
e=c%100;
f=e/10;
g=e%10;
lcd_cmd(0xc0);
lcd_data(b+48);
lcd_data(d+48);
lcd_data(f+48);
lcd_data(g+48);
delay(2000);

//2
CHS2=0;CHS1=0;CHS0=1;
ADON=1;
ADCON0=ADCON0|(0X04);
delay(1000);

a1=ADRESH<<8;
delay(1000);
a1=a1+ADRESL;

delay(1000);
b1=a1/1000;
c1=a1%1000;
d1=c1/100;
e1=c1%100;
f1=e1/10;
g1=e1%10;
lcd_cmd(0xc8);
lcd_data(b1+48);
lcd_data(d1+48);
lcd_data(f1+48);
lcd_data(g1+48);
delay(2000);
  
if (passing == 0)
{
 if (a <= 50) 
 {
  lcd_cmd(0x80);
  lcd_puts("CURRENT ");
  lcd_cmd(0x88);
  lcd_puts("VOLTAGE ");
  } 
   else 
   {
    LED = 0;
    passing = 1; 
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_puts("CURRENTLOAD HIGH");
    TXREG = 0X0D;
    data("CURRENT LOAD HIGH");
   }
  if (a1 <= 240) 
  {
  lcd_cmd(0x80);
  lcd_puts("CURRENT ");
  lcd_cmd(0x88);
  lcd_puts("VOLTAGE ");
  } 
   else 
   {
    LED = 0;
    passing = 1; 
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_puts("VOLTAGE HIGH");
    TXREG = 0X0D;
    data("VOLTAGE HIGH");
   }
 }  

if (passing == 1) 
{
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_puts("ENTER CALLPIN:");
 lcd_num();
 if (strcmp(number, pass1) == 0) 
 {
  lcd_cmd(0x01);
  lcd_puts("CALLING...");
  TXREG = 0X0D;
  data("CONFIRM THE OTP:5577");
  passing = 2;
  }
 }

if (passing == 2) 
{
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_puts("Enter OTP:");
 lcd_num();
 if (strcmp(number, pass2) == 0) 
 {
  lcd_cmd(0x01);
  lcd_puts("WORK STARTED");
  TXREG = 0X0D;
  data("WORK IS COMPLETED ENTER OTP:7755");
  passing = 3;
  }
 }

if (passing == 3) 
{
  lcd_cmd(0x01);
  lcd_cmd(0x80);
  lcd_puts("Enter Final OTP:");
  lcd_num();
  if (strcmp(number, pass3) == 0) 
  {
   lcd_cmd(0x01);
   lcd_puts("WORK COMPLETED");
   TXREG = 0X0D;
   data("WORK COMPLETED");
   LED = 1;
   passing = 0; 
   }
}

}
}
