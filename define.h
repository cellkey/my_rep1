#ifndef DEFINE_H
#define DEFINE_H

#include <mega644p.h>

#include <delay.h>
#include <stdio.h>
//#include <twi.h>




//-------Board Definitions------------

#define DebugMode       //
//modem type definition

//#define HE910           //Telit G3 modem
//-------------------------------
//#define MODEM_EXT_PWR           //Modem pwered by ext power (more than 3.6V)

//-----------------------------------------------------
#define MODEM_ON_BOARD //    //UNIT WITH MODEM ASSEMBLED
//------------------------------------------------------
#define SIM800 1   // SIMCOM_SIM800L

//---------------------------------------------------------------------------

#define BASE_INTERVAL   5
#define Otopus_interval  60

   #define INTERVAL_PARAM	5       //octopus value?
   #define MEASURE_INTERVAL 5

#define RTC_WD_VAL 20

#define _BV(bit)   (1<<(bit))

#define BLE_RESET_OFF  (PORTB.0 = 0)
#define BLE_RESET_ON  (PORTB.0 = 1)
#define BL_ADDRESS    0x7000       //4K bl

#define CloseNonLatchRelay1  (PORTC.7 = 1)
#define OpenNonLatchRelay1  (PORTC.7 = 0)
#define CloseNonLatchRelay2  (PORTC.5 = 1)
#define OpenNonLatchRelay2  (PORTC.5 = 0)


//typedef unsigned char BYTE;
typedef  char BYTE;

typedef union {
    int ival;
    unsigned char bval[2];
} int_bytes;

typedef union {
    long lVal;
    char bVal[4];
} long_bytes;

#define DATA_REGISTER_EMPTY (1<<UDRE0)
#define RX_COMPLETE (1<<RXC0)
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)

#define MAX_INT         32767
#define MIN_INT         -32768

#define MAX_PRM_TASKS   14
//#ifdef EXT_REF
//#define EXT_VREF 		3300	//reference voltage [mV] external
//#define INT_VREF 		2560	//reference voltage [mV] internal
//#define INT_VREF_1V1 	1100
//
////#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))  //reference voltage [2560mV] internal
//// Voltage Reference: AVCC pin
////#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))  //reference voltage [3300mV] internal
//
//// Voltage Reference: 2.56V, cap. on AREF
//#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))
//
//// Voltage Reference: 1.1V, cap. on AREF
////#define ADC_VREF_TYPE ((1<<REFS1) | (0<<REFS0) | (0<<ADLAR))
//


//#define BATTERY_MUX  4     //Octopus Analog input 3

#define TX_BUFFER_SIZE1 64
#define MAX_TX_BUF_LEN      64
#define MAX_RX_BUF_LEN      128 //was 64. change to 100 3/2014 for alerts
#define MAX_RX0_BUF_LEN     192 //was 64. change to 100 3/2014 for alerts
#define SMS_DATA_BUF_LEN    32
#define RX_BUFFER_SIZE1     192
#define RX_BUFFER_32_BYTE  32


#define SUCCESS 1
#define FAILURE 0

#define CONTINUE    255
#define WAIT        2
#define NO_RSPNS    3

#define ALERT_WAIT          1
#define TRHRESHOLD_CROSSED  2


//flag status

#define RELAY1_ON             0x01
#define RELAY2_ON             0x02
#define UNIT_ARMED               0x04
#define UNIT_ARM_PEND            0x08
#define IS_ALERT_NEEDED          0x10
#define IS_ALERT_ACTIVATED       0x20
#define xxx                      0x40
#define NEW_BLE_UNIT             0x80


#define MAX_ICCID_LEN   20
#define KeepAlivePeriod 600   //1 min

#define TRUE 1
#define FALSE 0
#define ON  1
#define OFF 0

#define LED_1        1
#define LED_2        2
#define LED_3        3

#define LED_OFF     0
#define LED_ON      1
#define LED_BLINK   2
#define SMS_PW_LEN 6
#define DTR_LOW()    (PORTB.3 = 0)
#define DTR_HIGH()    (PORTB.3 = 1)

#define LED1_ON     (PORTD.7 = 1)    //red
#define LED1_OFF    (PORTD.7 = 0)
#define LED2_ON     (PORTD.6 = 1)    //green
#define LED2_OFF    (PORTD.6 = 0)
#define TURN_LED2_TOGGLE()       ((PIND.6 == 1) ? (PORTD.6 = 0) : (PORTD.6 = 1))
#define MODEM_IS_ON() ((PINA.3) == 1) 
#define IGNIT_IO_ON (PORTC.2 = 1)
#define IGNIT_IO_OFF (PORTC.2 = 0)



#define BAT_LEVEL_SW_ON()  (PORTB.1=1)
#define BAT_SW_PIN_OUT()   (DDRB.1=1)
#define BAT_LEVEL_SW_OFF() ( PORTB.1=0)

//EazyGate units types
#define BLE_UNIT  1
#define MODEM_UNIT  2


#define NO_ANSWER       0
#define TASK_COMPLETE   1
#define TASK_FAILED     2

#define TASK_NONE       0

//#define TASK_MEASURE    1
#define TASK_MODEM        2
//#define TASK_DIRECT     3
#define TASK_SLEEP        4
#define TASK_WAKEUP       5
//#define TASK_MONITOR    6
#define TASK_SMS          8


#define TASK_BLUETOOTH   7



#define TASK_MODEM_INIT       1
//#define TASK_MODEM_CHK_VOLT   2
#define TASK_MODEM_CONNECT    3
#define TASK_MODEM_POST       4
#define TASK_MODEM_CLOSE      5
#define TASK_MODEM_SMS  6

#define TASK_MODEM_SMS_HANDLE  10
#define SUB_TASK_MODEM_RECEIVED_SMS 56
#define SUB_TASK_MODEM_ALERT_SMS 57

#define SUB_TASK_INIT_MODEM_ON   11
#define SUB_TASK_INIT_MODEM_OK   12
#define SUB_TASK_INIT_MODEM_REG  13
#define SUB_TASK_INIT_MODEM_REG_GPRS 55    //290517
#define SUB_TASK_INIT_MODEM_RSSI 14
#define SUB_TASK_INIT_MODEM_COPS 15
#define SUB_TASK_MODEM_CHK_ICCID        16
#define SUB_TASK_INIT_MODEM_IGN  17
#define SUB_TASK_INIT_MODEM_GET_COPS    18
#define SUB_TASK_INIT_MODEM_DELAY   19
#define SUB_TASK_INIT_MODEM_PARAMS    20
#define SUB_TASK_INIT_MODEM_SET_CIPMODE 21

#define SUB_TASK_MODEM_CONNECT_ATCH     31

#define SUB_TASK_MODEM_CONNECT_SETUP1   32
#define SUB_TASK_MODEM_CONNECT_SETUP2   33
#define SUB_TASK_MODEM_CONNECT_SETUP3   34
#define SUB_TASK_MODEM_CONNECT_PDP_DEF  35
#define SUB_TASK_MODEM_CONNECT_ACTV     36
#define SUB_TASK_MODEM_CONNECT_START_DIAL    37
//#define SUB_TASK_MODEM_CONNECT_IS_ACTV  38
#define SUB_TASK_MODEM_CONNECT_DELAY    39

//#define SUB_TASK_MODEM_POST_PRM   41

#define SUB_TASK_MODEM_POST_DATA  42
#define SUB_TASK_MODEM_ASK_LIST_UPDATE 43
#define SUB_TASK_MODEM_WAIT_FOR_LIST   44


#define SUB_TASK_MODEM_CLOSE_PPP    51
#define SUB_TASK_MODEM_CLOSE_TCP    52
#define SUB_TASK_MODEM_CLOSE_MDM    53
#define SUB_TASK_MODEM_OFF          54
#define SUB_TASK_MODEM_ENDED        55



#define DO_NONE 0
#define DO_DATA 1
#define DO_SLEEP  2
#define DO_BLE  3
#define DO_MODEM  4

#define TASK_MSR_START_HEAT 1
#define TASK_MSR_READ       2
//#define TASK_MSR_END_HEAT   3
#define TASK_MSR_SAVE       4

#define TASK_MONITOR_CONNECT    1
#define TASK_MONITOR_WAIT   2
//-----BLE----------------------
#define TASK_BLE_CHECK_STAT   1
#define TASK_BLE_CONNECTED   2
#define TASK_BLE_END_WAIT_FOR_USER_INPUT  3
#define TASK_BLE_USER_MSG_OK   4
#define TASK_BLE_END_CON       5
#define TASK_BLE_TO_MODEM    6

#define GET_UPDATE      1
#define CONFIRM_UPDATE  0

//------------sensors types definition ----------------------
#define SENSOR1         0     // not more than 10 sensors!!!
#define SENSOR2         1
#define SENSOR3         2
#define SENSOR4         3
#define SENSOR5         4
#define SENSOR6         5
#define SENSOR7         6
#define SENSOR8         7

#define MAX_SEN_NUM     8       //was 8 120316

//sensors' addresses
//#define SDI_EC_ADDRESS 3       //Ponsel sensors
//#define S485_EC_ADDRESS 30
//#define SDI_PH_ADDRESS 2       //Ponsel sensors
//#define S485_PH_ADDRESS 20
//#define SDI_5TE_ADDRESS 0       //Ponsel sensors

#define NO_TYPE 0

#define AT  2
#define PH  3                 //PH kando     4-20mA
#define EC_K0  4             //EC kando   4-20mA
#define WLV 5                //Level kando  -4-20mA
#define AH  6
#define TIR 7               //total radiation
#define EC_SDI12        8   //Danny
#define TMP_SDI12       9
#define PH_SDI12        10
#define COMBO2_EC_SDI12  11    //EC+temp  or PH+TEMP
#define COMBO4_SDI12    12    //EC+TEMP+PH +ORP on same connector and power
#define COMBO_5TE       13      //5TE sensor
#define COMBO2_PH_SDI12 14   //PH+temp+ORP
#define WLV_U 15            //Level kando
#define CT    16
#define LT    17
#define SD    18
#define HS10  19
#define BATTERY         20
#define PUMP            21       //Danny 040315
#define ST5   22
#define TNS   23    //0-2 volt
#define AT_B  24  //tmp Boaz   1wire
#define EC_K  25  //EC2 kando  -4-20mA
//----shkila-----------
#define SCAN  26       //shkila Scanner
#define SCALE 27       //shkila sCale
#define GPS   28       //shkila GPS
#define PURE_4_20 29     //sensor has external supply. Data ready any time
#define GS1   30
#define SM    31
#define EC    32
#define WLV_PULSTAR 33   //new water level 100117
#define TMP   34
#define OXGN  36

#define DW_LE  38 // use to be  37   //Kando-water existance - dry contact Spain
#define H2S   37        //290317 Kando

#define RAINMTR 39
#define WTRMTR  40
#define MATIC 41      //fLOATING SENSOR

#define SM_4_20 42     //elchanani
#define TSS 43
#define MICRO_FLOW 44   //flow rate measure
#define WL_NIVELCO  45

#define COMBO4_485    48   //0x30 - EC+TEMP+PH +ORP on same connector and power

#define COMBO_EC_485    49  //0x32 EC+Temperature
#define COMBO_PH_485    50  //0x32  PH+ORP
#define FAKE_SENS       240

#define DIGIN1 60      //digital input - PCINT0
#define DIGIN2 61
#define DIGIN3 62
#define DIGIN4 63

//--------------End sensors types------------------------

#define MAX_IO_TYPES    7

//PRR bits

#define PRR_TWI    7
#define PRR_TMR2   6
#define PRR_TMR0   5
#define PRR_UART1  4
#define PRR_TMR1   3
#define PRR_SPI    2
#define PRR_UART0  1
#define PRR_ADC    0

#define PRR_SHUTOFF()  (PRR = 0x7F);//(1<<PRR_TWI)|(1<<PRR_TMR2)|(1<<PRR_TMR0)|(1<<PRR_UART1)|(1<<PPRR_TMR1)|(1<<PRR_SPI)|(1<<PRR_UART0)|(1<<PRR_ADC));
#define PRR_ALL_ON()  (PRR = 0x04);
#define PRR_EN_TWI()  (PRR &= ~(1<<PRR_TWI));
#define PRR_EN_TMR2()  (PRR &= ~(1<<PRR_TMR2));
#define PRR_EN_TMR0()  (PRR &= ~(1<<PRR_TMR0));
#define PRR_EN_UART1()  (PRR &= ~(1<<PRR_UART1));
#define PRR_EN_TMR1()  (PRR &= ~(1<<PRR_TMR1));
#define PRR_EN_SPI()  (PRR &= ~(1<<PRR_SPI));
#define PRR_EN_UART0()  (PRR &= ~(1<<PRR_UART0));
#define PRR_EN_ADC()  (PRR &= ~(1<<PRR_ADC));


#define UDR UDR0
#define UCR UCSR0B               //change for atmega324p
#define USR UCSR0A               //change for atmega324p
#define UBRR UBRR0L              //CHANGE for atmega324

//#define ENABLE_EXT_INT0()  (GIMSK |= 0x40);
//#define DISABLE_EXT_INT0() (GIMSK &= 0xBF);
//
//#define ENABLE_EXT_INT1()  (GIMSK |= 0x80);
//#define DISABLE_EXT_INT1() (GIMSK &= 0x7F);
#define ENABLE_CLOCK_INT()   (EIMSK |= (1<<INT2));    // enable  external interrupt (clock int)
#define DISABLE_CLOCK_INT()  (EIMSK &= ~(1<<INT2));   // disable  external interrupt (clock int)

#define ENABLE_EXT_INT0()    (EIMSK |= (1<<INT0));    // enable  external interrupt (wind speed)
#define DISABLE_EXT_INT0()   (EIMSK &= ~(1<<INT0));   // disable  external interrupt (wind speed)

#define ENABLE_EXT_INT1()    (EIMSK |= (1<<INT1));    // enable  external interrupt (rain meter)
#define DISABLE_EXT_INT1()   (EIMSK &= ~(1<<INT1));   // disable  external interrupt (rain meter)

#define ENABLE_TIMER1_COMPA()   ( TIMSK1 |= (1<<OCIE1A));
#define DISABLE_TIMER1_COMPA()  ( TIMSK1 &= ~(1<<OCIE1A));   //danny

#define ENABLE_TIMER1_OVF()   ( TIMSK1 |=  (1<<TOIE1));
#define DISABLE_TIMER1_OVF()  ( TIMSK1 &= ~ (1<<TOIE1));   //danny

#define ENABLE_TIMER0()    (TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00)); // = 3.6 khz
#define DISABLE_TIMER0()   (TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00));
                         
#define ENABLE_TIMER1()    (TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (1<<CS10));
#define DISABLE_TIMER1()   (TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10));
                        
                       
#define ENABLE_TIMER2()    (TCCR2B =(0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20));  // = 7.2 khz  35.5 mili sec period
#define DISABLE_TIMER2()   (TCCR2B =(0<<WGM22) | (0<<CS22) | (0<<CS21) | (0<<CS20));

#define ENABLE_PA0_INT()   (PCMSK0 |= (1<<PCINT0)); //digital input  //PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (1<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);
#define DISABLE_PA0_INT()   (PCMSK0 &= ~(1<<PCINT0));  //PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (1<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);

#define ENABLE_PA1_INT()   (PCMSK0 |= (1<<PCINT1)); //digital input  //PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (1<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);
#define DISABLE_PA1_INT()   (PCMSK0 &= ~(1<<PCINT1));  //PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (1<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);

#define ENABLE_PA2_INT()   (PCMSK0 |= (1<<PCINT2)); //RING signal  //PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (1<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);
#define DISABLE_PA2_INT()   (PCMSK0 &= ~(1<<PCINT2));  //PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (0<<PCINT5) | (0<<PCINT4) | (0<<PCINT3) | (1<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);

#define ENABLE_PC2_INT() (PCMSK2 |= (1<<PCINT18)); //PCMSK2=(0<<PCINT23) | (0<<PCINT22) | (0<<PCINT21) | (0<<PCINT20) | (0<<PCINT19) | (1<<PCINT18) | (0<<PCINT17) | (0<<PCINT16);
#define DISABLE_PC2_INT()  (PCMSK2 &= ~(1<<PCINT18)); //PCMSK2=(0<<PCINT23) | (0<<PCINT22) | (0<<PCINT21) | (0<<PCINT20) | (0<<PCINT19) | (0<<PCINT18) | (0<<PCINT17) | (0<<PCINT16);

#define ENABLE_UART0()      (UCSR0B=(1<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80));
#define DISABLE_UART0()     (UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (0<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80));

#define ENABLE_UART1()      (UCSR1B=(1<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81));
#define DISABLE_UART1()     (UCSR1B=(0<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (0<<RXEN1) | (0<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81));

#define ENABLE_RX_INT_UART0()  (UCSR1B=(1<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80));
#define DISABLE_RX_INT_UART0() (UCSR1B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80));

#define ENABLE_RX_INT_UART1()  (UCSR1B=(1<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81));
#define DISABLE_RX_INT_UART1() (UCSR1B=(0<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81));

#define ENABLE_ADC()  (ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
//#define ENABLE_ADC()  (ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
#define DISABLE_ADC() (ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0));

#define WATCHDOG_ENABLE_STEP1() (WDTCSR=(0<<WDIE) | (1<<WDP3) | (1<<WDCE) | (1<<WDE) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0));
#define WATCHDOG_ENABLE_STEP2() (WDTCSR=(0<<WDIE) | (1<<WDP3) | (0<<WDCE) | (1<<WDE) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0));

//#define WATCHDOG_ENABLE() 	   	(WDTCSR = 0x0F) //WDE = 1 WDT = 2048 cycles
#define WATCHDOG_OFF_ENABLE()  	(WDTCSR = 0x1F) //WDTOE = 1 WDE = 1
#define WATCHDOG_DISABLE() 		(WDTCSR = 0x00) //WDE = 0

#define TWI_DISABLE()    TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);
#define TWI_ENABLE()    TWCR=(0<<TWEA) | (0<<TWSTA) | (1<<TWSTO) | (0<<TWEN) | (0<<TWIE);

//--------------------------------

//map memory: 1 KB memory per sensor
#define SENSOR_MEMORY_START     0x00   //0                                         +
#define SENSOR_MEMORY_SIZE      0x400   //1024
#define SENSOR_CNTRL_PRM_SIZE   0x10   //16   at the end of the 1024
#define pRead_BU_ADDRESS        7184   //0x400 * 7 + 16     120316

#define WEIGHING_HEAT_TIME      30
#define BATTERY_HEAT_TIME       15
#define VISHAY_HEAT_TIME        30
//#define  PUMP_HEAT_TIME         450   //according to Kando

#define CONNECTING_MINUTE     1   // Changd by Danny - 310115 the 1st minutes is the c

#define RETRY_CONNECTIN_TIME    30   //Danny - was 15

#define GET_REQUEST 0
#define SET_REQUEST 1

#define REQ_ID      0
#define REQ_MSR     10
#define REQ_INTRVL  20
#define REQ_TYPE    30
#define REQ_TIME    40
#define REQ_IP      41
#define REQ_PORT    42
#define REQ_APN     43
#define REQ_MCC     44
#define REQ_MNC     45
#define REQ_ROAMING 46
#define REQ_SCH     47  // START CONNECT HOUR
#define REQ_CPD     48  //CONNECTS PER DAY
#define REQ_CI      49  //CONNECTS INTERVAL
#define REQ_PSWD    50
#define REQ_OPTION1 51
#define REQ_OPTION2 52
#define REQ_OFFSET  53
#define REQ_GAIN    54
#define REQ_BTR     55
#define REQ_RSSI    56
//#define REQ_GMT     57
#define REQ_VER     58
#define REQ_NUM_SEN     59
#define REQ_COMBINATION 60
#define REQ_DISCNCT   61
//#define REQ_LIMITS      62
#define REQ_MSR_NOW   63


//Errors codes definition (note - err_buf real info started at position [1])
//the error code order is in roole: most importent error ->larger code num.
/*#define NO_ERROR        0x00
#define EXT_RESET       0x01
#define POWERDOWN_RESET 0x02
#define WATCHDOG_RESET  0x03
#define T_CIRC_FAIL     0x04
#define T_LVDT_FAIL     0x05
#define SENS_1_LO       0x06
#define SENS_1_HI       0x07
#define SENS_2_FAIL     0x08
#define SENS_1_FAIL     0x09
#define EE_FAIL         0x0A
#define CLOCK_FAIL      0x0B
#define RADIO_FAIL      0x0C
#define LO_BATT         0x0D
*/
//#ifdef DebugMode
void  _putchar1(char c);
void _putchar0(char c);
//#endif DebugMode
/////////////////////////////////////////////
// Data_manager functions
////////////////////////////////////////////
//declare functions prototypes
//the function will copy one data block in ext_e2 (pBread address)
//into 'sensDataBlock' buffer in ram
//return 1 (succsess) or 0 (failure)
//char CopyBlockIntoRam();

//open new data block
//char CreateNewDataBlock();

//arrange the saving of sensors measurments results in the external eeprom
//the function return (1 = success) or (0 = failue)

//char GetMeasurments(char read_mode);
//set next pBread position
//end of reading space is the first block (including first block)
//return 1 if there is new pBread address or 0 if not
//char NextpReadAddress();

//char InitDataBlocks(char interval);


//char PointersValidate();

//char ResetReadPointer();
//char _GetMeasurments_(char read_mode);
/////////////////////////////////////////////
// i2c_bus functions
////////////////////////////////////////////
//void SendStartBit(void);

//void SendStopBit(void);

//void SendByte(unsigned char send_byte);

//unsigned char TestAck(void);

//void SendAck(void);

//unsigned char RecByte(void);

//unsigned char SendBuf(unsigned char adress, int length, unsigned char  *buffer);

//unsigned char GetBuf(unsigned char adress, int length, unsigned char *buffer) ;

//unsigned char GetBufClk(unsigned char adress, int length, unsigned char *buffer) ;

/////////////////////////////////////////////
// twi functions
////////////////////////////////////////////
/*
void twiInit (void);

int twiWriteData(unsigned char address, int dataLen,  const unsigned char *data);

void twiReadData(unsigned char address, int dataLen,   unsigned char *data);
*/
/////////////////////////////////////////////
// general functions
////////////////////////////////////////////

//void WakeUpProcedure(void);

void PowerDownSleep ( void );

void WDT_off(void);

BYTE CheckSum( BYTE *buff, BYTE length, BYTE param );

void TransmitBuf(char iPortNum);

void InitProgram( void );

void  Init_Modem_Vars();

int CheckResult();

//void CheckSen();

//void ReadSensor();

//void CheckBtrVolt();
//copy from cpu e2 into buf
void cpu_e2_to_MemCopy( BYTE* to, char eeprom* from, BYTE length);

void cpu_flash_to_MemCopy( BYTE* to, char flash* from, BYTE length);

BYTE CopyFlashToBuf( BYTE* to, char flash* from);

void MemCopy( BYTE* to, BYTE* from, BYTE length);

//the function will set address into 'pSens_Ctrl_Params' and 'pSens_cpu_e2'

int bytes2int( char* buf);

void int2bytes(int int_pointer,  char* buf);
long Bytes2Long(char* buf);

void Long2Bytes(long l, char* buf);


BYTE IsTimeToConnectGPRS();
//char ResetAllReadPointerspBwrite();

//copy from ram buf into cpu e2
void MemCopy_to_cpu_e2( char eeprom* to, BYTE* from, BYTE length);


/////////////////////////////////////////////
// globals functions
////////////////////////////////////////////

//arrange the clock buf before setiing time into rtc
//void SetClockBuf(void);

//power on initialization ruthin
//check if power flag is on, if yes you should preform reset to r.t.cclock
//unsigned char IsPowerFlagOn(void);

//unsigned char ResetCommand(void);
//void ResetCommand(void);

//unsigned char DisableClockIntr(void);
//void DisableClockIntr(void);

//unsigned char SetRealTime(void);

//read the clock data into buffer
//unsigned char ReadTime(void);

//initiate the rtc at program startup
//char InitRTC();
//void InitRTC();

//unsigned char ResetClockIntr(unsigned char);
//void ResetClockIntr();

//void GetRealTime(void);
void rtc_set_timeAll(char *clockBuf); //Danny
void rtc_get_timeAll(char *clockBuf);

unsigned char RTC_CDI_init(unsigned int interval, char freq);
//void SetRtc24Hour();
//#ifdef DebugMode     //  Danny
void SendDebugMsg(flash  char *bufToSend);


void PrintNum(long val);
void PrintNum2(unsigned long val);
//void  _putchar1(char c);

//#endif DebugMode     //Danny
/////////////////////////////////////////////
// eeprom functions
////////////////////////////////////////////

//char e2_readSeqBytes(unsigned int address, char read_length);

//char e2_writePage(unsigned int address, char write_length, char* string_1);

/////////////////////////////////////////////
// modem_manager functions
////////////////////////////////////////////

void ModemMain();

//void ParseModemResponse();

void SetModemPwrOn();
void ModemIgnit(void);
//
//void SetModemOff();

//void SendATCmd(flash unsigned char *bufToSend);

/////////////////////////////////////////////
// measure functions
////////////////////////////////////////////
//void MeasureMain();

unsigned int AnalogSensRead();

//BYTE SetSensorCombination(BYTE* );

//BYTE SendAlerts();

//BYTE SetSensorType(BYTE IOindex, char newType);

/////////////////////////////////////////////
// Monitor_manager functions
////////////////////////////////////////////

//void MonitorMain();

/////////////////////////////////////////////
// Each sensors combination general functions
////////////////////////////////////////////

//void SetSensorHeatTime();


//BYTE GetSensorType();
//BYTE GetSensorIOType();

//void SensorPowerOn();

//void SensorPowerOff();
//
//void InitAdc(void);
/////////////////////////////////////////////
// w1_ds1820 functions:
/////////////////////////////////////////////

void BatLevel_ON(void);
void BatLevel_OFF(void);
//void SendPostData(/*BYTE readMode*/);
char RTC_Update_by_Modem(void);
// int PHcalc(int value);
// int ECcalc(int value);
// int EC_mV_calc(int value);
// int PH_mV_calc(int value);
 unsigned int STR2BIN(unsigned char *str);
 void Set_RTC_WD_Val(unsigned char WDtime);
 unsigned char Set_RTC_WD(void);
void Num2String_Modem(long val);

 //IO expender
// unsigned char ReadIOExpReg(unsigned char reg);
// void SetExpender(void);
// unsigned char ReadMode (void);
//char Pharse_params_struct(char *str, unsigned int index);

//char Pharse_SDIparams_struct(char *str, char index);
//void ResetPumpFlags(void);
  void CloseSuccessfulCom(void);
// char StoreOldReadPoiters(char num);
//#endif EC_SM_TMP

#endif DEFINE_H