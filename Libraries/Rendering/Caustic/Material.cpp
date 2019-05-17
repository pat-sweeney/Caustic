//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Material.h"

namespace Caustic
{
	//**********************************************************************
	// Loads an IMaterial object from the specified stream
	// pStream - Stream to load from
	//**********************************************************************
	void CMaterial::Load(IStream *pStream)
	{
		DWORD dwBytesRead;
		pStream->Read(&AmbientColor.x, sizeof(AmbientColor.x), &dwBytesRead);
		pStream->Read(&AmbientColor.y, sizeof(AmbientColor.y), &dwBytesRead);
		pStream->Read(&AmbientColor.z, sizeof(AmbientColor.z), &dwBytesRead);
		pStream->Read(&DiffuseColor.x, sizeof(DiffuseColor.x), &dwBytesRead);
		pStream->Read(&DiffuseColor.y, sizeof(DiffuseColor.y), &dwBytesRead);
		pStream->Read(&DiffuseColor.z, sizeof(DiffuseColor.z), &dwBytesRead);
		pStream->Read(&SpecularColor.x, sizeof(SpecularColor.x), &dwBytesRead);
		pStream->Read(&SpecularColor.y, sizeof(SpecularColor.y), &dwBytesRead);
		pStream->Read(&SpecularColor.z, sizeof(SpecularColor.z), &dwBytesRead);
		pStream->Read(&SpecularExp, sizeof(SpecularExp), &dwBytesRead);
		pStream->Read(&Alpha, sizeof(Alpha), &dwBytesRead);
		pStream->Read(&MaterialID, sizeof(MaterialID), &dwBytesRead);
		uint32 len;
		pStream->Read(&len, sizeof(len), &dwBytesRead);
		AmbientTextureFN.resize(len + 1);
		pStream->Read((LPVOID)AmbientTextureFN.c_str(), len, &dwBytesRead);
		pStream->Read(&len, sizeof(len), &dwBytesRead);
		DiffuseTextureFN.resize(len + 1);
		pStream->Read((LPVOID)DiffuseTextureFN.c_str(), len, &dwBytesRead);
		pStream->Read(&len, sizeof(len), &dwBytesRead);
		SpecularTextureFN.resize(len + 1);
		pStream->Read((LPVOID)SpecularTextureFN.c_str(), len, &dwBytesRead);
	}

	//**********************************************************************
	// Stores an IMaterial object to the specified stream
	// pStream - Stream to store to
	//**********************************************************************
	void CMaterial::Store(IStream *pStream)
	{
		DWORD dwBytesWritten;
		pStream->Write(&AmbientColor.x, sizeof(AmbientColor.x), &dwBytesWritten);
		pStream->Write(&AmbientColor.y, sizeof(AmbientColor.y), &dwBytesWritten);
		pStream->Write(&AmbientColor.z, sizeof(AmbientColor.z), &dwBytesWritten);
		pStream->Write(&DiffuseColor.x, sizeof(DiffuseColor.x), &dwBytesWritten);
		pStream->Write(&DiffuseColor.y, sizeof(DiffuseColor.y), &dwBytesWritten);
		pStream->Write(&DiffuseColor.z, sizeof(DiffuseColor.z), &dwBytesWritten);
		pStream->Write(&SpecularColor.x, sizeof(SpecularColor.x), &dwBytesWritten);
		pStream->Write(&SpecularColor.y, sizeof(SpecularColor.y), &dwBytesWritten);
		pStream->Write(&SpecularColor.z, sizeof(SpecularColor.z), &dwBytesWritten);
		pStream->Write(&SpecularExp, sizeof(SpecularExp), &dwBytesWritten);
		pStream->Write(&Alpha, sizeof(Alpha), &dwBytesWritten);
		pStream->Write(&MaterialID, sizeof(MaterialID), &dwBytesWritten);
		uint32 len = (uint32)AmbientTextureFN.length();
		pStream->Write(&len, sizeof(len), &dwBytesWritten);
		pStream->Write((void*)AmbientTextureFN.c_str(), len, &dwBytesWritten);
		len = (uint32)DiffuseTextureFN.length();
		pStream->Write(&len, sizeof(len), &dwBytesWritten);
		pStream->Write((void*)DiffuseTextureFN.c_str(), len, &dwBytesWritten);
		len = (uint32)SpecularTextureFN.length();
		pStream->Write(&len, sizeof(len), &dwBytesWritten);
		pStream->Write((void*)SpecularTextureFN.c_str(), len, &dwBytesWritten);
	}
}
