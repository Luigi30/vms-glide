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

#include "glide/glide2x/cvg/glide/src/glide.h"
#include "glide/glide2x/cvg/glide/src/fxglide.h"

#include "errchk.h"

static int r0_status;
static $DESCRIPTOR (voodoo2_dev, "FXA0:");
static $DESCRIPTOR (voodoo2_section_name, "voodoo2_global");
unsigned short int channel;
static uint64 buf;

VOID_PQ mapped_addr_base;
static uint64 mapped_addr_length;

static uint32 data_to_read;

#define PCI_MEM_BASE 0x80000000000
