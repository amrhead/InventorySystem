/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  21:56 : Created by mr-w1lde
- 06/27/2017  14:31 : Last edit by mr-w1lde

*************************************************************************/
#pragma once
#include <StdAfx.h>
#include <CrySystem\Scaleform\IFlashUI.h>

class CUIItemBtnListener;
class CUIRemoveBtnListener;

class CUIController : public IUIElementEventListener {
public:
	CUIController();
	~CUIController();

public:
	void AddUIItem(string id, string itemName, string itemType, string objPath, string itemUsing, string itemCount);
	void UpdateItems();
	void RemoveButtons();
	void GetInventoryList();
	void SetRemoveButtonId(string id);
	void InitializeInventorySlots();
	bool InitUI();

	virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args);

public:
	IUIElement* pInventory;

private:

};

