/*configuration PIC*/
#pragma config OSC = HS /* set osc mode to HS */ 
#pragma config WDT = OFF /* set watchdog timer off */ 
#pragma config LVP = OFF /* Low Voltage Programming Off */ 
#pragma config DEBUG = OFF /* Compile without extra Debug compile Code */ 
/***********************************************************************/

/*include header file*/
#include <p18f4520.h>
#include <delays.h>
#include <timers.h>
#include <adc.h>
/***********************************************************************/

/* macro definition*/
#define  right_sense  PORTCbits.RC1
#define  left_sense   PORTCbits.RC0
#define  green_forward   PORTCbits.RC4
#define  red_back   PORTCbits.RC5
#define  yellow_stop   PORTCbits.RC6
#define  white 0
#define  black 1
/***********************************************************************/
/*
void using_timer0(unsigned int i);
void turn_around();
void start_forward();
void turn_forward();
void turn_right();
void turn_left();
void end_forward();
void turn_back();
int adc_LDR();
*/
/***********************************************************************/
/* global variable*/
unsigned int start_forwardOK=0;// Flag bit,after cross start line set 1
unsigned int back_counter=0;   // count the number of times if the car turnning back 

/***********************************************************************/
  using_timer0(unsigned int i)// using timer0 for delay
{
	unsigned int j;
	unsigned int a;
	j=i;
	if(j>5){red_back=0;  	//if parameter of this fuction is bigger than 5,then turn on the yellow led
yellow_stop=1; 
green_forward=0;}
	for(;j--;j>0)			//according to the input parameter and change the delay time.
	{		
		OpenTimer0( TIMER_INT_OFF &/*configuration timer0*/
            T0_16BIT &
            T0_SOURCE_INT &
            T0_PS_1_4 );
		WriteTimer0( 12502 );		/*set initial value of timer0*/
	while(1)
	{
	a=ReadTimer0();					//read currenty value of timer0
	if(a>=0xFFFF)					//when the timer0 has overflowed ,reset the initial value.
	{WriteTimer0( 12502 );CloseTimer0(); break; }
	}
}	
}
/***********************************************************************/
int adc_LDR( void)
 { 
int read = 0;//define the varible which used to save the resistance  of LDR
   while (1)
   {
	   OpenADC(/*configuration ADC*/
	ADC_FOSC_32 & 
	ADC_RIGHT_JUST & 
	ADC_20_TAD,ADC_CH0 & 
	ADC_INT_OFF & 
	ADC_VREFPLUS_VDD & 
	ADC_VREFMINUS_VSS, 
	0b0000
		);	
	Delay10TCYx(5);
	SetChanADC(ADC_CH0);
	ConvertADC();
while(BusyADC( )); 
{
Delay10TCYx(5);	
read=ReadADC();
CloseADC();
}
if(read>=0x35f)/*the car will stop and turn on the yellow LED then return 1when the 
	resistance of LDR is bigger than this value.Otherwise,the car continue to run 
and turn off the yellow LED and return 0*/
{
	yellow_stop=1;
	
return 1;
}
else if(read<0x35f)
	{yellow_stop=0;	
	return 0;}
	   }
 }

/***********************************************************************/
 start_forward()// the fuction for crossing start line
{
	unsigned int r=2;//set the forward distance 	
	while(r--)
	{	
		red_back=0;  
		yellow_stop=0; 
		green_forward=1;
		PORTD=0x11;
		Delay10KTCYx(20);
		PORTD=0x99;
		Delay10KTCYx(20);
		PORTD=0x88;
		Delay10KTCYx(20);
		PORTD=0xaa;
		Delay10KTCYx(20);

		PORTD=0x22;
		Delay10KTCYx(20);
		PORTD=0x66; 
		Delay10KTCYx(20);
		PORTD=0x44; 
		Delay10KTCYx(20);
		PORTD=0x55;
		Delay10KTCYx(20);
	}	

	using_timer0(2); //stop
	
}

/***********************************************************************/
 turn_forward() //the fuction for turning forward
{
if (right_sense && left_sense )/*if two sensors detect the black line and return value of adc_LDR is 0,
	this fuction will be implemented.*/
{	
	if( adc_LDR()==0 )
	{		red_back=0;  
			yellow_stop=0; 
			green_forward=1;//turn on the green LED
			PORTD=0x11;
			Delay10KTCYx(20);
			PORTD=0x99;
			Delay10KTCYx(20);
			PORTD=0x88;
			Delay10KTCYx(20);
			PORTD=0xaa;
			Delay10KTCYx(20);
			PORTD=0x22;
			Delay10KTCYx(20);
			PORTD=0x66; 
			Delay10KTCYx(20);
			PORTD=0x44; 
			Delay10KTCYx(20);
			PORTD=0x55;
			Delay10KTCYx(20);
	}

}	
	


}
/***********************************************************************/
 turn_right()/*if left sensor detects  the white line and return value of adc_LDR is 0,
	this fuction will be implemented.*/
{if(left_sense==white  )
{
	if(  adc_LDR()==0)
	{	red_back=0;  
		yellow_stop=0; 
		green_forward=1;//turn on the green LED
		
		PORTD=0x01;
		using_timer0(2);
		PORTD=0x09;
		using_timer0(2);
		PORTD=0x08;
		using_timer0(2);
		PORTD=0x0a;
		using_timer0(2);	
		PORTD=0x02;
		using_timer0(2);
		PORTD=0x06; 
		using_timer0(2);
		PORTD=0x04; 
		using_timer0(2);
		PORTD=0x05;
		using_timer0(2);
	}
}
}
/***********************************************************************/
 turn_left()/*if right sensor detects the white line and return value of adc_LDR is 0,
	this fuction will be implemented.*/
{
if (right_sense==white )
{	
	if(adc_LDR()==0)
	{	red_back=0;  
		yellow_stop=0; 
		green_forward=1;//turn on the green LED
		
		PORTD=0x10;
		using_timer0(2);
		PORTD=0x90;
		using_timer0(2);
		PORTD=0x80;
		using_timer0(2);
		PORTD=0xa0;
		using_timer0(2);

		PORTD=0x20;
		using_timer0(2);
		PORTD=0x60; 
		using_timer0(2);
		PORTD=0x40; 
		using_timer0(2);
		PORTD=0x50;
		using_timer0(2);
	}
}
	
}
/***********************************************************************/
 turn_back()/*this fuction makes the car turn back*/
{
	unsigned int x=3;//set the turn back distance 
	

	while(x--)	
	{	red_back=1;   //turn on the red LED
		yellow_stop=0; 
		green_forward=0;
		PORTD=0x55;
		Delay10KTCYx(10);
		PORTD=0x44;
		Delay10KTCYx(10);
		PORTD=0x66;
		Delay10KTCYx(10);
		PORTD=0x22;
		Delay1KTCYx(10);

		PORTD=0xaa;
		Delay10KTCYx(10);
		PORTD=0x88; 
		Delay10KTCYx(10);
		PORTD=0x99; 
		Delay10KTCYx(10);
		PORTD=0x11;
		Delay10KTCYx(10);
	}


}
/***********************************************************************/
 turn_around()/*this fuction makes the car turn round*/
{
	unsigned int x=17;//set the turn around distance



	while(x--)	
	{
		red_back=1;  //turn on the red LED
		yellow_stop=0; 
		green_forward=0;
		PORTD=0x51;
		Delay10KTCYx(10);
		PORTD=0x49;
		Delay10KTCYx(10);
		PORTD=0x68;
		Delay10KTCYx(10);
		PORTD=0x2a;
		Delay10KTCYx(10);
		PORTD=0xa2;
		Delay10KTCYx(10);
		PORTD=0x86; 
		Delay10KTCYx(10);
		PORTD=0x94; 
		Delay10KTCYx(10);
		PORTD=0x15;
		Delay10KTCYx(10);
	}


}
/***********************************************************************/
end_forward()/*this fuction makes the car cross the end line after turning back*/
{
	unsigned int x=2;//set the forward distance

	while(x--)
	{	
		red_back=0;  
		yellow_stop=0; 
		green_forward=1;//turn on the green LED		
		PORTD=0x11;
		Delay10KTCYx(10);
		PORTD=0x99;
		Delay10KTCYx(10);
		PORTD=0x88;
		Delay10KTCYx(10);
		PORTD=0xaa;
		Delay10KTCYx(10);
		PORTD=0x22;
		Delay10KTCYx(10);
		PORTD=0x66; 
		Delay10KTCYx(10);
		PORTD=0x44; 
		Delay10KTCYx(10);
		PORTD=0x55;
		Delay10KTCYx(10);
	}
}

/***********************************************************************/
void main()
{ 
	LATD=0x00;/*configuration port*/
	TRISD=0x00;
	LATC=0x00;
	TRISC=0x03;
	TRISA = 0xFF; 
	while(1)
{
		if(start_forwardOK==0)//In order to make this fuction implemented agian.
		start_forward();	  //if start_forwardOK is equal to 1,the fuction"start_forward" can not be implemented 
		
		
		start_forwardOK=1;//assign 1 to the flat variable.
		
		turn_forward();//According the sensors and then the car judges whether it moves forward. 
		turn_left();//According the sensors and then the car judges whether it turn left.
		turn_right();//According the sensors and then the car judges whether it turn right
		
/************************** Car Backoff Algorithm********************************/	
/*Assume the two sensors both detect the white line,then programme will stop the car.
Next, check the states of sensors again.If one of the sensor is black,
then the car will turn to the direction which the sensor detect balck.
Otherwise,the car will turn back and then the counter will be incremented by 1 when the car turn back
Once the fuction of turning back is executed more than three times,that's mean the car is arriving the destination
So the car will turn around and clear the counter and go back to the start line.*/		
/***************************************************************************/		
		if(  (right_sense || left_sense)==0  )//if two sensors detect the white line.
{
			red_back=0;  
			yellow_stop=1; //stop and turn on the yellow LED
			green_forward=0;
			using_timer0(3);
}	
	
					
			//implemented turn back if two sensors detect the white line. 
			using_timer0(2);//stop the car
			if(  (right_sense || left_sense)==0  )//detect the states of two sensors again
			{		back_counter++;	turn_back();			  //the counter used to count the number of times if the car turnning back  
			}									  
					if(back_counter>3 )			  
												  /*if the car turns back more than three times,
													that's mean the car arrive the destination	*/		
					{red_back=1;  				  //turn on the red LED
					yellow_stop=0; 
					green_forward=0;
					turn_around(); 				 //turn around the car
					back_counter=0;				 //clean the counter
				    }
								  
							 //clean the counter
			
			//detect the counter.



}
}