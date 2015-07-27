//See COPYRIGHT.txt for info on copyright
#include "mytypes.h"
#include "dma.h"

/* Just helps in making things look cleaner.  :) */
typedef unsigned char   uchar;
typedef unsigned int    uint;

/* Defines for accessing the upper and lower byte of an integer. */
#define LOW_BYTE(x)         (x & 0x00FF)
#define HI_BYTE(x)          ((x & 0xFF00) >> 8)

/* Quick-access registers and ports for each DMA channel. */
uchar MaskReg[8]   = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
uchar ModeReg[8]   = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
uchar ClearReg[8]  = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };

uchar PagePort[8]  = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
uchar AddrPort[8]  = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
uchar CountPort[8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

void _dma_xfer(uchar DMA_channel, unsigned char page, unsigned int offset, unsigned int length, uchar mode);

void dma_xfer(uchar channel, unsigned long address, unsigned int length, unsigned char read)
{
	unsigned char page=0, mode=0;
	unsigned int offset = 0;

	if(read)
		mode = 0x48 + channel;
	else
		mode = 0x44 + channel;

	page = address >> 16;
	offset = address & 0xFFFF;
	length--;

	_dma_xfer(channel, page, offset, length, mode);
}


void _dma_xfer(uchar DMA_channel, unsigned char page, unsigned int offset, unsigned int length, uchar mode)
{
    /* Don't let anyone else mess up what we're doing. */
    asm("cli");

    /* Set up the DMA channel so we can use it.  This tells the DMA */
    /* that we're going to be using this channel.  (It's masked) */
    outportb(MaskReg[DMA_channel], 0x04 | DMA_channel);

    /* Clear any data transfers that are currently executing. */
    outportb(ClearReg[DMA_channel], 0x00);

    /* Send the specified mode to the DMA. */
    outportb(ModeReg[DMA_channel], mode);

    /* Send the offset address.  The first byte is the low base offset, the */
    /* second byte is the high offset. */
    outportb(AddrPort[DMA_channel], LOW_BYTE(offset));
    outportb(AddrPort[DMA_channel], HI_BYTE(offset));

    /* Send the physical page that the data lies on. */
    outportb(PagePort[DMA_channel], page);

    /* Send the length of the data.  Again, low byte first. */
    outportb(CountPort[DMA_channel], LOW_BYTE(length));
    outportb(CountPort[DMA_channel], HI_BYTE(length));

    /* Ok, we're done.  Enable the DMA channel (clear the mask). */
    outportb(MaskReg[DMA_channel], DMA_channel);

    /* Re-enable interrupts before we leave. */
    asm("sti");
}




/*


typedef struct
{
    char page;
    unsigned int offset;
    unsigned int length;
} DMA_block;


/* Just helps in making things look cleaner.  :) *
typedef unsigned char   uchar;
typedef unsigned int    uint;

/* Defines for accessing the upper and lower byte of an integer. *
#define LOW_BYTE(x)         (x & 0x00FF)
#define HI_BYTE(x)          ((x & 0xFF00) >> 8)

/* Quick-access registers and ports for each DMA channel. *
uchar MaskReg[8]   = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
uchar ModeReg[8]   = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
uchar ClearReg[8]  = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };

uchar PagePort[8]  = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
uchar AddrPort[8]  = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
uchar CountPort[8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

/*
void LoadPageAndOffset(DMA_block *blk, char *data)
{
    unsigned int temp, segment, offset;
    unsigned long foo;
    segment = FP_SEG(data);
    offset  = FP_OFF(data);
    blk->page = (segment & 0xF000) >> 12;
    temp = (segment & 0x0FFF) << 4;
    foo = offset + temp;
    if (foo > 0xFFFF)
        blk->page++;
    blk->offset = (unsigned int)foo;
}
*

void StartDMA(uchar DMA_channel, DMA_block *blk, uchar mode)
{
    /* First, make sure our 'mode' is using the DMA channel specified. *
    mode |= DMA_channel;

    /* Don't let anyone else mess up what we're doing. *
    stopints();

    /* Set up the DMA channel so we can use it.  This tells the DMA *
    /* that we're going to be using this channel.  (It's masked) *
    outportb(MaskReg[DMA_channel], 0x04 | DMA_channel);

    /* Clear any data transfers that are currently executing. *
    outportb(ClearReg[DMA_channel], 0x00);

    /* Send the specified mode to the DMA. *
    outportb(ModeReg[DMA_channel], mode);

    /* Send the offset address.  The first byte is the low base offset, the *
    /* second byte is the high offset. *
    outportb(AddrPort[DMA_channel], LOW_BYTE(blk->offset));
    outportb(AddrPort[DMA_channel], HI_BYTE(blk->offset));

    /* Send the physical page that the data lies on. *
    outportb(PagePort[DMA_channel], blk->page);

    /* Send the length of the data.  Again, low byte first. *
    outportb(CountPort[DMA_channel], LOW_BYTE(blk->length));
    outportb(CountPort[DMA_channel], HI_BYTE(blk->length));

    /* Ok, we're done.  Enable the DMA channel (clear the mask). *
    outportb(MaskReg[DMA_channel], DMA_channel);

    /* Re-enable interrupts before we leave. *
    startints();
}

void PauseDMA(uchar DMA_channel)
{
    /* All we have to do is mask the DMA channel's bit on. *
    outportb(MaskReg[DMA_channel], 0x04 | DMA_channel);
}

void UnpauseDMA(uchar DMA_channel)
{
    /* Simply clear the mask, and the DMA continues where it left off. *
    outportb(MaskReg[DMA_channel], DMA_channel);
}

void StopDMA(uchar DMA_channel)
{
    /* We need to set the mask bit for this channel, and then clear the *
    /* selected channel.  Then we can clear the mask. *
    outportb(MaskReg[DMA_channel], 0x04 | DMA_channel);

    /* Send the clear command. *
    outportb(ClearReg[DMA_channel], 0x00);

    /* And clear the mask. *
    outportb(MaskReg[DMA_channel], DMA_channel);
}
int z;
unsigned short _AX;
uint DMAComplete(uchar DMA_channel)
{
    /* Register variables are compiled to use registers in C, not memory. *



    z = CountPort[DMA_channel];
    outportb(0x0C, 0xFF);

    /* This *MUST* be coded in Assembly!  I've tried my hardest to get it *
    /* into C, and I've had no success.  :(  (Well, at least under Borland.) *
    __asm(".intel_syntax\n"
"redo:\n"
"mov  dx,_z\n"
"in   al,dx\n"
"mov bl,al\n"
"in al,dx\n"
"mov  bh,al\n"
"in al,dx\n"
"mov ah,al\n"
"in al,dx\n"
"xchg ah,al\n"
"sub  bx,ax\n"
"cmp  bx,0x40\n"
"jg redo\n"
"cmp bx,0x0FFC0\n"
"jl redo\n"
"mov __AX,ax\n"
".att_syntax\n");

    return _AX;
}

*/


