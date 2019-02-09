/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  21:57 : Created by mr-w1lde
- 07/17/2017  19:30 : Last edit by mr-w1lde

*************************************************************************/
#pragma once

#include <StdAfx.h>
#include <CryEntitySystem\IEntity.h>
#include <CryEntitySystem\IEntitySystem.h>
#include <CryGame\IGameFramework.h>
#include <../CryAction/IActorSystem.h>
#include <CryAnimation\IAttachment.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <CrySerialization\Decorators\Resources.h>
#include <CryInput\IInput.h>

class CEntityMgr {
public:
	CEntityMgr();
	~CEntityMgr();

public:
	IEntity* EntitySpawn(string entityName, EEntityFlags entityFlag, string entityClass, Vec3 entityScale, Vec3 entityPos);
	Vec3 GetLocalPlayerPos();
	void HideObject(IEntity* pEntity);
	void SetEntityModel(string entityName, int nSlot, string objPath);
	void AddAttachment(string sObject, string sName);
	void RemoveAttachment(string sName);
};

class CSelectableObject : public CGameObjectExtensionHelper<CSelectableObject, IGameObjectExtension>,
	  public IEntityPropertyGroup, public IActionListener
{
public:
	CSelectableObject() :
		m_mass(0)
		, m_objectPath(NULL)
		, m_itemIco(NULL)
		, m_itemName(NULL)
		, m_itemId(NULL)
		, m_itemType(NULL)
		, b_isHide(false)
		, b_isPickedUp(false)
		, pActionMapMgr(nullptr)
		, pActionMap(nullptr)
		, m_count(NULL)
	{
		m_mass = 50;
		m_objectPath = "objects/weapons/rifle/rifle_tp.cgf";

		m_itemIco = "texture/";
		m_itemName = "Name";
		m_itemId = "itemBtn_";
		m_itemType = "Type";

		m_count = 1;

		b_isPickedUp = false;

		pActionMapMgr = gEnv->pGameFramework->GetIActionMapManager();
		bool isInit = pActionMapMgr->InitActionMaps("Libs/config/defaultprofile.xml");
		pActionMapMgr->Enable(true);
		pActionMapMgr->EnableActionMap("inventory", true);

	}
	virtual ~CSelectableObject() {
		m_actionListener = {};
	}

public:
	
	virtual bool Init(IGameObject* pGameObject) override {
		SetGameObject(pGameObject);

		GetEntity()->LoadGeometry(0, m_objectPath);

		m_actionListener.AddHandler(ActionId("pickup"), &CSelectableObject::OnObjectSelect);

		OnParamsUpdate();

		

		return true;
	}

	virtual void PostInit(IGameObject* pGameObject) override {
		GetEntity()->Activate(true);

		pGameObject->EnableUpdateSlot(this, 0);
	}

	virtual void InitClient(int channelId) override {}

	virtual void PostInitClient(int channelId) override {}

	virtual bool ReloadExtension(IGameObject* pGameObject, const SEntitySpawnParams& params) override {return false;}

	virtual void PostReloadExtension(IGameObject* pGameObject, const SEntitySpawnParams& params) override {}

	virtual void FullSerialize(TSerialize ser) override { }

	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int pflags) override {return false;}

	virtual NetworkAspectType GetNetSerializeAspects() { return eEA_All; }

	virtual void PostSerialize() override {}

	virtual void  SerializeSpawnInfo(TSerialize ser) override {}

	virtual ISerializableInfoPtr GetSpawnInfo() override { return nullptr; }

	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override 
	{
		GetEntity()->Hide(b_isHide);
		pActionMap = pActionMapMgr->GetActionMap("inventory");

			if ((gEnv->pGameFramework->GetClientEntity()->GetWorldPos().x >= GetEntity()->GetWorldPos().x - 1.f && gEnv->pGameFramework->GetClientEntity()->GetWorldPos().x <= GetEntity()->GetWorldPos().x + 1.f) && (gEnv->pGameFramework->GetClientEntity()->GetWorldPos().y >= GetEntity()->GetWorldPos().y - 1.f && gEnv->pGameFramework->GetClientEntity()->GetWorldPos().y <= GetEntity()->GetWorldPos().y + 1.f)) {
				if (pActionMap != nullptr) {
					GetGameObject()->CaptureActions(this);
					pActionMap->SetActionListener(GetEntityId());
					pActionMap->Enable(true);
				}
			}
		
	}


	virtual void HandleEvent(const SGameObjectEvent& event) override {}

	virtual void GameSerialize(TSerialize ser) { FullSerialize(ser); };                         //!< From IEntityComponent

	virtual void SetChannelId(uint16 id) override {}
	virtual void SetAuthority(bool auth) override {}

	virtual void OnAction(const ActionId &action, int activationMode, float value) override {
		m_actionListener.Dispatch(this, GetEntityId(), action, activationMode, value);
	};

	virtual const void* GetRMIBase() const override { return nullptr; }

	virtual void PostUpdate(float frameTime) override {}

	virtual void PostRemoteSpawn() override {}

	struct IEntityPropertyGroup* GetPropertyGroup() override { return this; }

	virtual const char*         GetLabel() const { return "Item Information"; }

	virtual void                SerializeProperties(Serialization::IArchive& archive)
	{
		archive(Serialization::ModelFilename(m_objectPath), "Model", "Model");
		archive(m_mass, "Mass", "Mass");
		archive(m_itemId, "ID", "ID");
		archive(m_itemName, "Name", "Name");
		archive(m_itemIco, "ICO", "ICO");
		archive(m_itemType, "Type", "Type");
		archive(m_count, "Count", "Count");
		archive(b_isPickedUp, "PickedUp", "PickedUp");
		archive(b_isHide, "Hidden", "Hidden");

		if (archive.isEdit()) {
			if (b_isPickedUp != true)
				this->PostInit(GetGameObject());
		}

		OnParamsUpdate();
	}

	void OnPhysParamsUpdate() {

		SEntityPhysicalizeParams physicalizeParams;

		physicalizeParams.mass = static_cast<float>(m_mass);
		physicalizeParams.type = pe_type::PE_RIGID;


		GetEntity()->Physicalize(physicalizeParams);
	}

	void OnParamsUpdate() {
		GetEntity()->LoadGeometry(0, m_objectPath);
		OnPhysParamsUpdate();
		GetEntity()->Hide(b_isHide);
	}

	bool OnObjectSelect(EntityId entityId, const ActionId& actionId, int activationMode, float value);

private:
	string m_objectPath;
	string m_itemIco;
	string m_itemName;
	string m_itemId;
	string m_itemType;
	int m_mass;
	int m_count;

	bool b_isPickedUp;
	bool b_isHide;

	TActionHandler<CSelectableObject> m_actionListener;
	IActionMapManager* pActionMapMgr;
	IActionMap *pActionMap;
};

