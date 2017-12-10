#include "MyOctant.h"
using namespace Simplex;

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//Setting the properties to input data
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	
	//Making this the root
	m_uChildren = 0;
	m_uLevel = 0;
	m_pRoot = this;

	//Setting size and other variables related to size
	m_fSize = 50.0f;
	m_v3Center = vector3(0.0f, 0.0f, -25.0f);
	m_v3Min = -vector3(m_fSize / 2);
	m_v3Max = vector3(m_fSize / 2);

	//Making the ID equal to the number of octants and incrementing the number of octants
	m_uID = m_uOctantCount;
	m_uOctantCount++;

	//Adding to the list of nodes
	m_pRoot->m_lChild.push_back(this);

	//Dividing 
	Subdivide();
}

Simplex::MyOctant::MyOctant(vector3 center, float size, MyOctant* parentNode)
{
	//Setting the parent of the node
	m_pParent = parentNode;

	//Setting ID to current count then incrementing
	m_uID = m_uOctantCount;
	m_uOctantCount++;

	//Putting our level just below that of the parent
	m_uLevel = m_pParent->m_uLevel + 1;

	//Setting the size and variables related to size
	m_fSize = size;
	m_v3Center = center;
	m_v3Min = m_v3Center - vector3(m_fSize / 2);
	m_v3Max = m_v3Center + vector3(m_fSize / 2);

	//Setting the root to be the root of the parent
	m_pRoot = m_pParent->m_pRoot;

	//Adding to the list of nodes
	m_pRoot->m_lChild.push_back(this);
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uOctantCount++;
}

Simplex::MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	// TODO: insert return statement here
	return *this;
}

Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

void Simplex::MyOctant::Swap(MyOctant & other)
{

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
	MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);

	//Getting the entities rigidbody
	MyRigidBody* rb = entity->GetRigidBody();

	//Getting the mins and maxes of the rigidbody
	vector3 rb_min = rb->GetMinGlobal();
	vector3 rb_max = rb->GetMaxGlobal();

	bool isColliding = true;

	//Testing collisions
	if (this->m_v3Max.x < rb_min.x)
	{
		isColliding = false;
	}
	else if (this->m_v3Min.x > rb_max.x)
	{
		isColliding = false;
	}
	else if (this->m_v3Max.y < rb_min.y)
	{
		isColliding = false;
	}
	else if (this->m_v3Min.y > rb_max.y)
	{
		isColliding = false;
	}
	else if (this->m_v3Max.z < rb_min.z)
	{
		isColliding = false;
	}
	else if (this->m_v3Min.z > rb_max.z)
	{
		isColliding = false;
	}

	return isColliding;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//If there is nothing to display
	if (m_lChild.size() == 0)
	{
		return;
	}

	//Making sure we are within bounds
	if (a_nIndex >= m_lChild.size())
	{
		a_nIndex = m_lChild.size() - 1;
	}

	m_lChild[a_nIndex]->Display(a_v3Color);
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//Make a model matrix at the center
	matrix4 modelMatrix = glm::translate(m_v3Center);

	//scale the model to the proper size
	modelMatrix = glm::scale(m_fSize, m_fSize, m_fSize);

	m_pMeshMngr->AddWireCubeToRenderList(modelMatrix, a_v3Color);

	DisplayLeafs(a_v3Color);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//If we're a lead
	if (IsLeaf()) {
		//Go through our children and display them
		for (int i = 0; i < 8; i++)
		{
			m_pChild[i]->Display(a_v3Color);
			m_pChild[i]->DisplayLeafs();
		}
	}
}

void Simplex::MyOctant::ClearEntityList(void)
{
	//If we're a leaf
	if (IsLeaf())
	{
		//Clear the entity list
		m_EntityList.clear();
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			m_pChild[i]->ClearEntityList();
		}
	}
}

void Simplex::MyOctant::Subdivide(void)
{
	//If we have more levels than we want or we don't have enough entities 
	if (m_uLevel >= m_uMaxLevel || m_EntityList.size() <= m_uIdealEntityCount)
	{
		//If we have entities
		if (m_EntityList.size() > 0)
		{
			m_pRoot->m_lChild.push_back(this);
		}

		return;
	}

	//Make the new subdivision half the size
	float newSize = m_fSize / 2.0;
	float offset = newSize / 2.0;

	m_pChild[0] = new MyOctant(m_v3Center + vector3(offset, offset, offset), newSize, this);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(-offset, offset, offset), newSize, this);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(-offset, offset, -offset), newSize, this);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(offset, offset, -offset), newSize, this);
	m_pChild[4] = new MyOctant(m_v3Center + vector3(offset, -offset, offset), newSize, this);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(-offset, -offset, offset), newSize, this);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(-offset, -offset, -offset), newSize, this);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(offset, -offset, -offset), newSize, this);

	m_uChildren = 8;

	//Going through the entities and deciding which child they belong in
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < m_EntityList.size(); j++) {
			//If the child is colliding with the entity
			if (m_pChild[i]->IsColliding(m_EntityList[j])) {
				//Put the entity in the child's container
				m_pChild[i]->m_EntityList.push_back(m_EntityList[j]);
			}
		}
	}

	//Possibly subdividing all the children
	for (int i = 0; i < 8; i++) {
		m_pChild[i]->Subdivide();
	}

	//Clearing the entity list now that they are contained in the new children
	m_EntityList.clear();
}

MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
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
	return m_EntityList.size() > a_nEntities;
}

void Simplex::MyOctant::KillBranches(void)
{

}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
	//If we're the root node
	if (m_pParent == nullptr)
	{
		//Go through our children
		for (int i = 1; i < m_lChild.size(); i++)
		{
			//Delete all existing children
			if (m_lChild[i] != nullptr)
			{
				delete m_lChild[i];
			}
		}
	}
}

void Simplex::MyOctant::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_uOctantCount = 0;
	m_uMaxLevel = 0;
	m_uIdealEntityCount = 0;
}

void Simplex::MyOctant::ConstructList(void)
{
	//If we're a leaf...
	if (IsLeaf())
	{
		//Add us to the roots child list
		m_pRoot->m_lChild.push_back(this);
	}
	//Otherwise
	else
	{
		//Check the children
		for (int i = 0; i < 8; i++)
		{
			m_pChild[i]->ConstructTree();
		}
	}
}
