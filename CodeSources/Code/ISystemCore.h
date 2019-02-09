/*************************************************************************
Copyright (C), Stanislav Migunov: tfs.wilde@gmail.com , 2017.
-------------------------------------------------------------------------
History:
- 06/14/2017  22:03 : Created by mr-w1lde
- 06/16/2017  13:08 : Last edit by mr-w1lde

*************************************************************************/
#pragma once

#include <StdAfx.h>
#include "EntityMgr.h"
#include "UIMgr.h"
#include "InventoryMgr.h"

struct SSystemCore {

	CEntityMgr*			pEntityMgr;
	CUIController*		pUI;
	CInventoryMgr*		pInventoryMgr;

	inline void Initialize() {

		pEntityMgr		= new CEntityMgr;
		pUI				= new CUIController;
		pInventoryMgr	= new CInventoryMgr;
	}

	void DeleteAllPtrs() {
		SAFE_DELETE(pEntityMgr);
		SAFE_DELETE(pUI);
		SAFE_DELETE(pInventoryMgr)
	}
};

extern SSystemCore* pIS;