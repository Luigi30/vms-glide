#include "fxovms.h"

#define __NEW_STARLET 1

#include <FAR_POINTERS.H>
#include <stdio.h>
#include <stdlib.h>
#include <ssdef.h>
#include <stsdef.h>
#include <descrip.h>
#include <GEN64DEF.H>
#include <lib$routines.h>
#include <STARLET.H>
#include <iodef.h>
#include <ints.h>
#include <VADEF.H>
#include <PSLDEF.H>
#include <SECDEF.H>
#include <VA_RANGEDEF.H>

static char pciIdentVMS[] = "@#% fxPCI for OpenVMS";

static FxBool pciInitializeVMS(void);
static FxBool pciShutdownVMS(void);
static const char* pciIdentifierVMS(void);

static FxU8  pciPortInByteVMS(FxU16 port);
static FxU16 pciPortInWordVMS(FxU16 port);
static FxU32 pciPortInLongVMS(FxU16 port);
  
static FxBool pciPortOutByteVMS(FxU16 port, FxU8 data);
static FxBool pciPortOutWordVMS(FxU16 port, FxU16 data);
static FxBool pciPortOutLongVMS(FxU16 port, FxU32 data);

static FxBool pciMapLinearVMS(FxU32 busNumber, FxU32 physAddr,
                             unsigned long* linearAddr, FxU32* length);
static FxBool pciUnmapLinearVMS(unsigned long linearAddr, FxU32 length);

static FxBool pciSetPermissionVMS(const unsigned long addrBase, const FxU32 addrLen,
                                 const FxBool writePermP);

static FxBool pciMsrGetVMS(MSRInfo* in, MSRInfo* out);
static FxBool pciMsrSetVMS(MSRInfo* in, MSRInfo* out);

static FxBool pciOutputStringVMS(const char* msg);
static FxBool pciSetPassThroughBaseVMS(FxU32* baseAddr, FxU32 baseAddrLen);

VOID_PQ mapped_addr_base;
static uint64 mapped_addr_length;

const FxPlatformIOProcs ioProcsVMS = {
  pciInitializeVMS,
  pciShutdownVMS,
  pciIdentifierVMS,

  pciPortInByteVMS,
  pciPortInWordVMS,
  pciPortInLongVMS,

  pciPortOutByteVMS,
  pciPortOutWordVMS,
  pciPortOutLongVMS,

  pciMapLinearVMS,
  pciUnmapLinearVMS,
  pciSetPermissionVMS,

  pciMsrGetVMS,
  pciMsrSetVMS,

  pciOutputStringVMS,
  pciSetPassThroughBaseVMS
};

static $DESCRIPTOR (voodoo2_dev, "FXA0:");
unsigned short int channel;

FxBool
pciPlatformInit(void)
{
  gCurPlatformIO = &ioProcsVMS;
  return FXTRUE;
}

/* I/O procedures */

static int r0_status;
static $DESCRIPTOR (voodoo2_section_name, "voodoo2_global");
static uint64 buf;

static FxBool pciInitializeVMS(void)
{
    //sys$assign(&voodoo2_dev, &channel, 0, 0, 0);

    printf("pciInitializeVMS\n");

    #define PCI_MEM_BASE 0x80000000000

    printf("V2CLIENT: Attempting to open FXA0:\n");
    r0_status = sys$assign(&voodoo2_dev, &channel, 0, 0, 0);
    //errchk_sig (r0_status);

    if(r0_status != SS$_NORMAL)
    {
        return FXFALSE;
    }

    (void)printf ("Channel assigned to %-.*s\n",
                  voodoo2_dev.dsc$w_length,
                  voodoo2_dev.dsc$a_pointer);

    // Let's create a QIO request.
    uint32 address_to_read = (1 << 10) | 0;

    // printf("V2CLIENT: Attempting to read one longword from FXA0: at %08X\n", address_to_read);

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
    // errchk_sig (r0_status);

    //printf("V2CLIENT: SYS$QIOW returned %X\n", buf);
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

    if(r0_status != SS$_CREATED)
    {
        printf("Failed to map PFN: %d\n", r0_status);
        return(FXFALSE);
        //printf("Mapped Voodoo2 to %p length %08X\n", mapped_addr_base, mapped_addr_length);
        //printf("%08X\n", MMIO32((uint32)mapped_addr_base));
    }

    return FXTRUE;
}
static FxBool pciShutdownVMS(void)
{
    sys$dassgn(channel);
    return FXTRUE;
}
static const char* pciIdentifierVMS(void)
{
    return pciIdentVMS;
}

static FxU8  pciPortInByteVMS(FxU16 port)
{
    printf("pciPortInByteVMS! port %04X\n", port);

    //assert(FALSE);
    return 0;
}
static FxU16 pciPortInWordVMS(FxU16 port)
{
    printf("pciPortInWordVMS! port %04Xn", port);

    //assert(FALSE);
    return 0;
}
static FxU32 pciPortInLongVMS(FxU16 port)
{
    //printf("pciPortInLongVMS! port %04X data %08X\n", port);

    //assert(FALSE);
    return 0;
}
  
static FxBool pciPortOutByteVMS(FxU16 port, FxU8 data)
{
    printf("pciPortOutByteVMS! port %04X data %08X\n", port, data);

    //assert(FALSE);
    return 0;
}
static FxBool pciPortOutWordVMS(FxU16 port, FxU16 data)
{
    printf("pciPortOutWordVMS! port %04X data %08X\n", port, data);

    //assert(FALSE);
    return 0;
}
static FxBool pciPortOutLongVMS(FxU16 port, FxU32 data)
{
    //printf("pciPortOutLongVMS! port %04X data %08X\n", port, data);

    //assert(FALSE);
    return 0;
}

static FxBool pciMapLinearVMS(FxU32 busNumber, FxU32 physAddr,
                             unsigned long* linearAddr, FxU32* length)
{
    *linearAddr = (FxU32)mapped_addr_base;
    *length = 1048576*16;
    return FXTRUE;
}
static FxBool pciUnmapLinearVMS(unsigned long linearAddr, FxU32 length)
{
    // done by the kernel
    return FXTRUE;
}

static FxBool pciSetPermissionVMS(const unsigned long addrBase, const FxU32 addrLen,
                                 const FxBool writePermP)
{
    // done by the kernel
    return 0;
}

static FxBool pciMsrGetVMS(MSRInfo* in, MSRInfo* out)
{
    //assert(FALSE);
    return 0;
}
static FxBool pciMsrSetVMS(MSRInfo* in, MSRInfo* out)
{
    //assert(FALSE);
    return 0;
}

static FxBool pciOutputStringVMS(const char* msg)
{
    printf(msg);   
    return FXTRUE; 
}
static FxBool pciSetPassThroughBaseVMS(FxU32* baseAddr, FxU32 baseAddrLen)
{
    return FXFALSE; // not on VMS
}