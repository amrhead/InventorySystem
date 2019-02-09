/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  22:34 : Created by mr-w1lde
- 06/16/2017  13:09 : Last edit by mr-w1lde

*************************************************************************/
#include <StdAfx.h>
#include <CryFlowGraph\IFlowBaseNode.h>
#include "ISystemCore.h"
#include <CryInput\IHardwareMouse.h>

namespace entityNodes {

	class CFlowNode_EntitySpawn : public CFlowBaseNode<eNCT_Instanced>
	{
		enum INPUTS { EIP_Spawn = 0, EIP_Name, EIP_Class, EIP_Scale, EIP_Pos };

		enum OUTPUTS { EOP_Spawned = 0 };

	public:
		CFlowNode_EntitySpawn(SActivationInfo * pActInfo) { }

		~CFlowNode_EntitySpawn() {}


		IFlowNodePtr Clone(SActivationInfo* pActInfo) { return new CFlowNode_EntitySpawn(pActInfo); }

		virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

		void GetConfiguration(SFlowNodeConfig& config)
		{
			static const SInputPortConfig in_ports[] =
			{
				InputPortConfig_Void("Spawn", _HELP("")),
				InputPortConfig<string>("Name", _HELP("")),
				InputPortConfig<string>("Class", _HELP("")),
				InputPortConfig<Vec3>("Scale", _HELP("")),
				InputPortConfig<Vec3>("Pos", _HELP("")),
				{ 0 }
			};
			static const SOutputPortConfig out_ports[] =
			{
				OutputPortConfig<bool>("Spawned", _HELP("Return result: is Overlay Active?")),
				{ 0 }
			};
			config.pInputPorts = in_ports;
			config.pOutputPorts = out_ports;
			config.sDescription = _HELP("");
			config.SetCategory(EFLN_APPROVED);
		}

		void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
		{
			switch (event)
			{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;
			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Spawn))
				{
					const string& sName = GetPortString(pActInfo, EIP_Name);
					const string& sClass = GetPortString(pActInfo, EIP_Class);
					const Vec3& sScale = GetPortVec3(pActInfo, EIP_Scale);
					const Vec3& sPos = GetPortVec3(pActInfo, EIP_Pos);

					IEntity* spawnedEntity = pIS->pEntityMgr->EntitySpawn(sName, ENTITY_FLAG_SPAWNED, sName, sScale, sPos);
					spawnedEntity->EnablePhysics(false);

					ActivateOutput(&m_actInfo, EOP_Spawned, true);
				}
			}
			break;
			}
		}

	protected:
		SActivationInfo m_actInfo;
	};

	class CFlowNode_SetEntityObject : public CFlowBaseNode<eNCT_Instanced>
	{
		enum INPUTS { EIP_Set = 0, EIP_Slot, EIP_Object, EIP_EntityName };

		enum OUTPUTS { EOP_Spawned = 0 };

	public:
		CFlowNode_SetEntityObject(SActivationInfo * pActInfo) { }

		~CFlowNode_SetEntityObject() {}


		IFlowNodePtr Clone(SActivationInfo* pActInfo) { return new CFlowNode_EntitySpawn(pActInfo); }

		virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

		void GetConfiguration(SFlowNodeConfig& config)
		{
			static const SInputPortConfig in_ports[] =
			{
				InputPortConfig_Void("Set", _HELP("")),
				InputPortConfig<int>("Slot", _HELP("")),
				InputPortConfig<string>("objPath", _HELP(""), "objPath", _UICONFIG("dt=file")),
				InputPortConfig<string>("EntityName", _HELP("")),
				{ 0 }
			};
			static const SOutputPortConfig out_ports[] =
			{
				OutputPortConfig<bool>("Seted", _HELP("Return result: is Overlay Active?")),
				{ 0 }
			};
			config.pInputPorts = in_ports;
			config.pOutputPorts = out_ports;
			config.sDescription = _HELP("");
			config.SetCategory(EFLN_APPROVED);
		}

		void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
		{
			switch (event)
			{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;
			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Set))
				{
					const string& sObjPath = GetPortString(pActInfo, EIP_Object);
					const int& slot = GetPortInt(pActInfo, EIP_Slot);
					const string& sEntityName = GetPortString(pActInfo, EIP_EntityName);

					pIS->pEntityMgr->SetEntityModel(sEntityName, slot, sObjPath);

					ActivateOutput(&m_actInfo, EOP_Spawned, true);
				}
			}
			break;
			}
		}

	protected:
		SActivationInfo m_actInfo;
	};

	class CFlowNode_AttachToPlayer : public CFlowBaseNode<eNCT_Instanced>
	{
		enum INPUTS { EIP_Attach = 0, EIP_Object, EIP_Index };

		enum OUTPUTS { EOP_Added = 0 };

	public:
		CFlowNode_AttachToPlayer(SActivationInfo * pActInfo) { }

		~CFlowNode_AttachToPlayer() {}


		IFlowNodePtr Clone(SActivationInfo* pActInfo) { return new CFlowNode_AttachToPlayer(pActInfo); }

		virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

		void GetConfiguration(SFlowNodeConfig& config)
		{
			static const SInputPortConfig in_ports[] =
			{
				InputPortConfig_Void("Attach", _HELP("")),
				InputPortConfig<string>("Object", _HELP("")),
				InputPortConfig<string>("Index", _HELP("")),
				{ 0 }
			};
			static const SOutputPortConfig out_ports[] =
			{
				OutputPortConfig<bool>("Added", _HELP("Return result: is Overlay Active?")),
				{ 0 }
			};
			config.pInputPorts = in_ports;
			config.pOutputPorts = out_ports;
			config.sDescription = _HELP("");
			config.SetCategory(EFLN_APPROVED);
		}

		void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
		{
			switch (event)
			{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;
			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Attach))
				{
					const string& iIndex = GetPortString(pActInfo, EIP_Index);
					const string sObject = GetPortString(pActInfo, EIP_Object);

					pIS->pEntityMgr->AddAttachment(sObject, iIndex);

					ActivateOutput(&m_actInfo, EOP_Added, true);
				}
			}
			break;
			}
		}

	protected:
		SActivationInfo m_actInfo;
	};


}



namespace UINodes {
	class CFlowNode_AddIteam : public CFlowBaseNode<eNCT_Instanced>
	{
		enum INPUTS { EIP_Add = 0, EIP_Item, EIP_Count, EIP_ICO, EIP_Class, EIP_ID };

		enum OUTPUTS { EOP_Added = 0 };

	public:
		CFlowNode_AddIteam(SActivationInfo * pActInfo) { }

		~CFlowNode_AddIteam() {}


		IFlowNodePtr Clone(SActivationInfo* pActInfo) { return new CFlowNode_AddIteam(pActInfo); }

		virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

		void GetConfiguration(SFlowNodeConfig& config)
		{
			static const SInputPortConfig in_ports[] =
			{
				InputPortConfig_Void("Add", _HELP("")),
				InputPortConfig<string>("Item", _HELP("")),
				InputPortConfig<string>("Count", _HELP("")),
				InputPortConfig<string>("ICO", _HELP("")),
				InputPortConfig<string>("Class", _HELP("")),
				InputPortConfig<string>("ID", _HELP("")),
				{ 0 }
			};
			static const SOutputPortConfig out_ports[] =
			{
				OutputPortConfig<bool>("Added", _HELP("Return result: is Overlay Active?")),
				{ 0 }
			};
			config.pInputPorts = in_ports;
			config.pOutputPorts = out_ports;
			config.sDescription = _HELP("");
			config.SetCategory(EFLN_APPROVED);
		}

		void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
		{
			switch (event)
			{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;
			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Add))
				{
					const string& sIteam = GetPortString(pActInfo, EIP_Item);
					const string& sCount = GetPortString(pActInfo, EIP_Count);
					const string& sIcoPath = GetPortString(pActInfo, EIP_ICO);
					const string& sClass = GetPortString(pActInfo, EIP_Class);
					const string& sID = GetPortString(pActInfo, EIP_ID);

					//string itemName, string itemCount, string itemICO, string itemClass, string itemID

					//pIS->pUI->AddUIItem(sIteam, sCount, sIcoPath, sClass, sID);

					ActivateOutput(&m_actInfo, EOP_Added, true);
				}
			}
			break;
			}
		}

	protected:
		SActivationInfo m_actInfo;
	};

	class CFlowNode_GetInventoryItems: public CFlowBaseNode<eNCT_Instanced>
	{
		enum INPUTS { EIP_Get = 0};

		enum OUTPUTS { EOP_Added = 0 };

	public:
		CFlowNode_GetInventoryItems(SActivationInfo * pActInfo) { }

		~CFlowNode_GetInventoryItems() {}


		IFlowNodePtr Clone(SActivationInfo* pActInfo) { return new CFlowNode_GetInventoryItems(pActInfo); }

		virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

		void GetConfiguration(SFlowNodeConfig& config)
		{
			static const SInputPortConfig in_ports[] =
			{
				InputPortConfig_Void("Get", _HELP("")),
				{ 0 }
			};
			static const SOutputPortConfig out_ports[] =
			{
				OutputPortConfig<bool>("Added", _HELP("Return result: is Overlay Active?")),
				{ 0 }
			};
			config.pInputPorts = in_ports;
			config.pOutputPorts = out_ports;
			config.sDescription = _HELP("");
			config.SetCategory(EFLN_APPROVED);
		}

		void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
		{
			switch (event)
			{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;
			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Get))
				{
					
					pIS->pUI->GetInventoryList();

					ActivateOutput(&m_actInfo, EOP_Added, true);
				}
			}
			break;
			}
		}

	protected:
		SActivationInfo m_actInfo;
	};

	
}

namespace InventoryNodes {

	class CFlowNode_RemoveItemById : public CFlowBaseNode<eNCT_Instanced>
	{
		enum INPUTS { EIP_Remove = 0, EIP_Id };

		enum OUTPUTS { EOP_Added = 0 };

	public:
		CFlowNode_RemoveItemById(SActivationInfo * pActInfo) { }

		~CFlowNode_RemoveItemById() {}


		IFlowNodePtr Clone(SActivationInfo* pActInfo) { return new CFlowNode_RemoveItemById(pActInfo); }

		virtual void GetMemoryUsage(ICrySizer * s) const { s->Add(*this); }

		void GetConfiguration(SFlowNodeConfig& config)
		{
			static const SInputPortConfig in_ports[] =
			{
				InputPortConfig_Void("Remove", _HELP("")),
				InputPortConfig<string>("ID", _HELP("")),
				{ 0 }
			};
			static const SOutputPortConfig out_ports[] =
			{
				OutputPortConfig<bool>("Added", _HELP("Return result: is Overlay Active?")),
				{ 0 }
			};
			config.pInputPorts = in_ports;
			config.pOutputPorts = out_ports;
			config.sDescription = _HELP("");
			config.SetCategory(EFLN_APPROVED);
		}

		void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
		{
			switch (event)
			{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;
			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Remove))
				{

					const string& sId = GetPortString(pActInfo, EIP_Id);

					pIS->pInventoryMgr->RemoveItemById(sId);

					ActivateOutput(&m_actInfo, EOP_Added, true);
				}
			}
			break;
			}
		}

	protected:
		SActivationInfo m_actInfo;
	};
}

REGISTER_FLOW_NODE_EX("Inventory:SpawnObject", entityNodes::CFlowNode_EntitySpawn, CFlowNode_EntitySpawn);
REGISTER_FLOW_NODE_EX("Inventory:SetModelObject", entityNodes::CFlowNode_SetEntityObject, CFlowNode_SetEntityObject);
REGISTER_FLOW_NODE_EX("Inventory:EntitySystem:Attach", entityNodes::CFlowNode_AttachToPlayer, CFlowNode_AttachToPlayer);

REGISTER_FLOW_NODE_EX("Inventory:AddItem", UINodes::CFlowNode_AddIteam, CFlowNode_AddIteam);
REGISTER_FLOW_NODE_EX("Inventory:GetInventoryItems", UINodes::CFlowNode_GetInventoryItems, CFlowNode_GetInventoryItems);

REGISTER_FLOW_NODE_EX("Inventory:RemoveItemByID", InventoryNodes::CFlowNode_RemoveItemById, CFlowNode_RemoveItemById);
