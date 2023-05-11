#pragma once
#include "Control.h"

class BaseDialog
{
public:
	BaseDialog();
	BaseDialog(DWORD resourceId);
	BaseDialog(const TCHAR* resourceName);
	virtual ~BaseDialog();

public:
	//Method creates non modal window but not shows it
	void create();
	//Creates windows and show it in nonmodal/modal mode
	int show();
	int showModal();
	bool isCreated();
	void hide();
	virtual void kill(int code);
	//Dialog's parent is control???
	void setParent(Control* ctrlParent);
	void setResourceName(const TCHAR* resourceName);
	void setREsourceId(DWORD id);
	Control* getControl() { return &m_ctrlThis; };
	//Setup control by ID
	void setControlById(Control& control, DWORD id);
	void loadIcon(DWORD id);
	void updateIcon();

	//Puts this control foreground and activates it
	bool setForeground();

protected:
	void setDefaultPushButtton(UINT buttonId);

protected:
	//this methods must be override by child classes
	virtual BOOL onInitDialog();
	virtual BOOL onNotify(UINT controlID, LPARAM data);
	virtual BOOL onCommand(UINT controlID, UINT notificationID);
	virtual BOOL onClose();
	virtual	BOOL onDestroy();

	//this methods can be overrides by child classes
	virtual BOOL onDrawItem(WPARAM controlID, LPDRAWITEMSTRUCT dis);
	virtual void onMessageReveived(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//windows message proccessing method
	static INT_PTR CALLBACK dialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	TCHAR* getResourceName();
	
	Control m_ctrlThis;
private:
	TCHAR* m_resourceName;
	DWORD m_resourceId;
	Control* m_ctrlParent;

	BOOL m_isModal;
	BOOL m_isCreated;

	HICON m_hicon;
};

