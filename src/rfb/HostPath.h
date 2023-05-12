#pragma once
class HostPath {
public:
  HostPath();
  HostPath(const char* path, int defaultPort = 5900);
  virtual ~HostPath();

  bool set(const char* path);

  bool isValid() const { return (m_path != 0); }
  bool isSshHostSpecified() const { return (m_sshHost != 0); }

  const char* get() const { return m_path; }
  const char* getSshHost() const { return m_sshHost; }
  const int getSshPort() const { return m_sshPort; }
  const char* getVncHost() const { return m_vncHost; }
  const int getVncPort() const { return m_vncPort; }

private:
  static const size_t m_SSH_USER_MAX_CHARS;
  static const size_t m_SSH_HOST_MAX_CHARS;
  static const size_t m_SSH_PORT_MAX_CHARS;
  static const size_t m_VNC_HOST_MAX_CHARS;
  static const size_t m_VNC_PORT_MAX_CHARS;
  static const size_t m_MAX_PATH_LEN;

  char* m_path;
  char* m_sshHost;
  int m_sshPort;
  char* m_vncHost;
  int m_vncPort;

 
  void clear();

  //
  // Parse m_path[] and store lengths if tokens in the specified array
  // of four size_t elements. Up to four tokens are detected: SSH host
  // name (optionally including a user name), SSH port number, MNT
  // host name, MNT port or display number. Port number tokens include
  // colons at the their beginning. Note that the '/' character
  // between SSH-related and MNT-related parts is not counted in any
  // token length.
  //
  // Examples: "user@server:22/mnt::443"  ->  11, 3, 3, 5
  //           "host/:1"                  ->   4, 0, 0, 2
  //           ":1"                       ->   0, 0, 0, 2
  //
  void parsePath(size_t results[]) const;

  //
  // Return true if m_sshHost[] and m_vncHost[] strings are valid,
  // false otherwise. It checks the lengths and character sets of the
  // strings. m_sshHost may be a null pointer, that does not make the
  // function return false. However, m_vncHost[] string is mandatory
  // so the function will return false if m_vncHost is a null pointer.
  //
  bool validateHostNames() const;

  int m_defaultPort;
};

