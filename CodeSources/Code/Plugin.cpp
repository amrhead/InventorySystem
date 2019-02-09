#include "StdAfx.h"
#include "Plugin.h"
#include "ISystemCore.h"

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

IEntityRegistrator *IEntityRegistrator::g_pFirst = nullptr;
IEntityRegistrator *IEntityRegistrator::g_pLast = nullptr;

SSystemCore* pIS = new SSystemCore;

CPlugin::~CPlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	IEntityRegistrator* pTemp = IEntityRegistrator::g_pFirst;
	while (pTemp != nullptr)
	{
		pTemp->Unregister();
		pTemp = pTemp->m_pNext;
	}

	pIS->DeleteAllPtrs();
	SAFE_DELETE(pIS);
}

bool CPlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	pIS->Initialize();

	auto* pFramwork = gEnv->pGameFramework;


	IEntityClassRegistry::SEntityClassDesc clsDescSi;

	clsDescSi.sName = "SelectableItem";
	clsDescSi.editorClassInfo.sCategory = "Inventory";
	clsDescSi.editorClassInfo.sIcon = "itempickup.bmp";
	clsDescSi.editorClassInfo.bIconOnTop = true;

	CPlugin::RegisterEntityWithOwnComponent<CSelectableObject>("SelectableItem", clsDescSi);
	
	

	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this);
	gEnv->pHardwareMouse;

	return true;
}

void CPlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		// Register entities
		IEntityRegistrator* pTemp = IEntityRegistrator::g_pFirst;
		while (pTemp != nullptr)
		{
			pTemp->Register();
			pTemp = pTemp->m_pNext;
		}

		pIS->pInventoryMgr->OpenXmlFile("PlayerItems.xml", true, false);
		//pIS->pInventoryMgr->RemoveItemByName("Rifle");
		//pIS->pInventoryMgr->RemoveAllItems();

		//XmlNodeRef rootRef = pIS->pInventoryMgr->GetInventoryRef();

		//rootRef->deleteChildAt(0);

		//pIS->pInventoryMgr->SaveInventory();

//		XmlNodeRef additionsRef;

	//	pIS->pInventoryMgr->AddItem(1, "Rifle", "Weapon", "object/weapon/rifle/rifle_tp.cdf", additionsRef);

	//	XmlNodeRef damageRef = additionsRef->newChild("damage");
	//	damageRef->setAttr("value", 50);
		
	//	pIS->pInventoryMgr->SaveInventory();
	}

	break;
	case ESYSTEM_EVENT_LEVEL_LOAD_END: {
		pIS->pUI->InitUI();
	}
	break;
	}
}

CRYREGISTER_SINGLETON_CLASS(CPlugin)