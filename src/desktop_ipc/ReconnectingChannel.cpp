#include "ReconnectingChannel.h"
#include "thread/AutoLock.h"
#include "ReconnectException.h"
#include "Utils/DateTime.h"

ReconnectingChannel::ReconnectingChannel(unsigned int timeOut, LogWriter* log)
  : m_timeOut(timeOut),
    m_isClosed(false),
    m_channel(0),
    m_oldChannel(0),
    m_chanWasChanged(false),
    m_log(log)
{
}

ReconnectingChannel::~ReconnectingChannel()
{
  if (m_channel != 0) {
    delete m_channel;
  }
  if (m_oldChannel != 0) {
    delete m_oldChannel;
  }
}

size_t ReconnectingChannel::read(void* buffer, size_t len)
{
  Channel* channel = getChannel(_T("read"));

  try {
    if (channel == 0) {
      throw Exception(_T("read() function stopped because transport has not been initialized yet."));
    }
    return channel->read(buffer, len);
  } catch (Exception& e) {
    m_log->error(e.getMessage());
    waitForReconnect(_T("read"), channel);
  }

  return 0; // Call by an out caller again!
}

size_t ReconnectingChannel::write(const void* buffer, size_t len)
{
  Channel* channel = getChannel(_T("write"));

  try {
    if (channel == 0) {
      throw Exception(_T("write() function stopped because transport has not been initialized yet."));
    }
    return channel->write(buffer, len);
  } catch (Exception& e) {
    m_log->error(e.getMessage());
    waitForReconnect(_T("write"), channel);
  }

  return 0; // Call by an out caller again!
}

void ReconnectingChannel::replaceChannel(Channel* newChannel)
{
  AutoLock al(&m_chanMut);
  if (m_channel != 0) {
    m_chanWasChanged = true; // Toggle to true except first initialization
  }
  if (m_oldChannel != 0) {
    delete m_channel; // This channel in this case is guaranted doesn't use.
  }
  m_oldChannel = m_channel;
  m_channel = newChannel; // Now we are the owner.
  m_timer.notify();
}

void ReconnectingChannel::close()
{
  AutoLock al(&m_chanMut);
  m_isClosed = true;
  if (m_channel != 0) {
    m_channel->close();
  }
  if (m_oldChannel != 0) {
    m_oldChannel->close();
  }
  m_timer.notify();
}

Channel* ReconnectingChannel::getChannel(const TCHAR* funName)
{
  if (m_isClosed) {
    StringStorage errMess;
    errMess.format(_T("The %s() function has failed: connection has already been closed."), funName);
    throw IOException(errMess.getString());
  }
  Channel* channel;
  {
    AutoLock al(&m_chanMut);
    // Clean m_oldChannel
    if (m_oldChannel != 0) {
      delete m_oldChannel;
      m_oldChannel = 0;
    }
    if (m_chanWasChanged) {
      m_chanWasChanged = false;
      StringStorage errMess;
      errMess.format(_T("Transport was reconnected outside from the %s() function. The %s() function at this time will be aborted."),funName, funName);
      throw ReconnectException(errMess.getString());
    }
    channel = m_channel;
  }
  return channel;
}

void ReconnectingChannel::waitForReconnect(const TCHAR* funName, Channel* channel)
{
  DateTime startTime = DateTime::now();
  bool success = false;
  while (!success) {
    unsigned int timeForWait = max((int)m_timeOut - (int)(DateTime::now() - startTime).getTime(), 0);
    if (timeForWait == 0 || m_isClosed) { // Break this function with
      // critical error
      StringStorage errMess;
      errMess.format(_T("The ReconnectingChannel::%s() function failed."), funName);
      throw IOException(errMess.getString());
    }
    m_timer.waitForEvent(timeForWait);
    AutoLock al(&m_chanMut);
    if (m_channel != channel) {
      m_chanWasChanged = false; // Reconnection catched in this place!
      success = true;
    }
  }
  m_log->info(_T("ReconnectingChannel was successfully reconnected."));
  if (channel != 0) { // If this is not the first initialization
    StringStorage errMess;
    errMess.format(_T("Transport was reconnected in the %s() function. The %s() function() at this time will be aborted"), funName, funName);
    throw ReconnectException(errMess.getString());
  }
}
