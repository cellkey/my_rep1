#ifndef __ADC_C
#define __ADC_C

#include "define.h"

#define EXT_VREF 		3300	//reference voltage [mV] external
#define INT_VREF 		2560	//reference voltage [mV] internal
#define INT_VREF_1V1 	1100



// Voltage Reference: AVCC pin
//#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))  //reference voltage [3300mV] internal

// Voltage Reference: 2.56V, cap. on AREF
#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// Voltage Reference: 1.1V, cap. on AREF
//#define ADC_VREF_TYPE ((1<<REFS1) | (0<<REFS0) | (0<<ADLAR))





//extern eeprom BYTE SensorType[MAX_SEN_NUM];//= {0, 0, 0, 0, 0, 0, 0, 0};
//extern eeprom char ADCMux[MAX_SEN_NUM];


// Read the AD conversion result
unsigned int ReadAdc()  
{

               
    delay_us(10);
    // Start the AD conversion
    ADCSRA|=(1<<ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}




unsigned int AnalogSensRead()
{
    char i;
    unsigned int adc_res, hi_value, lo_value;
    unsigned long cur_value;
    char BATTERY_MUX = 4; 
    
//    #ifdef DebugMode
//        SendDebugMsg("\r\nAnalogSensRead()\r\n");      
//        #endif DebugMode   
              
    // enable ADC conversion
    ADCSRA |= (1<<ADEN);  
    ADCSRA |= (1<<ADATE);
    PRR_EN_ADC();     
    
    hi_value = adc_res = 0;
    lo_value = 0xFFFF;  
    
    ADMUX = BATTERY_MUX | ADC_VREF_TYPE;     //V4  =  4                 
  
    for(i = 0; i < 10; i++)
    {
        cur_value = ReadAdc();

        // calculate the voltage in [mV]: io 3 & 4 - ext, all other - internal
    //    cur_value = (cur_value * INT_VREF)/1024L; 
         cur_value = (cur_value * 2.5); 
    //      cur_value = (cur_value * INT_VREF_1V1)/1024L;       
      //   cur_value = (cur_value * EXT_VREF)/1024L;
        delay_ms(5);
        if (hi_value < cur_value)
            hi_value = (int)cur_value;
        if (lo_value > cur_value)
            lo_value = (int)cur_value;
        adc_res += cur_value;
    }
   // disable ADC conversion   
   
//     SendDebugMsg("\r\nRaw bat level- \0");  
//     cur_value = ReadAdc();
//     PrintNum((long)cur_value);
     
//disable ADC when done
     ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
     
    adc_res = adc_res - hi_value;
    adc_res = adc_res - lo_value;
    adc_res = adc_res / 8;
//     #ifdef DebugMode  
//      SendDebugMsg("\r\n***Measured val= ");
//    PrintNum(adc_res);  
//    #endif DebugMode

    return adc_res;
}

unsigned int STR2BIN(unsigned char *str)
 {
  
  unsigned int BinTmp;
 
      BinTmp = (unsigned int)(str[3]-0x30); 
      BinTmp += (unsigned int)(str[2]-0x30)*10; 
      BinTmp += (unsigned int)(str[1]-0x30)*100;
      BinTmp += (unsigned int)(str[0]-0x30)*1000; 
    //  BinTmp += (unsigned int)(str[0]-0x30)*10000; 
      
      return BinTmp;
  } 
#endif __ADC_C