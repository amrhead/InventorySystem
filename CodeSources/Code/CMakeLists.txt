cmake_minimum_required (VERSION 3.6.0)
project (InventorySystem VERSION 1.0 LANGUAGES CXX)

option(OPTION_PCH "Enable Precompiled headers" ON)

find_package (CryCommon REQUIRED)

set (SourceGroup_PluginMain
	"StdAfx.cpp"
	"StdAfx.h"
	"Plugin.cpp"
	"Plugin.h"
	"ISystemCore.h"
)

set (SourceGroup_UIMgr
	"UIMgr.cpp"
	"UIMgr.h"
)

set (SourceGroup_InventoryMgr
	"InventoryMgr.cpp"
	"InventoryMgr.h"
)

set (SourceGroup_EntityMgr
	"EntityMgr.cpp"
	"EntityMgr.h"
	"ISimpleExtension.h"
)

set (SourceGroup_CryNodes
	"CryNodes.cpp"
)

source_group("Main" FILES ${SourceGroup_PluginMain})
source_group("UIMgr" FILES ${SourceGroup_UIMgr})
source_group("InventoryMgr" FILES ${SourceGroup_InventoryMgr})
source_group("EntityMgr" FILES ${SourceGroup_EntityMgr})
source_group("Nodes" FILES ${SourceGroup_CryNodes})

set (SOURCE
	${SourceGroup_PluginMain}
	${SourceGroup_UIMgr}
	${SourceGroup_InventoryMgr}
	${SourceGroup_EntityMgr}
	${SourceGroup_CryNodes}
)

if(OPTION_PCH AND MSVC)
	set (PRECOMPILED_SOURCE ${SOURCE})
	list(REMOVE_ITEM PRECOMPILED_SOURCE "StdAfx.cpp")

	set (PRECOMPILED_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pch)
	set_source_files_properties (${PRECOMPILED_SOURCE} PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_DEPENDS "${PRECOMPILED_BINARY}")
	set_source_files_properties (StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_OUTPUTS "${PRECOMPILED_BINARY}")
endif (OPTION_PCH AND MSVC)

add_library(${PROJECT_NAME} MODULE ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE CryCommon)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE Ntdll)
endif(WIN32)