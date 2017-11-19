#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	for (uint uEntity = 0; uEntity < m_uEntityCount; uEntity++)
	{
		MyEntity* pEntity = m_entityList[uEntity];
		SafeDelete(pEntity);
	}
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
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//Find the entity with the ID, if there is one
	for (uint uIndex = 0; uIndex < m_uEntityCount; uIndex++)
	{
		if (a_sUniqueID == m_entityList[uIndex]->GetUniqueID())
		{
			return uIndex;
		}
	}

	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//If we don't have any entities
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	//If we're out of bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//Create a temporary entity with the unique ID
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//If it exists
	if (temp)
	{
		return temp->GetModel();
	}

	//Otherwise
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//If we don't have any entities
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	//If we're out of bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//Create a temporary entity with the unique ID
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//If it exists
	if (temp)
	{
		return temp->GetRigidBody();
	}

	//Otherwise
	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//If we don't have any entities
	if (m_entityList.size() == 0)
	{
		return IDENTITY_M4;
	}

	//If we're out of bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	//Create a temporary entity with the unique ID
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//If it exists
	if (temp)
	{
		return temp->GetModelMatrix();
	}

	//Otherwise
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	//Create a temporary entity with the unique ID
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	//If it exists
	if (temp)
	{
		temp->SetModelMatrix(a_m4ToWorld);
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//If we don't have any entities
	if (m_entityList.size() == 0)
	{
		return;
	}

	//If we're out of bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

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
	//Check collisions
	for (int i = 0; i < m_uEntityCount - 1; i++)
	{
		for (int j = 0; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//Storing information in a temp
	MyEntity* temp = new MyEntity(a_sFileName, a_sUniqueID);

	//If valid
	if (temp->IsInitialized)
	{
		m_entityList.push_back(temp);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//List is empty
	if (m_entityList.size() == 0)
	{
		return;
	}

	//Choosing the last one if we're out of bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	//Swap the entity for the last one
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
	}

	MyEntity* temp = m_entityList[m_uEntityCount - 1];
	SafeDelete(temp);
	m_entityList.pop_back();
	m_uEntityCount--;
	return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)index);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if (m_entityList.size() == 0)
	{
		return "";
	}

	if (a_uIndex >= m_entityList.size())
	{
		a_uIndex = m_entityList.size() - 1;
	}

	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	if (a_uIndex >= m_entityList.size())
	{
		a_uIndex = m_entityList.size() - 1;
	}

	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//If out of bounds
	if (a_uIndex >= m_uEntityCount)
	{
		//Add each member of the entity list
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; a_uIndex++)
		{
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}

	//Add a specific member
	else
	{
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//Storing information in a temp
	MyEntity* temp = new MyEntity(a_sUniqueID);

	//If valid
	if (temp)
	{
		temp->AddToRenderList(a_bRigidBody);
	}
}