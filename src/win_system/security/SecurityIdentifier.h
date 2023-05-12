#pragma once
#include "utils/winhdr.h"
#include "win_system/SystemException.h"

//Top level authority of a security identifier(SID)
enum Authority {
  /**
   * It only defines the Everyone well-known-SID.
   */
  Everyone,
  /**
   * It defines only the Local well-known-SID.
   */
   Local,
   /**
    * Specifies the Creator SID authority.
    * It defines the Creator Owner, Creator Group, and Creator Owner Server
    * well-known-SIDs.
    * These SIDs are used as placeholders in an access control list (ACL) and are replaced by the user,
    * group, and machine SIDs of the security principal.
    */
    Creator,
    /**
     * Specifies the Windows NT security subsystem SID authority. It defines all other SIDs in the forest.
     */
     Nt
};

/**
 * A SID consists of the following components:
 * The revision level of the SID structure;
 * A 48-bit identifier authority value that identifies the authority that issued the SID.
 * variable number of subauthority or relative identifier (RID) values that uniquely
 * identify the trustee relative to the authority that issued the SID.
 */
class SecurityIdentifier
{
public:
  SecurityIdentifier(SID* sid) throw(SystemException);
  SecurityIdentifier(const TCHAR* sidString);
  virtual ~SecurityIdentifier();
  bool isValid();
  void toString(StringStorage* sidString) throw(SystemException);
  static SecurityIdentifier* getProcessOwner(HANDLE processHandle) throw(SystemException);
  static SecurityIdentifier* createSidFromString(const TCHAR* sidString) throw(SystemException);
  SID* getSid() const;

private:
  SecurityIdentifier();
  static void getSidByString(const TCHAR* sidString, PSID* sid);
private:
  SID* m_sid;
};

