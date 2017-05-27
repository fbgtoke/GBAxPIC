#include <stdio.h>
#include <tonc.h>

#define SC_DATA 0x0001
#define SD_DATA 0x0002
#define SI_DATA 0x0004
#define SO_DATA 0x0008

#define STATUS_CHECK if ((REG_RCNT & SI_DATA) == 0) goto comm_done;


int main() {

    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    tte_init_chr4c(0,           // BG 0
        BG_CBB(0)|BG_SBB(31),   // Charblock 0; screenblock 31
        0xF000,                 // Screen-entry offset
        bytes2word(1,2,0,0),    // Color attributes.
        CLR_YELLOW,             // Yellow text
        &verdana9Font,          // Verdana 9 font
        NULL                    // Use default chr4 renderer
        );
    
    // Initialize use of stdio.
    tte_init_con();

    tte_printf("#{es}");

    /*  4000134h - RCNT (R) - SIO Mode, usage in GENERAL-PURPOSE Mode (R/W)
		Interrupts can be requested when SI changes from HIGH to LOW, as General Purpose mode does not require a serial shift clock, this interrupt may be produced even when the GBA is in Stop (low power standby) state.

		  Bit   Expl.
		  0     SC Data Bit         (0=Low, 1=High)
		  1     SD Data Bit         (0=Low, 1=High)
		  2     SI Data Bit         (0=Low, 1=High)
		  3     SO Data Bit         (0=Low, 1=High)
		  4     SC Direction        (0=Input, 1=Output)
		  5     SD Direction        (0=Input, 1=Output)
		  6     SI Direction        (0=Input, 1=Output, but see below)
		  7     SO Direction        (0=Input, 1=Output)
		  8     SI Interrupt Enable (0=Disable, 1=Enable)
		  9-13  Not used
		  14    Must be "0" for General-Purpose Mode
		  15    Must be "1" for General-Purpose or JOYBUS Mode

		SI should be always used as Input to avoid problems with other hardware which does not expect data to be output there.
	*/
    int REG_RCNT_BASE = 0x8000 | 0x0000;
    REG_RCNT = REG_RCNT_BASE;

    int i = 0;

	while (1) {
        unsigned char data = 0;

        tte_printf("#{P:5,%d} WAITING FOR VALUE...", i);
        while ((REG_RCNT & SI_DATA) == 0);

        while ((REG_RCNT & SI_DATA) != 0) {
        
        	while ((REG_RCNT & SC_DATA) == 0) STATUS_CHECK;

    		data <<= 1;
    		int value = (REG_RCNT & SD_DATA) >> 1;
    		data |= value;

    		while ((REG_RCNT & SC_DATA) != 0) STATUS_CHECK; 
        }

        comm_done:
        
        tte_printf("#{P:200,%d} RECEIVED %d", i, data);
        i += 10;
    }

	return 0;
}
