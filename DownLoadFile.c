 #include "define.h"
 #include "twi.h"       
 
#define PageSize 16 //128
#define highBit 2      //ATMLH030 memory chip
 
void MemoryReadTest(unsigned int block, unsigned int start, unsigned int end);

char ptr;
bit BUF1_FULL;
bit BUF2_FULL;
bit Buf1_Read;
bit Buf2_Read;
bit ListUpdateNeeded = FALSE;;

unsigned char gIndx ;
unsigned char BytesCount;

long MemIndex = 0;
extern char rx_buffer1[MAX_RX_BUF_LEN];
extern char NumsCount[4];
extern bit  ServerComOn;

extern bit UpdateSession;
extern bit bWaitForModemAnswer;
//extern bit FirmwareUpdateTime;

extern eeprom unsigned int eExtMemUsersQuant;
extern unsigned int ExtMemUsersQuantity;
extern char ListBuf1[17];
extern char ListBuf2[17];
extern char ListBufCount1;
extern char ListBufCount2;

extern int heat_time;
extern char Timer0_Ticks;
extern char RxUart0Buf[];    //100
//extern char chksumBuf[];
extern unsigned int rx0_buff_len;
//extern unsigned char rx1_buff_len;
extern int ServerResponseTimeOut;

extern int twiWriteExtMemN(char block, unsigned int addr, unsigned char n, char *pdata);
extern char ReadEEPCurrentAddress(char EEPaddr);
extern char twiReadEEP1Byte(char block,  unsigned int InternalAddr );
extern void ShowHexString(unsigned char *message, char Lengh);
extern void SetTimer1_100ms(void);
extern unsigned int STR2BIN(unsigned char *str);
extern void UART1_WriteMsg( char *InStr);



/*
void Send_Char(unsigned char ch) //send one character via the serial port
{

	while (!(UCSR1A & 0x20))
		;    //wait xmit ready
	UDR2 = ch;                 //send the character
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
    rch =  ReadEEPCurrentAddress(EEPchip);  //read memory current address - self incrimented

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




//char Ascii2Bin(char *str)
//{
//  char ptr, j, i, count;
//
//         
//           if(gIndx == 0)
//           {
//                 if(str[0]== '[')  //look for start
//               {
//                    tmp_buf[gIndx++] = str[0];   //[
//                    tmp_buf[gIndx++] = str[1] - 0x30; //1                      
//                    tmp_buf[gIndx++] = str[2]; 
//                                      
//                     _putchar1('S');
//                }
//           }
//
//            j = 0;       //j = 0 - do not change
//            while((str[j] != ':') && (j < 7)) //look for first ':'  
//            {               
//             //  _putchar1(str[j]);
//                 j++;  
//            }
//            if(j == 7)
//            {
//                 _putchar1('!'); 
//                PrintNum((long) RowsCount);
//                  return 0;
//            }
//            else   //found ':'
//            {
//                RowsCount++;
//                ptr = j;
//                tmp_buf[gIndx++] = str[ptr++];    //get :
//                //get num of data bytes in the current ro              
//           
//                 count = (CharsFilter(str[(int)ptr++]) * 16) + CharsFilter(str[(int)ptr++]) ;
//
//                tmp_buf[gIndx++] = count ;        //num of bytes
//
//                if(count > 0)  //regular  row
//                {
//                    i = 0;
////                    ptr++;         //skip num of bytes
////                    ptr++;
//                  do
//                    {
//                       tmp_buf[gIndx++] = (CharsFilter(str[ptr++]) * 16) + CharsFilter(str[ptr++]) ; //get byte
//                       i++;
//                    }
//                    while (i < (count + 4));    //plus address and checksum)
//
//                    if( gIndx >= PageSize)  //buf is full. go to write it
//                    {
//                       tmpBuf_full = TRUE;
//
//                    }
//                }
//                else      //count = 0- end of file
//                {
//                    tmp_buf[gIndx++] = 0;
//                    tmp_buf[gIndx++] = 0;
//                    tmp_buf[gIndx++] = 1;
//                    tmp_buf[gIndx++] = 0xFF;
//
//                    tmpBuf_full = TRUE;
//                    E_O_F = 1;
//                    _putchar1('%');
//                }
//                j = gIndx;
//            }
//
//
//            return j;   //buffer size
//
//}

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
//    ShowHexString(&ChkSumStr[0], 1);
//     ShowHexString(&ChkSumStr[1], 1);
//      ShowHexString(&ChkSumStr[2], 1);
//       ShowHexString(&ChkSumStr[3], 1); 
     
     //convert vertical checksum to bin 
     tVal = ((unsigned char)(CharsFilter(chksumBuf[0]) * 16) + CharsFilter(chksumBuf[1]));  //calc most sig byte     
     GlobChksum = (unsigned int)(tVal *256);                                               //put in upper byte
     tVal = ((unsigned char)(CharsFilter(chksumBuf[2]) * 16) + CharsFilter(chksumBuf[3]));  //add leat sig byte     
     GlobChksum += tVal;                                                                    //to be checked later
    

     Var =  twiReadEEP1Byte(0 , Saddress);  //read mem address 0
     do 	//wait for colon - start of line od data
      {
           //    ShowHexString(&Var,1);

             Var = Get_Char();      //read mem current address

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
             MemoryReadTest(0,0,8) ;
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
             
            SendDebugMsg("\r\nValidity check ended..Errors= \0") ;
            PrintNum((long)Errs);
             PrintNum((long)MemIndex);   //debug 
            PrintNum((long)RowsCounter);   //debug

        }
      
          return Errs;
}
 
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
*/ 
 
unsigned int RxUpdateFile(void)
{

        char k;
       unsigned int Waddress; 
       unsigned int  RowsCount,tRowsCount;
       //char str[15];
            
       BUF1_FULL = FALSE;
       BUF2_FULL = FALSE;
       Buf1_Read = TRUE;
       Buf2_Read = TRUE; 
   
       rx0_buff_len = 0;
     				 
       ListBufCount1 = 0;
       ListBufCount2 = 0;
				 
           
     
       Waddress = 0; 
       gIndx  = 0;
       RowsCount = 0;
  //-----------------debug -RX via cable- uart 0------------------------------
//        ServerComOn = TRUE;     
//        ListUpdateNeeded = TRUE;   /////////test via uart0///////////////////      
//         ENABLE_UART0();  //cable input uart - instead of modem
 //--------------------------------------------------------
        SendDebugMsg("\r\nRX List file..\r\n\0"); 
       
         heat_time  = 70;   //?? 7 S  max waiting  
         SetTimer1_100ms(); 
         ENABLE_TIMER1();
                
            
         while(( UpdateSession == FALSE)  && (heat_time > 0))  //in RX int - wait for start of file token to set flag
         #asm("wdr");

       if(heat_time == 0)     //file start not detected - abort
       {          

             ENABLE_UART1();
             SendDebugMsg("\r\nFAILED Receiving file - time out..!\r\n\0");
          //   delay_ms(100); 
             LED1_OFF;
             return 0;
       } 
       
        heat_time  = 35;        
        Timer0_Ticks = 20;
        bWaitForModemAnswer = TRUE;   
        
                                                             
 	   RowsCount =  STR2BIN(NumsCount);
  	  tRowsCount =  RowsCount;
//	 sprintf(str, "\rList Count = %d\r\n\0",RowsCount); //                      
//     UART1_WriteMsg(str);                                                                 
   
   
   while(RowsCount > 0)
 
    {
              #asm("wdr")  
              k = 0;
                 if(( BUF1_FULL == TRUE) && (Buf1_Read == FALSE))   //set in RX int when data byte recieved
                 {
//                         LED1_ON; 
                         
                         k = twiWriteExtMemN(0, Waddress, PageSize ,ListBuf1);    //67 ticks
                         Buf1_Read = TRUE;                     
                         heat_time  = 20;      //set timer for count down 
                       if(k)     //success
                       {                                                                       
                           RowsCount--;
                           Waddress += PageSize;                                                                                                                                                                                                                                                                             
                      }

                 }

                if(( BUF2_FULL == TRUE) && (Buf2_Read == FALSE))
                {
                         LED1_OFF; 

                        k = twiWriteExtMemN(0, Waddress, PageSize ,ListBuf2);    //67 ticks
                        Buf2_Read = TRUE;
                        heat_time  = 20; 
                       if(k)     //success
                       {                                                                       
                           RowsCount--;
                           Waddress += PageSize;                                                                                                                                                                                                                                                                             
                       }
                }                                     
                                                     
              
                  if( heat_time  == 0)  //time out - failed
                  {
                      Waddress = 0;
                      _putchar1('!');
                      break;
                  }
      }

 
     
       LED1_OFF;
       ServerResponseTimeOut = 70;
             
      PrintNum((long)Waddress);  //last memory address (Waddress / 16 = users quantity)
      if(Waddress)
      {
          eExtMemUsersQuant = tRowsCount; //update user quantity in eeprom 
          ExtMemUsersQuantity = tRowsCount; //update user quantity in ram
      }
       rx0_buff_len = 0;
       Timer0_Ticks = 12;   
       
      MemoryReadTest(0,0,Waddress-1);  //debug - read list to check memory for debug

      return Waddress;

   } 
   
  void MemoryReadTest(unsigned int block, unsigned int start, unsigned int end)
   {
         unsigned char k;
         unsigned int i;  
        char MemAdd =  0xA0;   
        
                  
          
          delay_ms(100);
           k =  twiReadEEP1Byte(block , start);  //block 0
//           if(k == '[') 
//           {
            _putchar1(k);         
               for (i = 1; i < end; i++)
               {     
                     if((i % 16) == 0)  
                       _putchar1('\r'); 
                       
                     k =  ReadEEPCurrentAddress(MemAdd); 
                  //   ShowHexString(&k, 1);
                      _putchar1(k);
                    //   delay_ms(1);
               }  
//           }
//              else
//            _putchar1('?');

   }