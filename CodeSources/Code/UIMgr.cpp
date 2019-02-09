/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  21:56 : Created by mr-w1lde
- 06/27/2017  14:31 : Last edit by mr-w1lde

*************************************************************************/
#include <StdAfx.h>
#include "UIMgr.h"
#include "ISystemCore.h"

CUIController::CUIController() :
	pInventory(nullptr)

{
}

CUIController::~CUIController()
{
	pIS->pUI->pInventory->RemoveEventListener(this);
	SAFE_DELETE(pInventory);

}

void CUIController::AddUIItem(string id, string itemName, string itemType, string objPath, string itemUsing, string itemCount)
{
	SUIArguments args;
	args.AddArgument(id);
	args.AddArgument(itemName);
	args.AddArgument(itemType);
	args.AddArgument(objPath);
	args.AddArgument(itemUsing);
	args.AddArgument(itemCount);

	pInventory->CallFunction("AddItem", args);
}

void CUIController::UpdateItems()
{
	pInventory->CallFunction("UpdatePos");
}

void CUIController::RemoveButtons()
{
	pInventory->CallFunction("RemoveAllButtons");
}

void CUIController::GetInventoryList()
{
	XmlNodeRef rootRef = NULL;
	rootRef = pIS->pInventoryMgr->OpenXmlFile("PlayerItems.xml", true, false);

	if (!rootRef) {
		gEnv->pLog->LogError("NOT!");
	}

	for (int i = 0; i < rootRef->getChildCount(); i++) {
		XmlNodeRef currentItemRef = rootRef->getChild(i);

		pIS->pUI->AddUIItem(currentItemRef->getAttr("id"), currentItemRef->getAttr("name"), currentItemRef->getAttr("class"), currentItemRef->getAttr("objPath"), currentItemRef->getAttr("is_Using"), currentItemRef->getAttr("count"));
	}


	//pIS->pUI->UpdateItems();
}

void CUIController::SetRemoveButtonId(string id)
{
	SUIArguments args;
	args.AddArgument(id);

	pInventory->CallFunction("setRemoveBtnID", args);
}

void CUIController::InitializeInventorySlots()
{
	pInventory->CallFunction("InitializeSlots");
}

bool CUIController::InitUI()
{
	pInventory = gEnv->pFlashUI->GetUIElement("Inventory");
	if (!pInventory) {
		gEnv->pLog->LogError("[Inventory System] Inventory UI pointer wasn't load");
		return false;
	}
	else {
		pInventory->AddEventListener(this, "OnItemUse");
		pInventory->AddEventListener(this, "OnItemRemove");
		return true; 
	}

}

void CUIController::OnUIEvent(IUIElement * pSender, const SUIEventDesc & event, const SUIArguments & args)
{
	string sEvent = event.sName;

	if (sEvent == "onItemUse") {
		int argsCount = args.GetArgCount();
		string itemClassArg = NULL;
		string itemNameArg = NULL;

		args.GetArgNoConversation(0, itemClassArg);
		args.GetArgNoConversation(1, itemNameArg);

		XmlNodeRef rootRef = pIS->pInventoryMgr->GetInventoryRef();

		for (int i = 0; i < rootRef->getChildCount(); i++) {
			XmlNodeRef currentRef = rootRef->getChild(i);

			if (currentRef->getAttr("name") == itemNameArg && currentRef->getAttr("class") == itemClassArg) {
				pIS->pEntityMgr->AddAttachment(currentRef->getAttr("objPath"), currentRef->getAttr("attachTo"));
				break;
			}
		}

		
		//params.m_target = ;
		
		//pShowcastModel->AttachChild();
		
	}

	if (sEvent == "onItemRemove") {
		int argsCount = args.GetArgCount();
		string itemNameArg = NULL;

		args.GetArgNoConversation(0, itemNameArg);

		XmlNodeRef rootRef = pIS->pInventoryMgr->GetInventoryRef();

		for (int i = 0; i < rootRef->getChildCount(); i++) {
			XmlNodeRef currentRef = rootRef->getChild(i);

			if (currentRef->getAttr("name") == itemNameArg) {
				pIS->pEntityMgr->RemoveAttachment(currentRef->getAttr("attachTo"));
				break;
			}
		}

	}

	/*
	if (sEvent == "onRemoveButton") {
		int argsCount = args.GetArgCount();
		string arg = NULL;

		string sEvent = event.sName;

		args.GetArgNoConversation(0, arg);

		XmlNodeRef inventoryRef = pIS->pInventoryMgr->GetInventoryRef();

		for (int i = 0; inventoryRef->getChildCount(); i++) {
			XmlNodeRef currentRef = inventoryRef->getChild(i);

			string itemID = currentRef->getAttr("id");
			string idArg = ".remove";
			string endArg = itemID + idArg;

			if (arg == endArg) {
				pIS->pInventoryMgr->RemoveItemById(itemID);
				break;
			}
		}
	}

	if (sEvent == "OnItemButton") {
		int argsCount = args.GetArgCount();
		string arg = NULL;

		args.GetArgNoConversation(0, arg);

		pIS->pUI->SetRemoveButtonId(arg + ".remove");

		IEntity* pShowModelEntity = gEnv->pEntitySystem->FindEntityByName("EntityModelShowcast");
		pShowModelEntity->Hide(false);

		XmlNodeRef inventoryRef = pIS->pInventoryMgr->GetInventoryRef();

		for (int i = 0; i < inventoryRef->getChildCount(); i++) {
			XmlNodeRef currentRef = inventoryRef->getChild(i);

			if (currentRef->getAttr("id") == arg) {
				pIS->pEntityMgr->SetEntityModel("EntityModelShowcast", 0, currentRef->getAttr("objPath"));
				break;
			}
		}
	}
	*/
}







