#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; }

void Simplex::MyOctant::Init(void)
{
	//Default values
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_uChildren = 0;

	m_fSize = 0;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f, 0.0f, -25.0f);
	m_v3Min = -vector3(m_fSize / 2);
	m_v3Max = vector3(m_fSize / 2);

	m_pRoot = nullptr;
	m_pParent = nullptr;

	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	//This is the first octant
	m_uOctantCount = 0;
	m_pRoot = this;
	m_lChild.clear();

	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	//Giving a unique ID
	m_uID = m_uOctantCount;

	std::vector<vector3> lMinMax;

	MyRigidBody* pRigidBody;

	uint numObjects = m_pEntityMngr->GetEntityCount();

	//Getting min and max based on rigid body "cloud"'s min and max
	for (uint i = 0; i < numObjects; i++)
	{
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		pRigidBody = pEntity->GetRigidBody();
		lMinMax.push_back(pRigidBody->GetMinGlobal());
		lMinMax.push_back(pRigidBody->GetMaxGlobal());
	}

	pRigidBody = new MyRigidBody(lMinMax);

	vector3 vHalfWidth = pRigidBody->GetHalfWidth();
	float fMax = vHalfWidth.x;
	for (int i = 1; i < 3; i++)
	{
		if (fMax < vHalfWidth[i])
		{
			fMax = vHalfWidth[i];
		}
	}

	vector3 vCenter = pRigidBody->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pRigidBody);

	m_fSize = fMax * 2.0f;
	m_v3Center = m_v3Center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	m_uOctantCount++;

	ConstructTree(m_uMaxLevel);
}

Simplex::MyOctant::MyOctant(vector3 center, float size)
{
	Init();

	m_v3Center = center;
	m_fSize = size;

	m_v3Min = m_v3Center - ((vector3(m_fSize)) / 2.0f);
	m_v3Max = m_v3Center + ((vector3(m_fSize)) / 2.0f);

	m_uOctantCount++;
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	//Setting our stuff to their stuff
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;
	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;
	
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

Simplex::MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	// TODO: insert return statement here

	//If we're not them
	if (this != &other)
	{
		//Make us them
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}

	//otherwise just return ourselves
	return *this;
}

Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	//Making our stuff their stuff
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);

	std::swap(m_lChild, other.m_lChild);
	std::swap(m_uChildren, other.m_uChildren);

	for (int i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	//Getting the entity with the index
	uint numObjects = m_pEntityMngr->GetEntityCount();

	if (a_uRBIndex >= numObjects)
	{
		return false;
	}

	//Getting the entity
	MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);

	//Getting the entities rigidbody
	MyRigidBody* rb = entity->GetRigidBody();

	//Getting the mins and maxes of the rigidbody
	vector3 rb_min = rb->GetMinGlobal();
	vector3 rb_max = rb->GetMaxGlobal();

	//Testing collisions
	if (m_v3Max.x < rb_min.x)
	{
		return false;
	}
	else if (m_v3Min.x > rb_max.x)
	{
		return false;
	}
	else if (m_v3Max.y < rb_min.y)
	{
		return false;
	}
	else if (m_v3Min.y > rb_max.y)
	{
		return false;
	}
	else if (m_v3Max.z < rb_min.z)
	{
		return false;
	}
	else if (m_v3Min.z > rb_max.z)
	{
		return false;
	}

	return true;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//Only displaying if the index is this octant
	if (m_uID == a_nIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
	}
	else
	{
		for (uint index = 0; index < m_uChildren; index++)
		{
			m_pChild[index]->Display(a_nIndex);
		}
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//Diplaying all octants
	for (uint index = 0; index < m_uChildren; index++)
	{
		m_pChild[index]->Display(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//Displaying the end nodes
	for (uint child = 0; child < m_lChild.size(); child++)
	{
		m_lChild[child]->DisplayLeafs(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList(void)
{
	for (uint child = 0; child < m_uChildren; child++)
	{
		m_pChild[child]->ClearEntityList();
	}

	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	if (m_uLevel >= m_uMaxLevel || !IsLeaf())
	{
		return;
	}

	m_uChildren = 8;

	//Getting the child nodes setup
	float newSize = m_fSize / 2.0f;
	float offset = newSize / 2.0f;
	m_pChild[0] = new MyOctant(m_v3Center + vector3(offset, offset, offset), newSize);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(-offset, offset, offset), newSize);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(-offset, offset, -offset), newSize);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(offset, offset, -offset), newSize);
	m_pChild[4] = new MyOctant(m_v3Center + vector3(offset, -offset, offset), newSize);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(-offset, -offset, offset), newSize);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(-offset, -offset, -offset), newSize);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(offset, -offset, -offset), newSize);

	for (uint index = 0; index < 8; index++)
	{
		m_pChild[index]->m_pRoot = m_pRoot;
		m_pChild[index]->m_pParent = this;
		m_pChild[index]->m_uLevel = m_uLevel + 1;
		if (m_pChild[index]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[index]->Subdivide();
		}
	}
}

MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild > 7)
	{
		return nullptr;
	}

	return m_pChild[a_nChild];
}

MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren == 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	uint nCount = 0;
	uint numObjects = m_pEntityMngr->GetEntityCount();
	for (uint n = 0; n < numObjects; n++)
	{
		if (IsColliding(n))
		{
			nCount++;
		}
		if (nCount > a_nEntities)
		{
			return true;
		}
	}

	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	//Go through each child and tell them to kill their children then kill your child. (This is messed up.)
	for (uint index = 0; index < m_uChildren; index++)
	{
		m_pChild[index]->KillBranches();
		delete m_pChild[index];
		m_pChild[index] = nullptr;
	}

	m_uChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//Only useable on the root node
	if (m_uLevel != 0)
	{
		return;
	}

	m_uMaxLevel = a_nMaxLevel;

	m_uOctantCount = 1;

	m_EntityList.clear();

	KillBranches();
	m_lChild.clear();

	//If we have more entities in the octant than we ideally want
	if (ContainsMoreThan(m_uIdealEntityCount))
	{
		//Make more octants
		Subdivide();
	}

	AssignIDtoEntity();

	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	for (uint child = 0; child < m_uChildren; child++)
	{
		m_pChild[child]->AssignIDtoEntity();
	}

	if (IsLeaf())
	{
		for (uint index = 0; index < m_pEntityMngr->GetEntityCount(); index++)
		{
			if (IsColliding(index))
			{
				m_EntityList.push_back(index);
				m_pEntityMngr->AddDimension(index, m_uID);
			}
		}
	}
}

void Simplex::MyOctant::Release(void)
{
	//Get rid of the children
	if (m_uLevel == 0)
	{
		KillBranches();
	}

	m_uChildren = 0;

	//Reset size
	m_fSize = 0.0f;

	//Clear the rest
	m_EntityList.clear();
	m_lChild.clear();
}

void Simplex::MyOctant::ConstructList(void)
{
	for (uint child = 0; child < m_uChildren; child++)
	{
		m_pChild[child]->ConstructList();
	}

	if (m_EntityList.size() < 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
