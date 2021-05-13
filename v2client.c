#include "v2client.h"

#define MMIO8(addr)		(*(volatile uint8 *)(addr))
#define MMIO16(addr)	(*(volatile uint16 *)(addr))
#define MMIO32(addr)	(*(volatile uint32 *)(addr))
#define MMIO64(addr)	(*(volatile uint64 *)(addr))

// Make sure Glide is either enabled or disabled in the synchronizer settings as needed.
void V2$ReadConfig(uint32 offset, uint32 *data)
{
    // This always returns a longword.
    sys$qiow(   
        0,
        channel,            // FXA0:
        IO$_READPBLK,       // command
        NULL,
        0,
        0,
        (void *)data,       // ptr to data
        offset,             // region to read from
        4,                  // longword
        TRUE,               // config space
        0,
        0
    );
}

int main()
{
    printf("OpenVMS Voodoo2 client program 0.01\n");

    printf("V2CLIENT: Attempting to open FXA0:\n");
    r0_status = sys$assign(&voodoo2_dev, &channel, 0, 0, 0);

    errchk_sig (r0_status);

    (void)printf ("Channel assigned to %-.*s\n",
                  voodoo2_dev.dsc$w_length,
                  voodoo2_dev.dsc$a_pointer);

    // Let's create a QIO request.
    uint32 address_to_read = (1 << 10) | 0;

    printf("V2CLIENT: Attempting to read one longword from FXA0: at %08X\n", address_to_read);

    r0_status = sys$qiow(0,
                         channel,
                         IO$_ACCESS,
                         NULL,
                         0,
                         0,
                         (void *)&buf,
                         0,
                         8,
                         0,
                         0,
                         0);
    errchk_sig (r0_status);

    printf("V2CLIENT: SYS$QIOW returned %X\n", buf);
    buf = buf | PCI_MEM_BASE;

    GENERIC_64 region_num;
    region_num.gen64$q_quadword = 0;

    uint64 voodoo2_phys_base_page = 0x48000000 / 8192;

    r0_status = sys$crmpsc_pfn_64(
        &region_num,
        buf/8192,                   // buf = the physical address of the PCI card
        (1048576*16) / 8192,
        PSL$C_USER,
        SEC$M_EXPREG|SEC$M_WRT,
        &mapped_addr_base,
        &mapped_addr_length
    );

    printf("V2CLIENT: sys$crmpsc_pfn_64 status %d\n", r0_status);

    if(r0_status == SS$_CREATED)
    {
        printf("Mapped Voodoo2 to %p length %08X\n", mapped_addr_base, mapped_addr_length);
        printf("%08X\n", MMIO32((uint32)mapped_addr_base));
    }

    GrHwConfiguration hwconfig;
    grSstQueryBoards(&hwconfig);
    printf("grSstQueryBoards sees %d SSTs\n", hwconfig.num_sst);

    if(hwconfig.num_sst == 0)
    {
        printf("Couldn't find any SSTs in your system.\n");
        return SS$_ABORT;
    }

    printf("performing grGlideInit\n");
    grGlideInit();
    grSstSelect(0);

    if(!grSstWinOpen(
        0,
        GR_RESOLUTION_640x480,
        GR_REFRESH_60Hz,
        GR_COLORFORMAT_ABGR,
        GR_ORIGIN_LOWER_LEFT,
        2,
        0
    ))
    {
        printf("grSstWinOpen failed!\n");
    }

    GrColor_t clearColor;
    grBufferClear(0xFF400000, 0, GR_WDEPTHVALUE_FARTHEST);

    float color = 255.0;
    
    guColorCombineFunction(GR_COLORCOMBINE_ITRGB);

    GrVertex vtx1, vtx2, vtx3;
    vtx1.x = 160; vtx1.y = 120;
    vtx1.r = color; vtx1.g = 0; vtx1.b = 0; vtx1.a = 0;
    vtx2.x = 480; vtx2.y = 180;
    vtx2.r = 0; vtx2.g = color; vtx2.b = 0; vtx2.a = 128;
    vtx3.x = 320; vtx3.y = 360;
    vtx3.r = 0; vtx3.g = 0; vtx3.b = color; vtx3.a = 255;

    grDrawTriangle(&vtx1, &vtx2, &vtx3);

    grBufferSwap(1);

    printf("V2CLIENT: Releasing I/O channel\n");
    r0_status = sys$dassgn (channel);
    errchk_sig (r0_status);

}