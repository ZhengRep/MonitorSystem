#include "AutoImpersonator.h"

AutoImpersonator::AutoImpersonator(Impersonator* imp, LogWriter* log)
    : m_imp(imp),
    m_log(log)
{
    try {
        m_imp->impersonateAsLoggedUser();
    }
    catch (Exception& e) {
        m_log->error(e.getMessage());
    }
}

AutoImpersonator::~AutoImpersonator()
{
    try {
        m_imp->revertToSelf();
    }
    catch (Exception& e) {
        m_log->error(e.getMessage());
    }