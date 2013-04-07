#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* includes for MQI */
#include <cmqc.h>
#include "logfile.h"

bool PutMessage(char *TQName,char *QMName,char *c_Message)
{
    /*   Declare MQI structures needed                                */
    MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
    MQMD     md = {MQMD_DEFAULT};    /* Message Descriptor            */
    MQPMO   pmo = {MQPMO_DEFAULT};   /* put message options           */
    /** note, sample uses defaults where it can **/

    MQHCONN  Hcon;                   /* connection handle             */
    MQHOBJ   Hobj;                   /* object handle                 */
    MQLONG   O_options;              /* MQOPEN options                */
    MQLONG   C_options;              /* MQCLOSE options               */
    MQLONG   CompCode;               /* completion code               */
    MQLONG   OpenCode;               /* MQOPEN completion code        */
    MQLONG   Reason;                 /* reason code                   */
    MQLONG   CReason;                /* reason code for MQCONN        */
    MQLONG   messlen;                /* message length                */

    /******************************************************************/
    /*                                                                */
    /*   Connect to queue manager                                     */
    /*                                                                */
    /******************************************************************/
    MQCONN(QMName,                  /* queue manager                  */
           &Hcon,                   /* connection handle              */
           &CompCode,               /* completion code                */
           &CReason);               /* reason code                    */
    /* report reason and stop if it failed     */
    if (CompCode == MQCC_FAILED)
    {
        //printf("MQCONN ended with reason code %d\n", CReason);
        WriteMQErrorLog("MQCONN ended with reason code ", CReason);
        return false;
        exit( (int)CReason );
    }
    strncpy(od.ObjectName, TQName, (size_t)MQ_Q_NAME_LENGTH);

    /******************************************************************/
    /*                                                                */
    /*   Open the target message queue for output                     */
    /*                                                                */
    /******************************************************************/
    O_options = MQOO_OUTPUT              /* open queue for output     */
                | MQOO_FAIL_IF_QUIESCING /* but not if MQM stopping   */
                ;                        /* = 0x2010 = 8208 decimal   */
    MQOPEN(Hcon,                      /* connection handle            */
           &od,                       /* object descriptor for queue  */
           O_options,                 /* open options                 */
           &Hobj,                     /* object handle                */
           &OpenCode,                 /* MQOPEN completion code       */
           &Reason);                  /* reason code                  */

    /* report reason, if any; stop if failed      */
    if (Reason != MQRC_NONE)
    {
        //printf("MQOPEN ended with reason code %d\n", Reason);
        WriteMQErrorLog("MQOPEN ended with reason code ", Reason);
        return false;
    }

    if (OpenCode == MQCC_FAILED)
    {
        //printf("unable to open queue for output\n");
        WriteMQErrorLog("unable to open queue for output ", OpenCode);
        return false;
    }

    if (CompCode != MQCC_FAILED)
    {
        messlen = strlen(c_Message);
        if (messlen > 0)
        {
            /**************************************************************/
            /* The following two statements are not required if the       */
            /* MQPMO_NEW_MSG_ID and MQPMO_NEW _CORREL_ID options are used */
            /**************************************************************/
            memcpy(md.MsgId,           /* reset MsgId to get a new one    */
                   MQMI_NONE, sizeof(md.MsgId) );

            memcpy(md.CorrelId,        /* reset CorrelId to get a new one */
                   MQCI_NONE, sizeof(md.CorrelId) );

            MQPUT(Hcon,                /* connection handle               */
                  Hobj,                /* object handle                   */
                  &md,                 /* message descriptor              */
                  &pmo,                /* default options (datagram)      */
                  messlen,             /* message length                  */
                  c_Message,           /* message buffer                  */
                  &CompCode,           /* completion code                 */
                  &Reason);            /* reason code                     */

            /* report reason, if any */
            if (Reason != MQRC_NONE)
            {
                //printf("MQPUT ended with reason code %d\n", Reason);
                WriteMQErrorLog("MQPUT ended with reason code ", Reason);
                return false;
            }
        }
        else   /* satisfy end condition when empty line is read */
            CompCode = MQCC_FAILED;
    }

    /******************************************************************/
    /*                                                                */
    /*   Close the target queue (if it was opened)                    */
    /*                                                                */
    /******************************************************************/
    if (OpenCode != MQCC_FAILED)
    {
        C_options = MQCO_NONE;        /* no close options             */

        MQCLOSE(Hcon,                   /* connection handle            */
                &Hobj,                  /* object handle                */
                C_options,
                &CompCode,              /* completion code              */
                &Reason);               /* reason code                  */

        /* report reason, if any     */
        if (Reason != MQRC_NONE)
        {
            //printf("MQCLOSE ended with reason code %d\n", Reason);
            WriteMQErrorLog("MQCLOSE ended with reason code ", Reason);
            return false;
        }
    }


    /******************************************************************/
    /*                                                                */
    /*   Disconnect from MQM if not already connected                 */
    /*                                                                */
    /******************************************************************/
    if (CReason != MQRC_ALREADY_CONNECTED)
    {
        MQDISC(&Hcon,                   /* connection handle            */
               &CompCode,               /* completion code              */
               &Reason);                /* reason code                  */

        /* report reason, if any     */
        if (Reason != MQRC_NONE)
        {
            //printf("MQDISC ended with reason code %d\n", Reason);
            WriteMQErrorLog("MQDISC ended with reason code ", Reason);
            return false;
        }
    }
    return true;
}
