/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  21:58 : Created by mr-w1lde
- 06/16/2017  13:08 : Last edit by mr-w1lde

*************************************************************************/
#include <StdAfx.h>
#include "EntityMgr.h"
#include "ISystemCore.h"

CEntityMgr::CEntityMgr()
{
}

CEntityMgr::~CEntityMgr()
{
}

IEntity* CEntityMgr::EntitySpawn(string entityName, EEntityFlags entityFlag, string entityClass, Vec3 entityScale, Vec3 entityPos)
{
	SEntitySpawnParams* entityParams = new SEntitySpawnParams;
	entityParams->sName = entityName;

	entityParams->nFlags = entityFlag;
	entityParams->pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(entityClass);
	entityParams->vScale = entityScale;
	entityParams->vPosition = entityPos;

	return gEnv->pEntitySystem->SpawnEntity(*entityParams, true);
}

Vec3 CEntityMgr::GetLocalPlayerPos()
{
	IEntity* pActorEntity =	gEnv->pGameFramework->GetClientActor()->GetEntity();
	return pActorEntity->GetPos();
}

void CEntityMgr::HideObject(IEntity * pEntity)
{
	pEntity->Hide(true);
}

void CEntityMgr::SetEntityModel(string entityName, int nSlot, string objPath)
{
	IEntity* Entity = gEnv->pEntitySystem->FindEntityByName(entityName);
	Entity->LoadGeometry(nSlot, objPath);

}

void CEntityMgr::AddAttachment(string sObject, string sName)
{
	IActor* pActor = gEnv->pGameFramework->GetClientActor();

	IEntity* pShowcastModel = gEnv->pEntitySystem->FindEntityByName("PlayerInventory");
	
	ICharacterInstance* pCharacter = gEnv->pEntitySystem->GetEntity(pShowcastModel->GetId())->GetCharacter(0);
	auto* pCharacterAttachments = pCharacter->GetIAttachmentManager()->GetInterfaceByName(sName);

	if (pCharacterAttachments != nullptr) {

		CCGFAttachment* pCGFAttachment = new CCGFAttachment();
		pCGFAttachment->pObj = gEnv->p3DEngine->LoadStatObj(sObject);

		pCharacterAttachments->AddBinding(pCGFAttachment);
	}

	
}

void CEntityMgr::RemoveAttachment(string sName)
{
	IEntity* pShowcastModel = gEnv->pEntitySystem->FindEntityByName("PlayerInventory");

	ICharacterInstance* pCharacter = gEnv->pEntitySystem->GetEntity(pShowcastModel->GetId())->GetCharacter(0);
	auto* pCharacterAttachments = pCharacter->GetIAttachmentManager()->GetInterfaceByName(sName);

	pCharacterAttachments->ClearBinding();
}

bool CSelectableObject::OnObjectSelect(EntityId entityId, const ActionId & actionId, int activationMode, float value)
{
	b_isPickedUp = true;
	GetEntity()->Hide(true);
	b_isHide = GetEntity()->IsHidden();
	GetGameObject()->DisableUpdateSlot(this, 0);
	pActionMap->Enable(false);
	GetGameObject()->ReleaseActions(this);

	pIS->pInventoryMgr->AddItem(m_itemId, m_itemName, m_itemType, m_objectPath, false, m_count);

	//TODO: ADD INTO XML FILE
	gEnv->pLog->Log("[Item] %s was picked up.", m_itemName);

	return true;
}
