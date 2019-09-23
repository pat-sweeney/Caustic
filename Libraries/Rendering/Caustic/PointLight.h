#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\RefCount.h"

//**********************************************************************
// File: PointLight.h
// Contains declaration for CPointLight
//**********************************************************************

namespace Caustic
{
    class CPointLight : public IPointLight, public CRefCount
    {
        Vector3 m_pos;
        Vector3 m_color;
    public:
        CPointLight() :
            m_pos(0.0f, 0.0f, 0.0f),
            m_color(1.0f, 1.0f, 1.0f)
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IPointLight
        //**********************************************************************
        virtual void SetPosition(Vector3 &pos) override { m_pos = pos; };
        virtual Vector3 GetPosition() override { return m_pos; }
        virtual void SetColor(Vector3 &color) override { m_color = color; }
        virtual Vector3 GetColor() override { return m_color; }
    };

	CAUSTICAPI void CreatePointLight(Vector3 &pos, IPointLight **ppLight);
}
