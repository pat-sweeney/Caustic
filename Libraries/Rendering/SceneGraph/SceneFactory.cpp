//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Base\Math\Matrix.h"
#include "ISceneFactory.h"
#include "SceneFactory.h"
#include "GroupElem.h"
#include "SceneGraph.h"
#include "MaterialElem.h"
#include "PointLightElem.h"
#include "MeshElem.h"

// Namespace: Caustic
namespace Caustic
{
	CAUSTICAPI void CreateSceneFactory(ISceneFactory **ppElem)
	{
		CSceneFactory *pFactory = new CSceneFactory();
		*ppElem = pFactory;
		(*ppElem)->AddRef();
	}
}
	
namespace Caustic
{
	CRefObj<ISceneFactory> CSceneFactory::s_factory;

	void CSceneFactory::CreateSceneGraph(ISceneGraph **ppGraph)
	{
		CRefObj<ISceneGroupElem> spGroup;
		CSceneFactory::Instance()->CreateGroupElem(&spGroup);
		std::unique_ptr<CSceneGraph> spGraphObj(new CSceneGraph(spGroup.p));
		*ppGraph = spGraphObj.release();
		(*ppGraph)->AddRef();
	}

	void CSceneFactory::CreateMaterialElem(ISceneMaterialElem **ppElem)
	{
		std::unique_ptr<CSceneMaterialElem> spMeshObj(new CSceneMaterialElem());
		CRefObj<IMaterialAttrib> spMaterial;
		m_spCausticFactory->CreateMaterial(&spMaterial);
		spMeshObj->SetMaterial(spMaterial.p);
		*ppElem = spMeshObj.release();
		(*ppElem)->AddRef();
	}

	void CSceneFactory::CreatePointLightElem(IScenePointLightElem **ppElem)
	{
		CRefObj<IPointLight> spPointLight;
		m_spCausticFactory->CreatePointLight(Vector3(0.0f, 0.0f, 0.0f), &spPointLight);
		std::unique_ptr<CPointLightElem> spPointLightObj(new CPointLightElem(spPointLight.p));
		*ppElem = spPointLightObj.release();
		(*ppElem)->AddRef();
	}

	void CSceneFactory::CreateGroupElem(ISceneGroupElem **ppElem)
	{
		std::unique_ptr<CSceneGroupElem> spGroupObj(new CSceneGroupElem());
		*ppElem = spGroupObj.release();
		(*ppElem)->AddRef();
	}

	void CSceneFactory::CreateMeshElem(ISceneMeshElem **ppElem)
	{
		std::unique_ptr<CSceneMeshElem> spMeshObj(new CSceneMeshElem());
		*ppElem = spMeshObj.release();
		(*ppElem)->AddRef();
	}
};
