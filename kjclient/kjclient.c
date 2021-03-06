/*
 * @file
 */

/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/
#define AJ_MODULE BASIC_CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <aj_debug.h>
#include <alljoyn.h>

static const char ServiceName[] = "com.example.bus.compass";
static const char ServicePath[] = "/compass";
static const uint16_t ServicePort = 42;

/*
 * Buffer to hold the full service name. This buffer must be big enough to hold
 * a possible 255 characters plus a null terminator (256 bytes)
 */
static char fullServiceName[AJ_MAX_SERVICE_NAME_SIZE];

uint8_t dbgBASIC_CLIENT = 0;
/**
 * The interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
static const char* const sampleInterface[] = {
    "com.example.bus.compass",       /* The first entry is the interface name. */
    "?getCompassAzimuth azimuth>d", /* Method at index 0. */
    "?getCompassPitch pitch>d",     /* Method at index 1. */
    "?getCompassRoll roll>d",       /* Method at index 2. */

    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
static const AJ_InterfaceDescription sampleInterfaces[] = {
    sampleInterface,
    NULL
};

/**
 * Objects implemented by the application. The first member in the AJ_Object structure is the path.
 * The second is the collection of all interfaces at that path.
 */
static const AJ_Object AppObjects[] = {
    { ServicePath, sampleInterfaces },
    { NULL }
};

/*
 * The value of the arguments are the indices of the object path in AppObjects (above),
 * interface in sampleInterfaces (above), and member indices in the interface.
 * The 'cat' index is 2. The reason for this is as follows: The first entry in sampleInterface
 * is the interface name. This makes the first index (index 0 of the methods) the second string in
 * sampleInterface[]. The two dummy entries are indices 0 and 1. The index of the method we
 * implement for basic_client, 'cat', is 2 which is the fourth string in the array of strings
 * sampleInterface[].
 *
 * See also .\inc\aj_introspect.h
 */
#define GET_COMPASS_AZIMUTH AJ_PRX_MESSAGE_ID(0, 0, 0)
#define GET_COMPASS_PITCH AJ_PRX_MESSAGE_ID(0, 0, 1)
#define GET_COMPASS_ROLL AJ_PRX_MESSAGE_ID(0, 0, 2)


#define CONNECT_TIMEOUT    (1000 * 60)
#define UNMARSHAL_TIMEOUT  (1000 * 5)
#define METHOD_TIMEOUT     (100 * 10)

void MakeMethodCall(AJ_BusAttachment* bus, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    status = AJ_MarshalMethodCall(bus, &msg, GET_COMPASS_AZIMUTH, fullServiceName, sessionId, 0, METHOD_TIMEOUT);

    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }

    AJ_InfoPrintf(("MakeMethodCall() resulted in a status of 0x%04x.\n", status));
}

void MakeMethodCall1(AJ_BusAttachment* bus, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    status = AJ_MarshalMethodCall(bus, &msg, GET_COMPASS_PITCH, fullServiceName, sessionId, 0, METHOD_TIMEOUT);

    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }

    AJ_InfoPrintf(("MakeMethodCall1() resulted in a status of 0x%04x.\n", status));
}

void MakeMethodCall2(AJ_BusAttachment* bus, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    status = AJ_MarshalMethodCall(bus, &msg, GET_COMPASS_ROLL, fullServiceName, sessionId, 0, METHOD_TIMEOUT);

    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }

    AJ_InfoPrintf(("MakeMethodCall2() resulted in a status of 0x%04x.\n", status));
}



int AJ_Main(void)
{
    AJ_Status status = AJ_OK;
    AJ_BusAttachment bus;
    uint8_t connected = FALSE;
    uint8_t done = FALSE;
    uint32_t sessionId = 0;

    double azimuth = 0, pitch = 0, roll = 0;

    /*
     * One time initialization before calling any other AllJoyn APIs
     */
    AJ_Initialize();
    AJ_PrintXML(AppObjects);
    AJ_RegisterObjects(NULL, AppObjects);

    while (!done) {
        AJ_Message msg;



        if (!connected) {
            status = AJ_StartClientByName(&bus,
                                          NULL,
                                          CONNECT_TIMEOUT,
                                          FALSE,
                                          ServiceName,
                                          ServicePort,
                                          &sessionId,
                                          NULL,
                                          fullServiceName);

            if (status == AJ_OK) {
                AJ_InfoPrintf(("StartClient returned %d, sessionId=%u.\n", status, sessionId));
                connected = TRUE;

                MakeMethodCall(&bus, sessionId);
                /*MakeMethodCall1(&bus, sessionId);
                MakeMethodCall2(&bus, sessionId);*/
            } else {
                AJ_InfoPrintf(("StartClient returned 0x%04x.\n", status));
                break;
            }
        } else {
            //printf("Have connection!\n");
        }

        status = AJ_UnmarshalMsg(&bus, &msg, UNMARSHAL_TIMEOUT);

        if (AJ_ERR_TIMEOUT == status) {
            continue;
        }
        /*Print Result of previous Calls*/
        printf("Azimuth: %f   Pitch: %f   Roll: %f\n", azimuth, pitch, roll);
        
        if (AJ_OK == status) {
            switch (msg.msgId) {
            case AJ_REPLY_ID(GET_COMPASS_AZIMUTH):
                {
                    status = AJ_UnmarshalArgs(&msg,"d", &azimuth);

                    if (AJ_OK == status) {
                        //printf("azimuth: %f\n", azimuth);
                        MakeMethodCall1(&bus, sessionId);
                        //done = TRUE;
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        /* Try again because of the failure. */
                        MakeMethodCall(&bus, sessionId);
                    }
                }
                break;
            case AJ_REPLY_ID(GET_COMPASS_PITCH):
                {
                    status = AJ_UnmarshalArgs(&msg,"d", &pitch);

                    if (AJ_OK == status) {
                        //printf("pitch: %f\n", pitch);
                        MakeMethodCall2(&bus, sessionId);
                        //done = TRUE;
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        /* Try again because of the failure. */
                        MakeMethodCall1(&bus, sessionId);
                    }
                }
                break;
            case AJ_REPLY_ID(GET_COMPASS_ROLL):
                {
                    status = AJ_UnmarshalArgs(&msg,"d", &roll);

                    if (AJ_OK == status) {
                        //printf("roll: %f\n", roll);
                        MakeMethodCall(&bus, sessionId);
                        //done = TRUE;
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        /* Try again because of the failure. */
                        MakeMethodCall2(&bus, sessionId);
                    }
                }
                break;

            case AJ_SIGNAL_SESSION_LOST_WITH_REASON:
                /* A session was lost so return error to force a disconnect. */
                {
                    uint32_t id, reason;
                    AJ_UnmarshalArgs(&msg, "uu", &id, &reason);
                    AJ_AlwaysPrintf(("Session lost. ID = %u, reason = %u", id, reason));
                }
                status = AJ_ERR_SESSION_LOST;
                break;

            default:
                /* Pass to the built-in handlers. */
                status = AJ_BusHandleBusMessage(&msg);
                break;
            }
        }

        /* Messages MUST be discarded to free resources. */
        AJ_CloseMsg(&msg);

        if (status == AJ_ERR_SESSION_LOST) {
            AJ_AlwaysPrintf(("AllJoyn disconnect.\n"));
            AJ_Disconnect(&bus);
            exit(0);
        }
    }

    AJ_AlwaysPrintf(("Basic client exiting with status %d.\n", status));

    return status;
}

#ifdef AJ_MAIN
int main()
{
    return AJ_Main();
}
#endif