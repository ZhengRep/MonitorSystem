#include "BaseDialog.h"

BaseDialog::BaseDialog()
	:m_ctrlParent(NULL), m_resourceName(0), m_resourceId(0), m_hicon(0)
{
}

BaseDialog::BaseDialog(DWORD resourceId)
	:m_ctrlParent(NULL), m_resourceName(0), m_resourceId(resourceId), m_hicon(0)
{
}

BaseDialog::BaseDialog(const TCHAR* resourceName)
	:m_ctrlParent(NULL), m_resourceName(0), m_resourceId(0), m_hicon(0)
{
	setResourceName(resourceName);
}

BaseDialog::~BaseDialog()
{
	if (m_hicon) {
		DeleteObject(m_hicon);
	}
	if (m_resourceName) {
		free(m_resourceName);
	}
}

void BaseDialog::create()
{
	HWND window, parentWindow = NULL;
	if (m_ctrlParent) {
		parentWindow = m_ctrlParent->getWindow();
	}
	window = CreateDialogParam(GetModuleHandle(0), getResourceName(), parentWindow, dialogProc, (LPARAM)this); //non modal

	m_isModal = false;
	_ASSERT(window);
}

int BaseDialog::show()
{
	if (m_ctrlThis.getWindow() == NULL) {
		create();
	}
	else {
		m_ctrlThis.setForeground();
	}
	return 0;
}

int BaseDialog::showModal()
{
	int result = 0;
	if (!m_ctrlThis.getWindow()) { // null
		m_isModal = true;
		HWND parentWindow = m_ctrlParent ? m_ctrlParent->getWindow() : NULL;
		result = DialogBoxParam(GetModuleHandle(0), getResourceName(), parentWindow, dialogProc, (LPARAM)this);
	}
	else {
		m_ctrlThis.setVisiable(true);
		m_ctrlThis.setForeground();
	}
	_ASSERT(result != -1);
	return result;
}

bool BaseDialog::isCreated()
{
	bool isInit = m_ctrlThis.getWindow() != 0;
	if (isInit) {
		return false;
	}
	return !!IsWindow(m_ctrlThis.getWindow());
}

void BaseDialog::hide()
{
	m_ctrlThis.setVisiable(false);
}

void BaseDialog::kill(int code)
{
	if (!m_isModal) {
		DestroyWindow(m_ctrlThis.getWindow());
	}
	else {
		EndDialog(m_ctrlThis.getWindow(), code);
	}
	m_ctrlThis.setWindow(NULL);
}

void BaseDialog::setParent(Control* ctrlParent)
{
	m_ctrlParent = ctrlParent;
}

void BaseDialog::setResourceName(const TCHAR* resourceName)
{
	if (m_resourceName) {
		free(m_resourceName);
	}
	m_resourceName = _tcsdup(resourceName);
}

void BaseDialog::setResourceId(DWORD id)
{
	m_resourceId = id;
}

void BaseDialog::setControlById(Control& control, DWORD id)
{
	control = GetDlgItem(m_ctrlThis.getWindow(), id);
}

void BaseDialog::loadIcon(DWORD id)
{
	if (m_hicon) {
		DeleteObject(m_hicon);
	}
	m_hicon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id));
}

void BaseDialog::updateIcon()
{
	if (m_hicon) {
		SetClassLongPtr(m_ctrlThis.getWindow(), GCLP_HICON, (LONG_PTR)m_hicon);
	}
}

bool BaseDialog::setForeground()
{
	return m_ctrlThis.setForeground();
}

void BaseDialog::setDefaultPushButton(UINT buttonId)
{
	SendMessage(m_ctrlThis.getWindow(), DM_SETDEFID, buttonId, 0);
}

BOOL BaseDialog::onInitDialog()
{
	return 0;
}

BOOL BaseDialog::onNotify(UINT controlID, LPARAM data)
{
	return 0;
}

BOOL BaseDialog::onCommand(UINT controlID, UINT notificationID)
{
	return 0;
}

BOOL BaseDialog::onClose()
{
	return 0;
}

BOOL BaseDialog::onDestroy()
{
	return 0;
}

BOOL BaseDialog::onDrawItem(WPARAM controlID, LPDRAWITEMSTRUCT dis)
{
	return TRUE;
}

void BaseDialog::onMessageReveived(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

INT_PTR BaseDialog::dialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BaseDialog* _this;
	BOOL result = FALSE;
	if (uMsg == WM_INITDIALOG) {
		_this = (BaseDialog*)lParam;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)_this);
		_this->m_ctrlThis.setWindow(hwnd);
		_this->updateIcon();
	}
	else {
		_this = (BaseDialog*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (_this == 0) {
			return FALSE;
		}
	}

	_this->onMessageReveived(uMsg, wParam, lParam);
	switch (uMsg) {
	case WM_INITDIALOG:
		result = _this->onInitDialog();
		break;
	case WM_NOTIFY:
		result = _this->onNotify(LOWORD(wParam), lParam);
		break;
	case WM_COMMAND:
		result = _this->onCommand(LOWORD(wParam), HIWORD(wParam));
		break;
	case WM_CLOSE:
		result = _this->onClose();
		break;
	case WM_DESTROY:
		result = _this->onDestroy();
		break;
	case WM_DRAWITEM:
		result = _this->onDrawItem(wParam, (LPDRAWITEMSTRUCT)lParam);
		break;
	}

	return result;
}

TCHAR* BaseDialog::getResourceName()
{
	if (m_resourceId) {
		return MAKEINTRESOURCE(m_resourceId);
	}
	return m_resourceName;
}
