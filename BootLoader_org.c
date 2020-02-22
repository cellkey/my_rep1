 #include "define.h"
 #include "twi.h"
void MemoryReadTest(char block, unsigned int start, unsigned int end);

char ptr;
bit BUF1_FULL;
bit BUF2_FULL;
bit Buf1_Read;
bit Buf2_Read;
//bit Folded = FALSE;
//bit LowMemFull = FALSE;
bit tmpBuf_full = FALSE;
//bit BufNotEmpty = FALSE;
bit E_O_F = 0;
bit HighMemFlag = 0;


#define PageSize 128
//#define RX_BUFFER_SIZE1 PageSize
char tmp_buf [150];    //larger than page size=128

#define highBit 2      //ATMLH030 memory chip

unsigned int GlobChksum;

//char rx_buffer1[50];
//char rx_buffer2[50];

unsigned char gIndx ;
unsigned char BytesCount;
unsigned int  RowsCount;
long MemIndex = 0;
extern bit  ServerComOn;
extern char Timer0_Ticks;
//extern bit bCheckRxBuf;
extern bit UpdateSession;
extern bit bWaitForModemAnswer;
extern bit FirmwareUpdateTime;
extern bit Modem_Message_Recieved;
extern int heat_time;

extern char RxUart0Buf[];    //100
extern char rx_buffer1[];
extern char chksumBuf[];
extern unsigned int rx0_buff_len;
extern unsigned int rx_counter1;
extern int ServerResponseTimeOut;


extern char ReadEEPCurrentAddress(void);
extern char twiReadEEP1Byte(char block,  unsigned int InternalAddr );
extern int twiWriteExtMemN(char block, unsigned int addr, unsigned char n, char *pdata); 
extern int twiWriteExtMem1Byte(char block, unsigned int addr, char pdata);
extern char twiReadEEPadd0(void);

void ShowHexString(unsigned char *message, char Lengh);




void Send_Char(unsigned char ch) //send one character via the serial port
{

	while (!(UCSR1A & 0x20))
		;    //wait xmit ready
	UDR1 = ch;                 //send the character
}
 ////now replace get byte
unsigned char Get_Char(void)  //get one character from the serial data source
{
  unsigned char rch, EEPchip;
 // unsigned char  filtered_char;


     if( HighMemFlag == 0)
      EEPchip = 0xA0;
      else
      EEPchip = (0xA0 | highBit);   //

    if(MemIndex == 0x10000)
    {
           rch =  twiReadEEP1Byte(1 , 0); //set address at upper section
            HighMemFlag = 1;
    }
    else
    rch =  ReadEEPCurrentAddress();  //read memory current address - self incrimented

        MemIndex++;
  //    ShowHexString(&rch, 1);   //gebugggggggggggggggggggggggg
       return (rch );

}

unsigned int Get_Word (void)   //read word of data from data source
{
    unsigned int ch_word;

//    ch_word = Get_Byte() * 256;   //msb byte
//    ch_word = ch_word + Get_Byte(); //lsb byte

       ch_word = (unsigned int)( Get_Char() * 256);   //msb byte
    ch_word = (unsigned int)(ch_word + Get_Char()); //lsb byte

    return ch_word;
}

unsigned char Get_Byte (void) //read byte of data from data source
{
    unsigned char ch_byte;
    //TxMsg("8");
    ch_byte = Get_Char() * 16;  //msb digit

    ch_byte = ch_byte + Get_Char();  //lsb digit

    return ch_byte;
}

//the next two functions would need to be changed to use a data source other than
//the serial port


char CharsFilter(char rch)
{
      char filtered_char;

   //filter character into hex digits or special characters
   if ((rch >= 0x30) && (rch<0x3A))
      filtered_char = rch-0x30;  //numeric digit
   else if (rch > 0x40)
      filtered_char = (rch & 0xdf) - 55;    //alpha digit
   else if (rch < 0x20)
      filtered_char = 0xff;        //dump the line feeds, etc.
   else
      filtered_char = rch;   //unconverted digit

   return (filtered_char);            //return character
}

char Ascii2Bin(char *str)
{
  char ptr, j, i, Bytes_count;

         
           if(gIndx == 0)
           {
               if(str[0]== '[')   //look for start
               {
                    tmp_buf[gIndx++] = str[0];   //[
                    tmp_buf[gIndx++] = str[1] - 0x30; //1                      
                    tmp_buf[gIndx++] = str[2]; 
                                      
                     _putchar1('S');  
                   //  _putchar1(str[2]);    //debug
                }
                else  _putchar1('@');   
                
           }

            j = 0;       //j = 0 - do not change
            while((str[j] != ':') && (j < 7)) //look for first ':'  
            {               
             //  _putchar1(str[j]);
                 j++;  
            }
            if(j == 7)
            {
                 _putchar1('!'); 
                PrintNum((long) RowsCount);
                  return 0;
            }
            else   //found ':'
            {
                RowsCount++;
                ptr = j;
                tmp_buf[gIndx++] = str[ptr++];    //save :
                //get num of data bytes in the current ro              
           
                 Bytes_count = (CharsFilter(str[(int)ptr++]) * 16) + CharsFilter(str[(int)ptr++]) ;

                tmp_buf[gIndx++] = Bytes_count ;        //set num of bytes

                if(Bytes_count > 0)  //regular  row
                {
                    i = 0;

                  do
                    {
                       tmp_buf[gIndx++] = (CharsFilter(str[ptr++]) * 16) + CharsFilter(str[ptr++]) ; //get byte
                       i++;
                    }
                    while (i < (Bytes_count + 4));    //plus address and checksum)

                    if( gIndx >= PageSize)  //buf is full. go to write it
                    {
                       tmpBuf_full = TRUE;

                    }
                }
                else      //count = 0- end of file
                {
                    tmp_buf[gIndx++] = 0;
                    tmp_buf[gIndx++] = 0;
                    tmp_buf[gIndx++] = 1;
                    tmp_buf[gIndx++] = 0xFF;

                    tmpBuf_full = TRUE;
                    E_O_F = 1;
                 //   _putchar1('E'); 
                  
                }
                Bytes_count = gIndx;
            }


            return Bytes_count;   //buffer size

}

#pragma used-
//READ ALL FILE FROM MEM AND CHECK ALL CS VALUES. MAKE SURE WRITING TO MEME OK
int ValidityCheck (unsigned int Saddress)
 {
	unsigned char num_bytes;       //line buffer number of bytes

	unsigned char Var;                   //general varaiable
	 int Errs = 0;                //error counter
	unsigned char CS;
    unsigned int Addr_start;
    unsigned char k;
    char EOF_code = 0;
    unsigned int RowsCounter; 
    unsigned int VerticalChksum;  
    unsigned char tVal;
    
     SendDebugMsg("File Validity check..\r\n\0") ; 
      SendDebugMsg("Rows expected = \0") ;
     PrintNum((long)RowsCount);

 	Var = 0;
  	Errs = 0;
    k=0;
    HighMemFlag = 0;
    MemIndex = 1;  
    VerticalChksum = 0;
    
     CS = 5;  
    ShowHexString(&chksumBuf[0], 1);
     ShowHexString(&chksumBuf[1], 1);
      ShowHexString(&chksumBuf[2], 1);
       ShowHexString(&chksumBuf[3], 1); 
     
     //convert vertical checksum to bin 
     tVal = ((unsigned char)(CharsFilter(chksumBuf[0]) * 16) + CharsFilter(chksumBuf[1]));  //calc most sig byte     
     GlobChksum = (unsigned int)(tVal *256);                                               //put in upper byte
     tVal = ((unsigned char)(CharsFilter(chksumBuf[2]) * 16) + CharsFilter(chksumBuf[3]));  //add leat sig byte     
     GlobChksum += tVal;                                                                    //to be checked later
    
      
     Var =  twiReadEEP1Byte(0 , Saddress);  //read mem address 0 
      ShowHexString(&Var,1);
     do 	//wait for colon - start of line od data
      {              
             Var = Get_Char();      //read mem current address
               ShowHexString(&Var,1);
             if (Var == 2)  //if eeprom only
             {
                  SendDebugMsg("Check EEP..\r\n\0") ;
             }

            else if (Var == 3)  // if this symbol is received, eeprom file first, then flash
            {
                     SendDebugMsg("Both..\r\n\0") ;
            }
          // else  if (Var == 1)  SendDebugMsg("Flash..\r\n\0") ;
           CS--;

      }  while ((Var != ']') && (CS > 0));

       if(CS == 0)  //we have found : -row start
       {
             Errs = 255;  //out
             SendDebugMsg("No file mark found..\r\n\0") ; 
          //  MemoryReadTest(0,0,8) ;
       }
       else
       {
           RowsCounter = 0;
          do  
            {
                     if((RowsCounter % 50) == 0)  
                     {
                         LED1_ON;
                          Send_Char('~');  
                     } 
                   
                    Var = Get_Char();    //get : of next row of hex
                     CS = Get_Char();
                    num_bytes = CS;              //get number of bytes  in row -hex file

                        Addr_start = Get_Word();     //get starting address of current line
                        CS += Addr_start & 0xFF;     //build checksum on CS
                        CS += ((Addr_start >> 8) & 0xFF);


                         EOF_code = Get_Char(); // get data type code 
                          
                        
                         if(EOF_code == 1)
                         {
                             RowsCounter++;
                             SendDebugMsg("found EOF..\r\n\0") ;
                             goto END;
                             //break;
                         } 
//                         else
//                         {
//                              if(EOF_code == 2)
//                              SendDebugMsg("EOF_code == 2\r\n\0") ;
//                         }
                         
                          CS += EOF_code;

                        //now get data line into buffer if not eof
                        for (k = 0;k < num_bytes;k++)
                        {
                             Var = Get_Char();
                              CS += Var;
                      
                        }

                        Var = Get_Char();                 //get checksum
                        if (((CS-1) ^ 0xFF) != Var )      //compar checksums
                        {                        
                            Errs++;
                            Send_Char('E');             //debug only
                            PrintNum((long)RowsCounter);  //debug only
                        } 
                        else
                        {
                           VerticalChksum += Var;
                        }

//                          Send_Char('~');        //debug only
                           RowsCounter++;
                              LED1_OFF;
//                   }
//                   else  //end of file mark? //  01FF
//                   {
//                          Var = Get_Char();
//                          Var = Get_Char();
//                          Var = Get_Char();
//                          if(Var != 1)
//                         Errs++;
//                        Send_Char('?');  //debug only
//                   }
                  END:
                   #asm("wdr");
            }  while((EOF_code != 1) && (RowsCounter < RowsCount));
         
            if(VerticalChksum == GlobChksum)
            {     
                 SendDebugMsg("Vertical Checksum OK..\n\r\0") ;              
            }
            else
            {
                   SendDebugMsg("\r\nVertical Checksum Error..!\n\r\0") ; 
                    Errs = 99;
            } 
             
            SendDebugMsg("\r\nValidity check errors = \0") ;
            PrintNum((long)Errs);
             PrintNum((long)MemIndex);   //debug 
            PrintNum((long)RowsCounter);   //debug

        }
      
          return Errs;
}

unsigned int RxUpdateFile(void)
{

        char i,k,m;
       unsigned int Waddress;
        bit  MemSec = 0; 
        char Buf1_Count = 0; 
        char Buf2_Count = 0;

       tmpBuf_full = FALSE;     
       BUF1_FULL = FALSE;
       BUF2_FULL = FALSE;
       Buf1_Read = TRUE;
       Buf2_Read = TRUE; 
   
       rx0_buff_len = 0;
       rx_counter0 = 0; 
       rx_counter1 = 0;
           
        MemSec = 0;
       Waddress = 0; 
       gIndx  = 0;
       RowsCount = 0; 
       UpdateSession = FALSE;
       E_O_F = 0;
       k = 0;
       m=0;
 
//       LED1_ON;            //debug    
       #asm("sei");  
  //-----------------debug -RX via cable- uart 2------------------------------
         ServerComOn = TRUE;      
         FirmwareUpdateTime = TRUE;   /////////test uart 0///////////////////       
          ENABLE_UART0();   //1??
 //--------------------------------------------------------
    
       
          heat_time  = 70;   //?? 15 S  max waiting  
          ENABLE_TIMER1();
             
         SendDebugMsg("\r\nRX update file..\r\n\0");  
        
         while(( UpdateSession == FALSE)  && (heat_time > 0))  //wait for start of file token to set flag
         #asm("wdr");

       if(heat_time == 0)     //file start not detected - abort
       {          
//             UpdateSession = FALSE;   not here because RX int beaviour
              ServerComOn = FALSE;      
             FirmwareUpdateTime = FALSE;   /////////test uart 0///////////////////       
              ENABLE_UART0();   //1??
              ENABLE_UART1();
             SendDebugMsg("\r\nFAILED Receiving FOTA file.!\r\n\0");
             delay_ms(50);
             return 0;
       } 
       
        heat_time  = 55;
        DISABLE_TIMER2(); 
        DISABLE_RX_INT_UART1();
        Modem_Message_Recieved = FALSE; 
        Timer0_Ticks = 10;
        bWaitForModemAnswer = TRUE;
     //   UBRR0L=0x5F;        //9600

   while(E_O_F == 0)
 //    while(1)
    {
              #asm("wdr")   
              
              
              
                 if(( BUF1_FULL == TRUE)&& (Buf1_Read == FALSE)&& (tmpBuf_full == FALSE))   //set in RX int when data byte recieved
                 {
                         BytesCount = Ascii2Bin(RxUart0Buf);  //14 ticks
                          rx0_buff_len = 0;
                         Buf1_Read = TRUE;                     
                         heat_time  = 25;      //set timer for count down
                          Buf1_Count++;
                 }

                if(( BUF2_FULL == TRUE)&& (Buf2_Read == FALSE)&& (tmpBuf_full == FALSE))
                {
                        BytesCount = Ascii2Bin(rx_buffer1);    
                        rx_counter1 = 0;
                        Buf2_Read = TRUE;
                        heat_time  = 25; 
                        Buf2_Count++;
                 }

                 if(tmpBuf_full == TRUE)  //buffer is ready to write into mem
                 {
                      tmpBuf_full = FALSE;
                       LED1_ON;

                     //  ShowHexString(tmp_buf, BytesCount);    //debug ??????????????? 
                                           
                            
                       if(BytesCount < PageSize) 
                       {
                                k = twiWriteExtMemN(MemSec , Waddress, BytesCount ,tmp_buf); 
                             //  PrintNum((long)BytesCount);     
                       }         
                       else
                       {
                              k = twiWriteExtMemN(MemSec, Waddress, PageSize ,tmp_buf);    //67 ticks 
                                                         
                              if(m == 0)  
                              {   
                              
//                                  ShowHexString(tmp_buf, 16); 
//                                   _putchar1('-');
                                 
                              }  
                       }
                      if(k)     //success
                      {
                            m++;
                           LED1_OFF;
                           
                           //set new address
                           if(BytesCount < PageSize)  //last page?                           
                                Waddress += BytesCount;                                                
                           else 
                               Waddress += PageSize;
                                                                                                                                                                     
                            if(Waddress == 0x00)  //last low section page was written- reached end of low mem +1 =10000h
                            {     
                                // PrintNum3((long)Waddress);
                                   MemSec = 1;   //now in upper mem
                                 Waddress = 0;  //address 0 again
                              
                                 
                            }
                               
                          
                          if(BytesCount > PageSize)      //extra bytes taken to next session
                          {
                              for(i = 0; i < (BytesCount - PageSize); i++)
                              tmp_buf[i] = tmp_buf[i + PageSize];  //save extra bytes to at start of buf
                              gIndx = i;
                          }
                          else  gIndx = 0;    //new buffer index

                          if(E_O_F == 1)     //end of file
                          {       
                              //    UBRR0L=0x2F;  //19200
                                    _putchar1('E');                   
                                    break;  //end of file recieved
                          }

                      }
                     else    //k = 0, failed
                     {
                       //   UBRR0L=0x2F; 
                          _putchar1('?');
                           heat_time = 0;  //error
                         break;
                     }
                 }

                if( Modem_Message_Recieved == TRUE)
                {
                   //    _putchar1('M');                   
                       if(rx0_buff_len > 0)   //handle partial buffer. last received bytes
                       {
                          BUF1_FULL = TRUE ;
                          Buf1_Read = FALSE;
                                                      
                       }

                       else if(rx_counter1 > 0)
                       {
                             BUF2_FULL = TRUE ;
                            Buf2_Read = FALSE; 
                           //  PrintNum((long)rx_counter1);                           
                       }
                       tmpBuf_full = FALSE;                                               
                }
              if( heat_time  == 0)
              {
                //  Waddress = 0;
                  _putchar1('H');
                  break;
              }
      }

 
       ENABLE_UART1();
      LED1_OFF;
       ServerResponseTimeOut = 70;

 //   UpdateSession = FALSE;    not here
  
       PrintNum((long)RowsCount); 
        
        if(MemSec == 1)      
          PrintNum((long)Waddress+0xFFFF);
        else
          PrintNum((long)Waddress);  
          
//           PrintNum((long)Buf1_Count);     //debug
//             PrintNum((long)Buf2_Count);  
//               PrintNum((long)m); 

      MemoryReadTest(0, 0,4);        
              
      Modem_Message_Recieved = FALSE; 
      rx0_buff_len = 0;
      Timer0_Ticks = 10;   
   //     UBRR0L=0x2F;    //19200   
   
   //  MemoryReadTest(0, 0,8);
   
      return Waddress;

   } 
   
  void MemoryReadTest(char block, unsigned int start, unsigned int end)
   {
         unsigned char k;
         unsigned int i;  
     //    char MemAdd =  0xA0;   
       
                
          delay_ms(50);
        //   k =  twiReadEEP1Byte(0 , start);  //block 0       
             k =  twiReadEEPadd0();  //
           ShowHexString(&k, 1);        
               for (i = 1; i < end; i++)
               {
                     k =  Get_Char(); 
                     ShowHexString(&k, 1);
                   //    _putchar1(k);
                     delay_ms(1);
               }  
//           }
//              else
//            _putchar1('?');

   }
   
   void TestMemory(void)
   {
           unsigned char k;
         unsigned char i = 0;  
        char MemAdd =  0xA0; 
        char buf[] = "123456789";  
        char data = 0x31; 
        
        
     //   k =  twiWriteExtMem1Byte(0, 0,data);
         k = twiWriteExtMemN(0, 0,9,buf);    //67 ticks 
         if(!k)   _putchar1('#');
        
         else 
         {    
             delay_ms(5);
              i =  twiReadEEP1Byte(0 , 0);  //block 0
              if(i)  ShowHexString(&i, 1);  
              else  _putchar1('?');
         }    
       

//           k =  twiReadEEP1Byte(0 , 1);  //block 0 
//         ShowHexString(&k, 1);
//           k =  twiReadEEP1Byte(0 , 2);  //block 0 
//         ShowHexString(&k, 1);
         
       
          {
              for(i = 1;i<9;i++)
              {      
                //  k = twiWriteExtMemN(0, i,1 ,buf);   //67 ticks 
                   k =  ReadEEPCurrentAddress();  //block 0  
                    ShowHexString(&k, 1); 
              }
          }
          
   }