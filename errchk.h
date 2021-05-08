#ifndef __jfd__errchk
#define __jfd__errchk

#include <stsdef.h>
#include <lib$routines.h>

#define errchk_ret(arg) if (!($VMS_STATUS_SUCCESS(arg))) \
			    return(arg);

#define errchk_sig(arg) if (!($VMS_STATUS_SUCCESS(arg))) \
			    (void)lib$signal(arg);

#define errchk_stop(arg) if (!($VMS_STATUS_SUCCESS(arg))) \
		    	     (void)lib$stop(arg);

#endif /* __jfd__errchk */