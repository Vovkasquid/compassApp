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

package org.alljoyn.services.common;

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Status;

/**
 * A base class for service clients.
 * @deprecated
 */
@Deprecated
public interface ClientBase
{
    /**
     * Stop the AllJoyn session with the device.
     * @deprecated
     */
    @Deprecated
    public void disconnect();

    /**
     * Start an AllJoyn session with the device.
     * @deprecated
     * @return AllJoyn Bus Status
     */
    @Deprecated
    public Status connect();

    /**
     * Is there an open session with the device.
     * @deprecated
     * @return true if there is a session with the device.
     */
    @Deprecated
    public boolean isConnected();

    /**
     * Interface version
     * @deprecated
     * @return Interface version
     * @throws BusException indicating failure obtain Version property
     */
    @Deprecated
    public short getVersion() throws BusException;

    /**
     * The peer device's bus name, as advertised by Announcements
     * @deprecated
     * @return Unique bus name
     */
    @Deprecated
    public String getPeerName();

    /**
     * The id of the open session with the peer device.
     * @deprecated
     * @return AllJoyn session id
     */
    @Deprecated
    public int getSessionId();

    /**
     * Initialize client by passing the BusAttachment
     * @deprecated
     * @param busAttachment BusAttachment associated with this ClientBase instance
     * @throws Exception error indicating failure to initialize the client
     */
    @Deprecated
    void initBus(BusAttachment busAttachment) throws Exception;

}