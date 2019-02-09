/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  21:57 : Created by mr-w1lde
- 06/16/2017  13:07 : Last edit by mr-w1lde

*************************************************************************/
#include <StdAfx.h>
#include "InventoryMgr.h"
#include "ISystemCore.h"

CInventoryMgr::CInventoryMgr() :
	_InventoryFile(NULL),
	_fileDirect(NULL)
{
}

CInventoryMgr::~CInventoryMgr()
{
}

XmlNodeRef CInventoryMgr::OpenXmlFile(string sFilename, bool bReuseStrings, bool bEnablePatching)
{
	_fileDirect = "libs/inventory/" + sFilename;
	return _InventoryFile = gEnv->pSystem->GetXmlUtils()->LoadXmlFromFile(_fileDirect, bReuseStrings, bEnablePatching);
}

void CInventoryMgr::AddItem(string id, string itemName, string itemType, string objPath, bool itemUsing, int itemCount)
{
	XmlNodeRef rootRef = _InventoryFile;
	XmlNodeRef itemRef = rootRef->newChild("item");

	itemRef->setAttr("id", id);
	itemRef->setAttr("name", itemName);
	itemRef->setAttr("type", itemType);
	itemRef->setAttr("objPath", objPath);
	itemRef->setAttr("is_Using", itemUsing);
	itemRef->setAttr("count", itemCount);

	this->SaveInventory();
}

void CInventoryMgr::RemoveItemById(string id)
{
	for (int i = 0; i < _InventoryFile->getChildCount(); i++) {
		XmlNodeRef currentItem = _InventoryFile->getChild(i);

		string arg = currentItem->getAttr("id");
		int ing = i;

		if (arg == id) {
			_InventoryFile->deleteChildAt(i);
			pIS->pEntityMgr->EntitySpawn(currentItem->getAttr("name"), EEntityFlags::ENTITY_FLAG_SPAWNED, currentItem->getAttr("name"), Vec3(1,1,1), pIS->pEntityMgr->GetLocalPlayerPos());
			break;
		}

	}

	this->SaveInventory();

	IEntity* pShowModelEntity = gEnv->pEntitySystem->FindEntityByName("EntityModelShowcast");
	pShowModelEntity->Hide(true);

	pIS->pUI->RemoveButtons();
	pIS->pUI->GetInventoryList();
}



void CInventoryMgr::RemoveAllItems()
{
	_InventoryFile->removeAllChilds();
	_InventoryFile->removeAllAttributes();

	_InventoryFile->createNode("items");

	this->SaveInventory();
}

bool CInventoryMgr::SaveInventory()
{
	return _InventoryFile->saveToFile(_fileDirect);
}

XmlNodeRef CInventoryMgr::GetInventoryRef()
{
	return _InventoryFile;
}
