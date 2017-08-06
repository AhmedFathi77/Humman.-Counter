/*
 * main.c
 *
 *  Created on: Aug 5, 2017
 *      Author: leenovoz510
 */
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define RS 5
#define RW 6
#define EN 7

void send_command(char command);
void send_data(char data);
void clear_screan();
void Print_On_Screen(char *str);
void Go_to(char x ,char y);
void LCD_init();
volatile  int Total_OVF;
int Secodes_Counert = 0;
char dispay_seconds[16];

ISR(TRIMER0_OVF_vect){
	Total_OVF++ ;
}

void main(void){
	 DDRC = 0xff;
	 DDRD = 0xff;
	 DDRA = 0xff;
	 _delay_ms(10);
	 LCD_init();
	 _delay_ms(10);
	 clear_screan();

	 TIMER0_Init();
char counter[5] ;
	while(1){
		Go_to(1,1);
		Print_On_Screen("Object : ");
		itoa(TCNT0, counter, 10);
		Go_to(1,9);
		Print_On_Screen(counter);
		/*
		if(Total_OVF >= 12){
			if(TCNT0 >=35 ){
				PORTA ^= (1<<1) | (1<<7);    // toggles the led
				TCNT0 = 0;            // reset counter
				Total_OVF = 0;
			}
		}
		*/
	}
}

void TIMER0_Init(){
	TCCR0 |= 0x06; // 128 Prescaler
	TCNT0 = 0;
	/*
	TIMSK |= (1<<TOIE0);
	sei();
	Total_OVF = 0;
	*/
}
void send_command(char commnd){
	PORTC = commnd;
	PORTD &=~ (1<<RS);
	PORTD &=~(1<<RW);
	PORTD |= (1<<EN);
	_delay_ms(50);
	PORTD &=~(1<<EN);
}
void send_data(char data){
	PORTC = data;
	PORTD |= (1<<RS);
	PORTD &= ~(1<<RW);

	PORTD |= (1<<EN);
	_delay_ms(50);
	PORTD &=~(1<<EN);
}
void clear_screan(){
	send_command(0x01);
	_delay_ms(10);
}
void Print_On_Screen(char *str){
	int i=0;
	while(str[i] != 0){
		send_data(str[i]);
		i++;
	}
}
void Go_to(char x ,char y){
	char firstAddress[] = {0x80,0xC0,0x94,0xD4};
	send_command(firstAddress[y-1] + x-1);
		_delay_ms(10);
}
void LCD_init(){
	send_command(0x38);  // 8-bit mode
	send_command(0x0C);
	send_command(0x06);
	send_command(0x80);
	_delay_ms(10);
}


