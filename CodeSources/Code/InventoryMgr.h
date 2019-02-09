/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  21:57 : Created by mr-w1lde
- 06/16/2017  13:08 : Last edit by mr-w1lde

*************************************************************************/
#pragma once

#include <StdAfx.h>
#include <CrySystem\XML\IXml.h>

class CInventoryMgr {
public:
	CInventoryMgr();
	~CInventoryMgr();

public:
	XmlNodeRef OpenXmlFile(string sFilename, bool bReuseStrings, bool bEnablePatching);
	//! AddItem - using SaveIventory in manualy after adding additions!
	//! additionVar - Additions for Item; using after adding a item and save after you wrote in yours additions.
	void AddItem(string id, string itemName, string itemType, string objPath, bool itemUsing, int itemCount);
	void RemoveItemById(string id);
	void RemoveAllItems();
	bool SaveInventory();
	XmlNodeRef GetInventoryRef();

private:
	XmlNodeRef _InventoryFile;
	string _fileDirect;
};