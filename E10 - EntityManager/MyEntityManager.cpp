#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (int x = 0; x < m_uEntityCount; x++)
	{
		if (m_entityList[x]->GetUniqueID == a_sUniqueID)
		{
			return x;
		}
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetModel;
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	for (int x = 0; x < m_uEntityCount; x++)
	{
		if (m_entityList[x]->GetUniqueID == a_sUniqueID)
		{
			return m_entityList[x]->GetModel;
		}
	}
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetRigidBody;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	for (int x = 0; x < m_uEntityCount; x++)
	{
		if (m_entityList[x]->GetUniqueID == a_sUniqueID)
		{
			return m_entityList[x]->GetRigidBody;
		}
	}
	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetModelMatrix;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	for (int x = 0; x < m_uEntityCount; x++)
	{
		if (m_entityList[x]->GetUniqueID == a_sUniqueID)
		{
			return m_entityList[x]->GetModelMatrix;
		}
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	for (int x = 0; x < m_uEntityCount; x++)
	{
		if (m_entityList[x]->GetUniqueID == a_sUniqueID)
		{
			m_entityList[x]->SetModelMatrix(a_m4ToWorld);
		}
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{

}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	m_entityList.push_back(new MyEntity(a_sFileName, a_sUniqueID));
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{

}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{

}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetUniqueID;
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	return  m_entityList[a_uIndex]->GetEntity;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_bRigidBody)
	{
		
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{

}