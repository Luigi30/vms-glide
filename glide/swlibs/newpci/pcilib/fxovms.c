#include "fxovms.h"

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
static unsigned short int channel;

FxBool
pciPlatformInit(void)
{
  gCurPlatformIO = &ioProcsVMS;
  return FXTRUE;
}

/* I/O procedures */
static FxBool pciInitializeVMS(void)
{
    sys$assign(&voodoo2_dev, &channel, 0, 0, 0);
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
    printf("pciPortInByteVMS! port %04X data %08X\n", port);

    //assert(FALSE);
    return 0;
}
static FxU16 pciPortInWordVMS(FxU16 port)
{
    printf("pciPortInWordVMS! port %04X data %08X\n", port);

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

extern unsigned long long mapped_addr_base;

static FxBool pciMapLinearVMS(FxU32 busNumber, FxU32 physAddr,
                             unsigned long* linearAddr, FxU32* length)
{
    // uint32 buf;
    // // Make an I/O call to the driver and fetch the linear base.
    // sys$qiow(0,
    //     channel,
    //     IO$_ACCESS,
    //     NULL,
    //     0,
    //     0,
    //     (void *)&buf,
    //     0,
    //     4,
    //     0,
    //     0,
    //     0);

    // *linearAddr = buf;
    // *length = 1048576*16;
    // return FXTRUE;

    printf("voodoo2 is mapped to %08X\n", mapped_addr_base);

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