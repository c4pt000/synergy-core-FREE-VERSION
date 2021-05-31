/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2004 Chris Schoeneman
 * 
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 * 
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/IInterface.h"
#include "base/Event.h"
#include "base/IEventQueue.h"
#include "base/EventTypes.h"

class IEventQueue;

namespace synergy {

//! Bidirectional stream interface
/*!
Defines the interface for all streams.
*/
class IStream : public IInterface {
public:
    IStream() { }

    //! @name manipulators
    //@{

    //! Close the stream
    /*!
    Closes the stream.  Pending input data and buffered output data
    are discarded.  Use \c flush() before \c close() to send buffered
    output data.  Attempts to \c read() after a close return 0,
    attempts to \c write() generate output error events, and attempts
    to \c flush() return immediately.
    */
    virtual void        close() = 0;

    //! Read from stream
    /*!
    Read up to \p n bytes into \p buffer, returning the number read
    (zero if no data is available or input is shutdown).  \p buffer
    may be NULL in which case the data is discarded.
    */
    virtual UInt32        read(void* buffer, UInt32 n) = 0;

    //! Write to stream
    /*!
    Write \c n bytes from \c buffer to the stream.  If this can't
    complete immediately it will block.  Data may be buffered in
    order to return more quickly.  A output error event is generated
    when writing fails.
    */
    virtual void        write(const void* buffer, UInt32 n) = 0;

    //! Flush the stream
    /*!
    Waits until all buffered data has been written to the stream.
    */
    virtual void        flush() = 0;

    //! Shutdown input
    /*!
    Shutdown the input side of the stream.  Any pending input data is
    discarded and further reads immediately return 0.
    */
    virtual void        shutdownInput() = 0;

    //! Shutdown output
    /*!
    Shutdown the output side of the stream.  Any buffered output data
    is discarded and further writes generate output error events.  Use
    \c flush() before \c shutdownOutput() to send buffered output data.
    */
    virtual void        shutdownOutput() = 0;

    //@}
    //! @name accessors
    //@{

    //! Get event target
    /*!
    Returns the event target for events generated by this stream.  It
    should be the source stream in a chain of stream filters.
    */
    virtual void*        getEventTarget() const = 0;

    //! Test if \c read() will succeed
    /*!
    Returns true iff an immediate \c read() will return data.  This
    may or may not be the same as \c getSize() > 0, depending on the
    stream type.
    */
    virtual bool        isReady() const = 0;

    //! Get bytes available to read
    /*!
    Returns a conservative estimate of the available bytes to read
    (i.e. a number not greater than the actual number of bytes).
    Some streams may not be able to determine this and will always
    return zero.
    */
    virtual UInt32        getSize() const = 0;

    //! Get name of the stream source if available
    /*!
    Returns a string containing a name of the source that provides
    the data. For network connections will return the host name.
    */
    virtual String        getSource() const = 0;

    //@}
};

}
