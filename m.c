/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.6 Evaluation
Automatic Program Generator
© Copyright 1998-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Chip type               : ATmega164A
Program type            : Application
AVR Core Clock frequency: 20.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega164a.h>
#include <delay.h>

#define led         PORTD.6
#define sensor_trig PORTC.6     // pinul de generare semnal TRIGGER pentru senzorul ultrasonic
#define motorA      PORTB.6     // pinul de generare de semnal pentru motor
#define motorB      PORTB.7     // pinul de generare de semnal pentru motor

unsigned int time_elapsed   = 0;    // durata masurata pentru pulsul primit de la senzor
int computed_distance       = 0;    // valoarea distantei masurate de senzor
int warning_action          = 50;   // distanta minima sub care se ridica bariera
int barrier_state           = 0;    // starea curenta a barierei; 0 - jos; 1 - sus


// Timer1 output compare A interrupt service routine

// Intrerupere pe timer1 generata la fiecare 0.42 s pentru a genera semnalul TRIG
// pentru calculul distantei -> aceasta se calculeaza o data la 0.42 s
interrupt [TIM1_COMPA] void timer1_compa_isr(void)      
{
    // Generarea impulsului: 10ms pe high, urmand sa se intoarca pe low
    sensor_trig = 1;
    delay_ms(10);
    sensor_trig = 0;
    
    // Se reseteaza TIMER1 pentru a putea masura timpul trecut pana la primirea
    // raspunsului de catre senzor
    TCNT1H = 0x00;
    TCNT1L = 0x00;
}

// External Interrupt 0 service routine

// Intrerupere externa declansata de frontul descrescator
// al semnalului primit ca raspuns de catre senzorul ultrasonic
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    // Se calculeaza distanta masurata de senzor
    // Se aduna valoarea din cei 2 registrii ai timerului pentru a obtine timul trecut
    // Se inmulteste cu T = 52us, asta fiind perioada de numarare a timerului pentru frecventa de 19.531kHz
    // La final se inmulteste cu viteza sunetului si imparte la 2 deoarece unda ultrasonica parcurge de doua ori drumul
    time_elapsed = 52*(TCNT1H << 8| TCNT1L);
    computed_distance = time_elapsed*0.017;                 
}



void main(void)
{    
// Crystal Oscillator division factor: 1   //20MHz
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=Out 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=0 
PORTB=0x00;
DDRB=0xC0;    //PB6 si PB7 iesiri

// Port C initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x40;    //activare iesire pe pinul PC6 pentrut trigger-ul senzorului ultrasonic

// Port D initialization
// Func7=In Func6=Out Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=0 State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;   
DDRD=0xC0;     //LED conectat pe pinul D6, iesire difuzor pe D7

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
TCCR0A=0x00;
TCCR0B=0x00;                         
TCNT0=0x00;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 19.531 kHz  // Fclk/1024
// Mode: CTC top=OCR1A
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: On
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x0D;      
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x20;     //configurare timer1 cu intrerupere la 0.42 secunde  8000/19530       
OCR1AL=0x10;
OCR1BH=0x00;
OCR1BL=0x00;

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1:  Off
// INT1  Off
// INT2: Off
// Interrupt on any change on pins PCINT0-7: Off
// Interrupt on any change on pins PCINT8-15: Off
// Interrupt on any change on pins PCINT16-23: Off
// Interrupt on any change on pins PCINT24-31: Off
EICRA=0x02;
EIMSK=0x01;
EIFR=0x01;
PCICR=0x00;

// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=0x00;          

// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=0x02;

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=0x00;          

// USART0 initialization
// USART0 disabled
UCSR0B=0x00;          

// USART1 initialization
// USART1 disabled
UCSR1B=0x00;              

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
ADCSRB=0x00;              
DIDR1=0x00;

// ADC initialization
// ADC disabled          
ADCSRA=0x00;

// SPI initialization
// SPI disabled
SPCR=0x00;               

// TWI initialization
// TWI disabled           
TWCR=0x00;

// Global enable interrupts
#asm("sei")

while (1)
    {  
        // Daca s-a atins distanta minima pentru a actiona bariera
        // si aceasta este jos se ridica bariera
        if (computed_distance < warning_action && barrier_state == 0) {
            // Aprindem led-ul
            led = 1;
            // Modificam starea interna a barierei
            barrier_state = 1;
            // Trimitem semnal catre driver-ul de motor pentru a-l 
            // putea actiona
            motorA = 1;
            motorB = 0;
            // Il actionam timp de 10ms
            delay_ms(10);
            // Oprim rotirea motorului
            motorA = 0;
            motorB = 0;
        }
        
        // Daca distanta minima pentru a actiona bariera nu mai este
        // indeplinita (nu mai exista un obstacol in fata senzorului)
        // si bariera este ridicata, coboram bariera
        if (computed_distance > warning_action && barrier_state == 1) {
            // Stingem led-ul
            led = 0;
            // Doar daca bariera este ridicata
            // pentru a evita "coborarea" infinita a barierei
            if (barrier_state) {
                // Modificam starea interna a barierei  
                barrier_state = 0; 
                // Trimitem semnal catre driver-ul de motor pentru a-l 
                // putea actiona
                motorA = 0;
                motorB = 1;  
                // Il actionam timp de 10ms
                delay_ms(10);
                // Oprim rotirea motorulu
                motorA = 0;
                motorB = 0;
            }
        }
    }
}
