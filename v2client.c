#define __NEW_STARLET 1

#include <stdio.h>
#include <stdlib.h>
#include <ssdef.h>
#include <stsdef.h>
#include <descrip.h>
#include <lib$routines.h>
#include <starlet.h>
#include <iodef.h>
#include <ints.h>

#include "errchk.h"

static int r0_status;
static $DESCRIPTOR (voodoo2_dev, "FXA0:");
static unsigned short int channel;

static uint32 data_to_read;

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
    uint32 buf;

    printf("V2CLIENT: Attempting to read one longword from FXA0: at %08X\n", address_to_read);

    r0_status = sys$qiow(0,
                         channel,
                         IO$_READPBLK,
                         NULL,
                         0,
                         0,
                         (void *)&buf,
                         address_to_read,
                         4,
                         0,
                         0,
                         0);
    errchk_sig (r0_status);

    printf("V2CLIENT: SYS$QIOW returned %08X\n", buf);

    printf("V2CLIENT: Releasing I/O channel\n");
    r0_status = sys$dassgn (channel);
    errchk_sig (r0_status);

}