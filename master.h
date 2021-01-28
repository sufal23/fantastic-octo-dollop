#include <ascii.h>

/* ASCII FRAME size */
#define ASCII_FRAME_SIZE 60  //CHANGE  

/* ASCII FRAME END CHARS */
#define CR 0x0D
#define LF 0x0A

/* ASCII FRAME */
uint8 ascii_frame[ASCII_FRAME_SIZE]; // !!!size = 255
uint8 data_count = 0;
uint8 send_count = 0;

/* functions codes parameters */
uint16 idata parameters[10];
uint8 byte_count = 0;

/* functions prototypes*/
void gen_start();
void gen_addr(uint8);
void gen_function(uint8);
void gen_lrc();
void clear_frame();
uint8 lrc_calc();
void tx_assci_frame();

/* functions Prototypes */
void ReadCoilStatus_01(uint8, uint16, uint16);
void ReadInputStatus_02(uint8, uint16, uint16);
void ReadHoldingRegisters_03(uint8, uint16, uint16);
void ReadInputRegisters_04(uint8, uint16, uint16);

/* aux functions */
void gen_start()
{
    ascii_frame[0] = 0x3A;
}

void gen_addr(uint8 val)  //Gen address
{
    ByteToAscii(val);
    ascii_frame[1] = ascii[0];
    ascii_frame[2] = ascii[1];
}

void gen_function(uint8 val) // Gen func code
{
    ByteToAscii(val);
    ascii_frame[3] = ascii[0];
    ascii_frame[4] = ascii[1];
}

void gen_lrc() // error check
{
    ByteToAscii(lrc_calc());
    ascii_frame[data_count++] = ascii[0];
    ascii_frame[data_count++] = ascii[1];
   
    ascii_frame[data_count++] = CR;
    ascii_frame[data_count++] = LF;    
}

void clear_frame()
{    
    uint8 i = 0;
    data_count = 0;
    send_count = 0;
    for ( i = 0; i < ASCII_FRAME_SIZE; i++ )
    {
        ascii_frame[i] = 0;
    }
}

void tx_assci_frame()
{     
    RS485En = 1; // set for tx
   
    send_count = 1;
    TI = 0;
    TB8 = 0;        
    SBUF = ascii_frame[0];
}

uint8 lrc_calc()  //error check longitudnal redundancy
{
    uint8 result = 0, i = 0;    
        
    for ( i = 1; i < data_count; i+=2 )
    {
        result += AsciiToByte(ascii_frame[i], ascii_frame[i+1]);
    }

    result = ~result;
    return ( result + 1 );
}

void tx_byte2(char byte)     //transmit a single byte
{      
    TI = 0;
    TB8 = 0;
    SBUF = byte;
}

/* functions Codes */
void ReadCoilStatus_01(uint8 dir, uint16 start, uint16 cant) // 0x01, OK
{
    int i = 0;

    gen_start();
    gen_addr(dir);
    gen_function(1);    // 0x01    
    data_count = 5;

    TwoByteToAscii(start - 1);
    for ( i = 3; i >= 0; i-- )
    {    
        ascii_frame[data_count++] = ascii[i];     
    }
    
    TwoByteToAscii(cant);
    for ( i = 3; i >= 0; i-- )
    {       
        ascii_frame[data_count++] = ascii[i];     
    }     
    gen_lrc();

    tx_assci_frame();
}

void ReadInputStatus_02(uint8 dir, uint16 start, uint16 cant) // 0x02, OK!!!
{
    int i = 0;
    
    gen_start();
    gen_addr(dir);
    gen_function(2);    // 0x02    
    data_count = 5;
    
    TwoByteToAscii(start - 1);
    for ( i = 3; i >= 0; i-- )
    {    
        ascii_frame[data_count++] = ascii[i];     
    }
    
    TwoByteToAscii(cant);
    for ( i = 3; i >= 0; i-- )
    {       
        ascii_frame[data_count++] = ascii[i];     
    }     
    gen_lrc();

    tx_assci_frame();
}


void ReadHoldingRegisters_03(uint8 dir, uint16 start, uint16 cant) // 0x03, OK!!
{
    int i = 0;
    
    gen_start();
    gen_dir(dir);
    gen_function(3);    // 0x03
    data_count = 5;
    
    TwoByteToAscii(start - 1);
    for ( i = 3; i >= 0; i-- )
    {    
        ascii_frame[data_count++] = ascii[i];     
    }
    
    TwoByteToAscii(cant);
    for ( i = 3; i >= 0; i-- )
    {       
        ascii_frame[data_count++] = ascii[i];     
    }     
    gen_lrc();

    tx_assci_frame();
}


void ReadInputRegisters_04(uint8 dir, uint16 start, uint16 cant)  // 0x04 , OK!!!
{
    int i = 0;
    
    gen_start();
    gen_dir(dir);
    gen_function(4);    // 0x04
    data_count = 5;
    
    TwoByteToAscii(start - 1);
    for ( i = 3; i >= 0; i-- )
    {    
        ascii_frame[data_count++] = ascii[i];     
    }
    
    TwoByteToAscii(cant);
    for ( i = 3; i >= 0; i-- )
    {       
        ascii_frame[data_count++] = ascii[i];     
    }     
    gen_lrc();

    tx_assci_frame();
}

