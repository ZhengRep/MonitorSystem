#pragma once
#include <vector>
#include "thread/LocalMutex.h"
#include "thread/AutoLock.h"

using namespace std;

template<class T> 
class SafeVector : public vector<T>, public LocalMutex
{
public:
  virtual ~SafeVector(){};
};

template<class T>
class ListenerContainer
{
public:
  void addListener(T listener) {
    AutoLock l(&m_listeners);

    typename vector<T>::iterator it;
    for (it = m_listeners.begin(); it != m_listeners.end(); it++) {
      T current = *it;
      if (current == listener) {
        return;
      } // if found
    } // for all listeners
    m_listeners.push_back(listener);
  } // void

  void removeAllListeners() {
    AutoLock l(&m_listeners);

    m_listeners.clear();
  }

  //
  // Removes listener from class listeners list
  //

  void removeListener(T listener) {
    AutoLock l(&m_listeners);

    typename vector<T>::iterator it;
    for (it = m_listeners.begin(); it != m_listeners.end(); it++) {
      T current = *it;
      if (current == listener) {
        m_listeners.erase(it);
        return;
      } // if found
    } // for all listeners
  } // void

public:
  SafeVector<T> m_listeners;
};

