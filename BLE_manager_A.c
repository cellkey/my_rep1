#include "define.h"
#include "uart.h"
#include "sms.h"
//#include "vars.h"
#include <iobits.h >
#include <string.h >
#include <stdlib.h>
#include "util.h"


#define BLE_TIME_OUT 30; //3 sec
#define RelayOpenPeriod 80
//#define KeepAlivePeriod 600   //1 min

extern flash unsigned char KEEP_ALIVE[]; 
extern  flash unsigned char AT_TCP_CLS[];  //sim800
extern  flash unsigned char MODEM_SHDN[];
extern eeprom Relay_table Relay_vars[2]; 
extern eeprom  char eUnit_Name[17];
extern eeprom char eFLAGS_STSTUS;
extern eeprom unsigned int  eLogIndex;

char ATH[] = "ATH\r\n\0";   //hangup call  
char CHUP[] = "AT+CHUP\r\0";
char AT_D0[] = "AT&D0\r\n\0";     //dtr hangup call
char AT_D1[] = "AT&D1\r\n\0";     //dtr hangup call
char plusplus[]= "+++";

char ATO_count = 0;             //ATO failures count



bit timer1_count_session;
bit EncriptedValOK;
bit BLE_connected = 0;
bit SearchServerDataBase;
bit SearchDial = FALSE;
bit ReDialNeeded = FALSE;
bit SMS_ACTIVE = FALSE;
bit  ATO_needed = FALSE;
bit ResetNeeded = FALSE;
bit ServerSMSneeded = FALSE;
 bit Relays_Activation_Needed = FALSE;

BYTE BLECurTask;
int time1_count;
char Relay_period;
char encrypted_data[10];
char ActiveRelay;
char RingCounter = 0;
char UNIT_TYPE;
unsigned char index;
unsigned int CurrentCount;
unsigned int BLE_RX_count;
  
extern void CloseRelay1 (void);
extern void OpenRelay1 (void);
extern void CloseRelay2 (void);
extern void OpenRelay2 (void);
extern void PCmessage( char *message );
extern void InitPeripherals();
extern unsigned char IsNumAllowed1( char *Num, unsigned char Length );
extern  unsigned char IsNumAllowed2( char *Num, unsigned char Length );
extern void UART0_WriteMsg( char *InStr);

extern  void Uart1MsgN( char *message, char Lengh );
extern unsigned char  GetStatusReg(unsigned char mask);
extern void SetStatusReg(unsigned char mask, unsigned char val);
extern void SendATCmd(flash unsigned char *bufToSend);
extern void SendSModemString( char *bufToSend, BYTE bufLen);
extern void  Modem_Close_Connection(void);
extern  char CheckMessage(char source);
extern char Modem_Server_Reconnect(void);

extern char ComBuf[MAX_TX_BUF_LEN];
extern  unsigned int UsersQuantity;
extern  unsigned int rx_counter1;
extern int heat_time;
extern char rx_buffer1[];
extern volatile unsigned int rx0_buff_len; //unsigned int rx0_buff_len;
extern BYTE NoRecDataU0Cnt;
extern char  FlagsStatus;
extern char TicksCount;
extern char RxUart0Buf[MAX_RX0_BUF_LEN];
extern char CallerIdBuf[]; 
extern char UnitIDstring[17];
extern BYTE mainTask;
extern BYTE modemCurTask;
extern unsigned int ExtMemUsersQuantity;
extern unsigned int Keep_Alive_timer;
extern BYTE modemCurSubTask;
extern BYTE BytesToSend;
extern char UNIT_NAME[];


extern   bit Message_Recieved; 
extern   bit DIGITAL_IN_ACTIVE;
extern   bit BOTTON_ACTIVE;
extern   bit OPEN_SESSION;
extern   bit CLOSE_SESSION;
extern   bit CONTINUOUS_RELAY;
extern   bit Modem_Message_Recieved;
extern   bit ModemAlive;
extern   bit RING_PULSE_ACTIVE;
extern   bit  ServerComOn;;
extern bit KeepAliveMsgNeeded;
extern bit FOTA_process;
  
extern   bit Open_Session_On;
extern bit MODEM_GOT_IP;


//char UNIT_TOKEN[] = "CR";
//char USER_OK[] = "USER_OK\r\n\0";
//char Hello_Msg[] = "Hello..\r\n\0";


 //adjusted for more than one reponses like in FTP activities
unsigned char ChkBLERespons(char flash *str1,char flash *str2, unsigned char timeout)
{
  
     char *ptr;
     bit Match1, Match2;  
     Match1 = Match2 = FALSE; 
     
       time1_count = timeout;   //100mS value
       timer1_count_session = TRUE;  //at timer1 int
       ENABLE_TIMER1();          //
    
     do
     {

         if (Message_Recieved)
         {
              Message_Recieved = FALSE;
              ptr = strstrf( rx_buffer1, str1);           //If str in RX buf
              if(ptr != NULL) 
              {   
               
                 Match1 = TRUE;                  
              }
              
              else if (str2[0] != '-')
              {
                    ptr = strstrf( rx_buffer1, str2);           //If str in RX buf
                    if(ptr != NULL)
                    Match2 = TRUE;
              }
//               TxMsg ("Debug: ");
//               ShowOnScreen( rx_buffer1);       //debug
           
          }
      
      }
      while(( Match1 == FALSE) && (Match2 == FALSE) &&  (time1_count > 0)); 
       //  DISABLE_TIMER1() ;  //stop clock
       #asm  ("WDR");   
        if(time1_count > 0)   //found match
        {
             if (Match1 == TRUE) 
             return 1;
             else if (Match2 == TRUE)
             return 2;
        }
        SendDebugMsg("BLE command response Time Out!\r\n\0");
        return 0;      //no respons - time out, return 0

}

//adjusted for more than one reponses like in FTP activities
unsigned char ChkNodemRespons(char flash *str1,char flash *str2, unsigned char timeout)
{
  
     char *ptr;
     bit Match1, Match2;  
     Match1 = Match2 = FALSE; 
     
       time1_count = timeout;   //100mS value
       timer1_count_session = TRUE;  //at timer1 int
       ENABLE_TIMER1();          //
    
     do
     {

         if (Modem_Message_Recieved)
         {
              Modem_Message_Recieved = FALSE;
              ptr = strstrf( RxUart0Buf, str1);           //If str in RX buf
              if(ptr != NULL) 
              {   
               
                 Match1 = TRUE;                  
              }
              
              else if (str2[0] != '-')
              {
                    ptr = strstrf( RxUart0Buf, str2);           //If str in RX buf
                    if(ptr != NULL)
                    Match2 = TRUE;
              }
           
          }
      
      }
      while(( Match1 == FALSE) && (Match2 == FALSE) &&  (time1_count > 0)); 
         DISABLE_TIMER1() ;  //stop clock
       
        if(time1_count > 0)   //found match
        {     
              ENABLE_TIMER1(); 
             if (Match1 == TRUE) 
             return 1;
             else if (Match2 == TRUE)
             return 2;
        }
        SendDebugMsg("Modem response Time Out!\r\n\0"); 
         ENABLE_TIMER1(); 
        return 0;      //no respons - time out, return 0

}

 void BLE_RESET(void)
 {    
       DDRB.0 = 1;   //output 
       BLE_RESET_ON;  //      
       delay_ms(300);
       BLE_RESET_OFF; // PORTB.0 = 0;  
       delay_ms(500);  
       BLE_RX_count = 0;
 }
 
//set unit name for advertising
char Set_BLE_Name(void)
{
//   char BLE_ID[] = "ccr160649\r\n\0";  //10 bytes
 
 
   char str[22];
   char result = 0; 
 //  char n[] = "n\0";
    
    #asm("sei");  
       UBRR1L=0x03; //BLE boaud 115200
    
       BLE_RESET();  
       delay_ms(100);           
       sprintf(str, "AT+NAME%s\r\n",UNIT_NAME); //                       
       UART1_WriteMsg(str);  //send string to ble module 
       delay_ms(100); 
       result = ChkBLERespons("OK","-",25);  //2.5 sec timeout 
                                            
       BLE_RESET(); 
      if(UNIT_NAME[3] < 0x38)       //cr16xxxxxxx
      UNIT_TYPE = BLE_UNIT;
      else                          //cr18xxxxx
      UNIT_TYPE = MODEM_UNIT;       
       UBRR1L=0x17;    //uart baud 19200?????????
     
       if(result)
       {    
          LED1_ON;
          delay_ms(100);                    
          LED1_OFF;
          return 1;
       }
     
      return 0;
}

BYTE GetNextBLETask()
{

 char *ptr,i,j,k;

 
    switch (BLECurTask)
    {
        case TASK_NONE: 
                 
                  
                if (BOTTON_ACTIVE == TRUE)
                {   
                   SendDebugMsg("Ext. botton active..\r\n\0");
                    BOTTON_ACTIVE = FALSE; 
//                    Relay_period = 2;   
//                    Relays_Activation_Needed = TRUE;     
//                    BLECurTask = TASK_BLE_USER_MSG_OK; //open                        
//                    return CONTINUE; 

                      SendDebugMsg("jump to BL..\r\n\0");  
                      #asm("jmp 0x7000");    //to BL
                } 
                 if ( Message_Recieved == TRUE)   //BLE msg in buffer
                 {   
                         Message_Recieved =  FALSE;  
                      
//                       for(i = 0; i<rx_counter1; i++)
//                      _putchar1(rx_buffer1[i]);
                    //   SendDebugMsg("\r\nBLE Message_Recieved..\r\n\0"); 
                                                   
                      ptr = strstrf(rx_buffer1,"+CON");   //
                     if(ptr != NULL)   //CONN  found         
                      {                       
                                BLE_connected = TRUE; 
                                LED1_ON;  //red                                                                                                                                                              
                                BLECurTask = TASK_BLE_CONNECTED;  
                            //    SendDebugMsg("\r\nBLE connected..\r\n\0"); 
                                break; 
                      }                                     
                      else
                      {    
                           rx_counter1 = 0; 
                           ENABLE_RX_INT_UART1();                     
                        //   SendDebugMsg("\r\nUnit Ready..\r\n\0");
                      }      
                  } 
                   
                 //============modem===================================       
                 else if (Modem_Message_Recieved == TRUE)   //Modem msg in buffer  
                 {    
                       DISABLE_RX_INT_UART0();    //stop receiving - keep buffer                         
                       Modem_Message_Recieved = FALSE;   
                    //   rx0_buff_len = 0;                                        
                       SendDebugMsg("RX Modem Msg....\r\n\0");
                      
                        BLE_RESET_ON;       //Modem session - stop BLE 
                      
                        
                          ptr = strstrf(RxUart0Buf,"CLIP");   //RING .+CLIP: "0522784873",129,"",0,"",0 
                          if(ptr != NULL)                     //RING  found         
                          {      
                                 SendDebugMsg("RING process..\r\n\0");          
                                 RingCounter++; 
                            //     PrintNum((long)RingCounter);
                                 LED1_ON;  //red 
                              //   m = 0;                                                                                                                                                   
                               
                                  
                                if( RingCounter == 1) //check on first ring only-if not found hangup after 3 rings
                                
                                    k = (ptr - RxUart0Buf);  //k pointer to start of token CLIP  
                                    k += 7;                 //k point to first digit of number 972......
                                    i = k + 13;              //k point to end of caller id
                                    j=0;
                                    while ((RxUart0Buf[k] != '"') && (k < i))    // get caller ID 
                                    CallerIdBuf[j++] = RxUart0Buf[k++]; 
                                    CallerIdBuf[j]  = '\0'; 
                                                                      
                                          
                                    UART0_WriteMsg (CHUP);    //hangup call //hangup call 
                                     ChkNodemRespons("OK","-",30);
//                                            delay_ms(200); 
                                    ATO_needed = TRUE;                                                              
                                    ReDialNeeded = FALSE;  //new server dial in case of close                                                                      
                                    SearchDial = TRUE;     //checked at modem     
                                                     //---------------------------Boaz back door----------------------------------------      
                                                         ptr = strstrf(CallerIdBuf,"91723");   //boaz num - eneble back door
                                                         if(ptr != NULL)                     //RING  found         
                                                         {
                                                             ResetNeeded = TRUE;  
                                                             OpenRelay1(); //   //default relay  
                                                             delay_ms(1000);
                                                             CloseRelay1(); //   //default relay  
                                                             delay_ms(100);     
                                                             ResetNeeded = FALSE; 
                                                             SendATCmd(MODEM_SHDN); 
                                                             delay_ms(1000);  
                                                                     
                                                             #asm("jmp 0");     //reset unit
                                                         }
                                                      //--------------------------------------------------------------------                                              
                                    Modem_Message_Recieved = FALSE;  
                                    BLECurTask =  TASK_BLE_TO_MODEM;  //move to modem code 
                                    MODEM_GOT_IP = TRUE;  //test it 
                                           
                                     return CONTINUE;                               
                                     
                                      
                         }                                                                          
                          else   //no ring -SMS?
                         {      
                              SendDebugMsg("NO RING process..\r\n\0"); 
                             ptr = strstrf(RxUart0Buf,"CMT");    //SMS text - +CMT: "+972522784873","","20/01/03,09:06:14+08" .123456{"L"} 
         
                             if(ptr != NULL)   //CMT  found         
                              {                                                            
                                    LED1_ON;  //red                                                                                                                                                     
                                    SendDebugMsg("SMS process..\r\n\0");  
                                                
                                     Init_Modem_Vars();   //to modem tasks
                                     RING_PULSE_ACTIVE = TRUE;    //ring signal by SMS 
                                     SMS_ACTIVE = TRUE;
                                  break; 
                              } 
                                else    // FOTA?
                              {       
                                     SendDebugMsg("NO CMT process..\r\n\0"); 
                                     ptr = strstrf(RxUart0Buf,"FOTA");    //SMS text - +CMT: "+972522784873","","20/01/03,09:06:14+08" .123456{"L"} 
         
                                     if(ptr != NULL)   //        
                                      {                                                            
                                            LED1_ON;  //red                                                                                                                                                     
                                             SendDebugMsg("FOTA process..\r\n\0"); 
                                             FOTA_process = TRUE; 
                                                        
                                           //  Init_Modem_Vars();   //to modem tasks
                                             RING_PULSE_ACTIVE = FALSE;    //ring signal by SMS 
                                             SMS_ACTIVE = FALSE;
                                             MODEM_GOT_IP = TRUE;  
                                             BLECurTask =  TASK_BLE_TO_MODEM; 
                                             return CONTINUE;
                                      } 
                                     else
                                    {         
                                           SendDebugMsg("NO FOTA process..\r\n\0"); 
                                            ptr = strstrf(RxUart0Buf,"SMS");   //SMS972522784873
                                           if(ptr != NULL)                     //RING  found    
                                           {     
                                                                
                                                  index = (ptr - RxUart0Buf) + 3;    // index point to first digit of target num (SMS972522784873) 
                                                  i = 0;
                                                  while(RxUart0Buf[index] != '\r')
                                                  {
                                                       CallerIdBuf[i++] = RxUart0Buf[index++]; 
                                                  } 
                                                  CallerIdBuf[i] = '\0';
                                                  ServerSMSneeded = TRUE;   
                                                  RING_PULSE_ACTIVE = TRUE; //Modem treat sms                                                                                                                                                 
                                                  BLECurTask =  TASK_BLE_TO_MODEM;  //move to modem code 
                                                  MODEM_GOT_IP = TRUE;  //test it 
                                                   
                                                  return CONTINUE;                               
                                           } 
                                    }           
                              }                               
                                
                          } 
                          
                                                                                                                                                                                         
                                                                      
                          ptr = strstrf(RxUart0Buf,"OPEN");    // OPEN102                            
                          if (ptr != NULL)   
                          {       
                                  index = (ptr - RxUart0Buf) + 4;    // index point to relay index digit in buffer (OPEN102)
                                  ActiveRelay =  RxUart0Buf[index]-0x30;   // make it bin
                                  index++;                                //collect period value (2 gigits)
                                  Relay_period =  ((RxUart0Buf[index++]-0x30) * 10 + ( RxUart0Buf[index]-0x30));  //
                                  if(Relay_period == 0)
                                  Relay_period = 2;           
                                  SendDebugMsg("OPEN by sever..\r\n\0");                                                       
                                  BLECurTask =  TASK_BLE_USER_MSG_OK; //open relay                                      
                                   return CONTINUE;       
                          }  
                           else                                                                                                          
                          {           
                               ptr = strstrf(RxUart0Buf,"CLOS");
                               if (ptr != NULL) 
                               {
                                      SendDebugMsg("Connection CLOSED - Redial..\r\n\0"); 
                                   //   Modem_Close_Connection();
                                   
                                   //   delay_ms(1000);                                                                      
                                  //    Init_Modem_Vars();   //to modem tasks  
                                      SearchDial = FALSE;
                                      ReDialNeeded = TRUE;
                                      ATO_needed = FALSE;   
                                      RING_PULSE_ACTIVE = FALSE;
                                      BLECurTask =  TASK_BLE_TO_MODEM;
                                      return CONTINUE; 
                               }
                                             
                          }     
                                                                                                                                                                               
                   //  if(ServerComOn == FALSE)  ??
                    
                     
                       ENABLE_UART0();
                       rx0_buff_len = 0; 
                   }     
                                                                                                                               
                 //=====================RING pulse handling==================================   
                 else   if(UNIT_TYPE == MODEM_UNIT)  
                 {
                      if(RING_PULSE_ACTIVE == TRUE ) // int Ring detedted?                    
                      {        
                             delay_ms(500); 
                             SendDebugMsg("RING pulse.\r\n\0"); 
                             
                             RING_PULSE_ACTIVE = FALSE;                                          
                             BLE_RESET_ON;         //shot ble                             
                      
                             heat_time = 30; // 1 sec 
                                                     
                             while((Modem_Message_Recieved == FALSE) && (heat_time > 0)); //wait for next rx int
                         
    //                         PrintNum((long) heat_time); 
    //                          PrintNum((long) Keep_Alive_timer); 
                             
                              if( Modem_Message_Recieved == TRUE)   //modem msg in buffer
                              {      
                                                                      
                                      DISABLE_RX_INT_UART0();         //keep msg in buffr for parsing  
                                       rx0_buff_len = 0;
                                      ptr = strstrf(RxUart0Buf,"OPEN");    //is it OPEN from server?  
                                                                                                       
                                      if (ptr != NULL)         //yes
                                      {       
                                              Modem_Message_Recieved = FALSE; //reset flag                                                      
                                              
                                              index = (ptr - RxUart0Buf) + 4;    // index point to relay index digit in buffer (OPEN102)
                                              ActiveRelay =  RxUart0Buf[index]-0x30;   // make relay index bin                                                                                  
                                              
                                              index++;                                //point to period value (2 gigits)
                                              Relay_period =  ((RxUart0Buf[index++]-0x30) * 10 + ( RxUart0Buf[index]-0x30));  //in sec 
                                              if(( Relay_period < 0) || ( Relay_period > 15))   //default - 1 sec
                                               Relay_period = 1;
                                               if(Relay_period == 0)  //special short period for HAMAMA
                                               Relay_period = 0.1;
                                                             
                                          //    SendDebugMsg("OPEN by sever..\r\n\0");                                                       
                                              BLECurTask =  TASK_BLE_USER_MSG_OK; //go to open relay                                      
                                              return CONTINUE;       
                                      }  
                                      
                                      else  ptr = strstrf(RxUart0Buf,"FOTA");             
                                      if(ptr != NULL)   //        
                                      {                                                            
                                            LED1_ON;  //red                                                                                                                                                     
                                             SendDebugMsg("FOTA process..\r\n\0"); 
                                             FOTA_process = TRUE; 
                                                                                                  
                                             RING_PULSE_ACTIVE = FALSE;    //ring signal
                                             SMS_ACTIVE = FALSE;
                                             MODEM_GOT_IP = TRUE;  
                                             BLECurTask =  TASK_BLE_TO_MODEM; 
                                             return CONTINUE;
                                      }                           
                                   
                                      else    // can be CLIP (dialing) - wait for CLIP
                                      {                                            
                                                 
                                              SendSModemString(plusplus,3);    //Modem to command mode - handle ring
                                              ChkNodemRespons("OK","-",40);    
                                              ATO_needed = TRUE;                              
                                              heat_time = 40;        //wait for possible CLIP                                                                                                             
                                              return WAIT;       //get back to task none to check ring string .+CLIP: "0522784873",129,"",0,"",0
                                                   
                                      } 
                                                                                                                
                                       
                              }  //if( Modem_Message_Recieved 
                          
                             else  //no message following = get into waiting loop
                             {
                                 SendSModemString(plusplus,3);      //ring? check in main loop for clip or else
                                 ChkNodemRespons("OK","-",40);  
                                 ATO_needed = TRUE;
                                 heat_time = 40;                   //wait for possible CLIP 
                             }
                                
                             return WAIT;  //back to main loop to check input
                      }  
                      

                    
                      if(ATO_needed == TRUE)   //due to +++  without any continuation -redial  
                      if(heat_time == 0)
                      {     
                            heat_time = 40;
                            while((PINA.2 == 0) && (heat_time > 0)); 
                            SendDebugMsg("New Dial needed..\r\n\0"); 
                            ATO_needed = FALSE;  
                            SendATCmd(AT_TCP_CLS);  //close socket  
                            ChkNodemRespons("OK","-",40);
                            
                            ReDialNeeded = TRUE;
       
                            BLECurTask =  TASK_BLE_TO_MODEM;  //redial by modem needed
                             return CONTINUE; 
                      }                                                
                       if(UNIT_TYPE == MODEM_UNIT)    //time to send keep alive nsg
                      {      
                           if(KeepAliveMsgNeeded == TRUE)
                           {
                                KeepAliveMsgNeeded = FALSE; 
                                SendATCmd(KEEP_ALIVE);       //send keep alive message  "1\r\nA\r\n"
                                Keep_Alive_timer = KeepAlivePeriod;    //60 sec
                             //   SendDebugMsg("Sent Keep Alive msg..\r\n\0");
                                return WAIT; 
                            }
                      } 
                      
                   
                  }                      
                                      
                  else  //nothing got in - loop around
                   return WAIT; 
                    
        break;  //task none
        
        case TASK_BLE_CONNECTED:
                 
                  if(heat_time == 0)
                 {                       
                     BLECurTask =  TASK_BLE_END_WAIT_FOR_USER_INPUT;  //time out- no user input following connect
                 }
                 else
                  if ( Message_Recieved == TRUE)   //msg in buffer
                 {   
                        Message_Recieved =  FALSE; 
                  //    -------------------------------------------------------------
//                       _putchar1('@');                       
//                        for(k = 0; k < rx_counter1; k++)   //debug - show received text
//                       _putchar1(rx_buffer1[k]); 
//                                 
//                        _putchar1('\r');
//                        _putchar1('\n'); 
                       //-------------------------------------------------------------  
                        
                          ptr = strstrf(rx_buffer1,"+LOS");   //
                          if(ptr != NULL)   //+LOST found        
                          {                       
                               BLE_connected = FALSE;  
                               BLECurTask = TASK_NONE;
                               SendDebugMsg("BLE disconnected..\r\n\0");
                          }                                     
                          else
                          {   
                                 heat_time = BLE_TIME_OUT;    //reset timer on every message 
                               
                                  ptr = strstrf(rx_buffer1,"OPEN");   //    // OPEN102                            
                                  if (ptr != NULL)   
                                  {       
                                          index = (ptr - rx_buffer1) + 4;    // index point to relay index digit in buffer (OPEN102)
                                          ActiveRelay =  rx_buffer1[index]-0x30 + 10;   // make it bin
                                          index++;                                //collect period value (2 gigits)
                                          Relay_period =  ((rx_buffer1[index++]-0x30) * 10 + ( RxUart0Buf[index]-0x30));  //
                                           if(( Relay_period < 0) || ( Relay_period > 15))   //default - 1 sec
                                           Relay_period = 2;
                                           if(Relay_period == 0)  //special short period for HAMAMA
                                           Relay_period = 0.1;                                   
                                          SendDebugMsg("BLE OPEN..\r\n\0");                                                       
                                          BLECurTask =  TASK_BLE_USER_MSG_OK; //open relay                                                                                     
                                             
                                           return CONTINUE;       
                                  }                                      
                                  
                                  else  k = CheckMessage(1);                         
                             
                                           
                                  if((k==11)||(k==12)||(k==13))   //open request approved -relay indication
                                  {
                                      //  SendDebugMsg("User gate request approved..\r\n\0"); 
                                          ActiveRelay = k - 10;
                                          BLECurTask = TASK_BLE_USER_MSG_OK; //open   
                                          Relay_period = 2;
                                          CurrentCount = 0; 
                                          
                                  }  
                                  else  if(k==0)
                                  {
                                     //   SendDebugMsg("RX error, repeat?..\r\n\0"); 
                                  }
                          }                                                                
                       //   rx_counter1 = 0;      //ready for next msg
                   
                          ENABLE_RX_INT_UART1();                       
                         
                 }  
                
                else
                {                          
                  return WAIT;   //continue looping
                }      
                                                                                                                   
         break;  
         
          case TASK_BLE_USER_MSG_OK:   
                
             
          if(CONTINUOUS_RELAY == TRUE)    //garge door contiuas activation  
          {      
               
                  if (BLE_RX_count > CurrentCount)  //rx int working - inc rx_counter1
                  {
                         CurrentCount = BLE_RX_count; 
                         Message_Recieved = FALSE;
                      //   SendDebugMsg("still working..\r\n\0"); 
                         heat_time = 20;   //add more time
                         return WAIT;
                  } 
                      
                  else  if( Message_Recieved == TRUE) //RX end - quiet period 
                  {                                              
                         Message_Recieved =  FALSE; 
                         heat_time = 0;                //end seesion-open relays 
                          OPEN_SESSION = FALSE;
                          CLOSE_SESSION = FALSE;                       
                    //     SendDebugMsg("ending garage door activation\r\n\0");                                                               
                  } 
                  else  heat_time = 30;   //more time to wait                   
             
           }  
          
         if(heat_time == 0)
         {    
                if (Relays_Activation_Needed == TRUE)
                {   
                   Relays_Activation_Needed = FALSE;
                    CloseRelay1();
                    CloseRelay2();
                }
                else if( ActiveRelay == 1)
                CloseRelay1();  
                else
                CloseRelay2();
                                       
                LED1_OFF;                          
                LED2_OFF;                                          
              
                CONTINUOUS_RELAY = FALSE;  
                OPEN_SESSION = FALSE;
                CLOSE_SESSION = FALSE;
                Open_Session_On = FALSE;
              
                 BLE_RX_count = 0;
                 TicksCount = 12;
               //  DISABLE_TIMER1() ;  //stop clock 
                 BLECurTask = TASK_BLE_END_CON; //open  
         } 
           
         else   return WAIT;   //continue looping
          
          break; 
           
            case TASK_BLE_END_CON:  
                   if(heat_time == 0)
                   {                                          
                    
                          BLE_connected = FALSE;  
                          ENABLE_RX_INT_UART1(); 
                         
                          ENABLE_RX_INT_UART0(); 
                          ENABLE_PA2_INT();   //RING pulse enabled  -------------------        
                          rx_counter1 = 0; 
                          rx0_buff_len = 0;  
                          KeepAliveMsgNeeded = FALSE;                                                                             
                          Keep_Alive_timer = KeepAlivePeriod;      
                          BLECurTask = TASK_NONE; 
                          SendDebugMsg("\r\nUnit Ready for new session..\r\n\0"); 
                        
                   } 
                   else   return WAIT;   //continue looping                                     
          break;

            
        default:
    }
    return CONTINUE;
}

void  BlueToothMain(void)
{
   char i; 
   
      if (GetNextBLETask() == WAIT)
     {
            return;
     }  

 
    switch (BLECurTask) 
    {    
    
         case TASK_BLE_CONNECTED:  

//                rnd = GetRandom();     //make long uniq number                  
//                rnd = atol(UnitIDstring) ;
//                random = &rnd;                  
//                GetEncryptedData((unsigned long)random, encrypted_data);
//                delay_ms(800);
//                SendDebugMsg(":reply:\0");
//                PrintNum2((unsigned long)*random);
//               SendDebugMsg(":end:\r\n\0");
//                SendDebugMsg("\r\n\0");

               heat_time = BLE_TIME_OUT;   //30 sec
               ENABLE_TIMER1();
               ENABLE_RX_INT_UART1(); 
               
               //----------modem list download check--------------------    
//               BLE_RESET_ON;                   
//                SendDebugMsg("to Modem..Server connection..\r\n\0"); 
//                Init_Modem_Vars();
               //-------------------------------------                                                                 
        break;  
        
              
        case TASK_BLE_END_WAIT_FOR_USER_INPUT:  
                            
                BLE_RESET();                
                BLE_connected = FALSE;  
                ENABLE_RX_INT_UART1();             
              //  rx_counter1 = 0;   
                 LED1_OFF;  
                 LED2_OFF;        
                BLECurTask = TASK_NONE; 
                SendDebugMsg("BLE Connect timeout - Disconnect..\r\n\0"); 
               
       
           //      #asm("jmp 0x0000");    //time out reset unit
        break;  
        
        case TASK_BLE_USER_MSG_OK:    //open relays
                
                 LED2_ON;  //green
       
                if( OPEN_SESSION == TRUE)// ||(CLOSE_SESSION == TRUE))  
                { 
                     OpenRelay1(); 
                  //   OPEN_SESSION = FALSE;
                     SendDebugMsg("Opening garage gate..\r\n\0"); 
                }
                else if(CLOSE_SESSION == TRUE)
                {  
                     OpenRelay2();  
                  //   CLOSE_SESSION = FALSE;                  
                     SendDebugMsg("Closing garage gate..\r\n\0"); 
                } 
                 
           
                 else 
                {     
                     if (Relays_Activation_Needed  == TRUE)
                     {       
                            SendDebugMsg("Botton-Activate both relays..\r\n\0"); 
                          
                            OpenRelay1(); 
                            OpenRelay2(); 
                     }
                     else if(ActiveRelay == 1)  
                     {
                         SendDebugMsg("Activate relay 1..\r\n\0"); 
                          OpenRelay1(); //   //default relay  
                       
                     }
                     else   if(ActiveRelay == 2)  
                     {
                         SendDebugMsg("Activate relay 2..\r\n\0"); 
                          OpenRelay2();    //default relay   
                         
                     } 
                      
                     if (Relays_Activation_Needed  == FALSE) 
                     {
                          sprintf(ComBuf,"6\r\nOPENED\r\n");     
                          BytesToSend = 11; //12 + i;
                          TransmitBuf(0);
                     }
                      
                }
                 
                heat_time = (int)Relay_period * 10; // value from server *10 = 100mS per count
                
                BLE_RX_count = 10;  
                Message_Recieved = FALSE;                               
                ENABLE_TIMER1();                             
                                        
                ENABLE_RX_INT_UART1();  
                
//                 if(( OPEN_SESSION == FALSE) && ( CLOSE_SESSION == FALSE))
//                 BLE_RESET(); 
                
                if(ModemAlive == TRUE)
                {
                    ENABLE_RX_INT_UART0();
                     Modem_Message_Recieved = FALSE; 
                }     
                               
                                                         
        break;  
        
          case TASK_BLE_END_CON:   
          
            //    SendDebugMsg("Delayed disconnect..\r\n\0");             
                heat_time = 0;//100; //time out 10 sec 
            //    ENABLE_TIMER1();                       
                                                         
          break;   
          
           case TASK_BLE_TO_MODEM:   
               if( ATO_needed == TRUE)   // no redial needed - checked in modem task 
               {  
                     ATO_needed = FALSE;  
                                                           
                   i = Modem_Server_Reconnect();                             
                   if(!i)
                   {      
                          SendDebugMsg("Reseting..\r\n\0");
                          SendATCmd(MODEM_SHDN); 
                           MODEM_GOT_IP = FALSE;
                          delay_ms(1000);                                                   
                          #asm("jmp 0");     //reset unit
                   } 
                   else
                   {
                         SendDebugMsg("Re-connected..\r\n\0");
                         Init_Modem_Vars(); 
                   }
                                       
                                   
               }                                                    
        
               else
               {  
                   SendDebugMsg("reconnection is NOT needed ..\r\n\0");
                   Init_Modem_Vars(); //re dial to server needed due to CLOSE or +++ - go to modem
               }             

          break;

      
        default:
               BLECurTask = TASK_NONE;
        break;
        
    } 
     return;     
 }