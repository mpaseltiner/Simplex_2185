#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;

//the big 3
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	std::vector<vector3> lMinMax; //holds the min and max vectors of the bounding object

	int nObjects = m_pEntityMngr->GetEntityCount();

	for (int x = 0; x < nObjects; x++)
	{
		MyEntity* newEntity = m_pEntityMngr->GetEntity(x);
		MyRigidBody* newRigidBody = newEntity->GetRigidBody();
		lMinMax.push_back(newRigidBody->GetMinGlobal());
		lMinMax.push_back(newRigidBody->GetMaxGlobal());
	}
	MyRigidBody* lMinMaxRigidBody = new MyRigidBody(lMinMax);

	vector3 halfWidth = lMinMaxRigidBody->GetHalfWidth();
	float max = halfWidth.x;
	for (int x = 0; x < 3; x++)
	{
		if (max < halfWidth[x])
		{
			max = halfWidth[x];
		}
	}
	
	vector3 center = lMinMaxRigidBody->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(lMinMaxRigidBody);

	m_fSize = max * 2.0f;
	m_v3Center = center;
	m_v3Max = m_v3Center + (vector3(max));
	m_v3Min = m_v3Center - (vector3(max));

	m_uOctantCount++;

	ConstructTree(m_uMaxLevel); 
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);

	m_uOctantCount++; 

}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Max = other.m_v3Max;
	m_v3Min = other.m_v3Min;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (int x = 0; x < 8; x++)
	{
		m_pChild[x] = other.m_pChild[x];
	}
}

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}

	return *this;
}

Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_v3Min, other.m_v3Min);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);

	for (int x = 0; x < 8; x++)
	{
		std::swap(m_pChild[x], other.m_pChild[x]);
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
	int objectCount = m_pEntityMngr->GetEntityCount();
	//if the index given is larger than the number of elements in the bounding object there is no collision
	if (a_uRBIndex >= objectCount)
	{
		return false;
	}

	//1. get all vectors in global space
	MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* rigidBody = entity->GetRigidBody();
	vector3 entityMin = rigidBody->GetMinGlobal();
	vector3 entityMax = rigidBody->GetMaxGlobal();

	//2. check x
	if (m_v3Max.x < entityMin.x)
	{
		return false;
	}
	if (m_v3Min.x > entityMax.x)
	{
		return false;
	}
	//3. check y
	if (m_v3Max.y < entityMin.y)
	{
		return false;
	}
	if (m_v3Min.y > entityMax.y)
	{
		return false;
	}
	//4. check z
	if (m_v3Max.z < entityMin.z)
	{
		return false;
	}
	if (m_v3Min.z > entityMax.z)
	{
		return false;
	}

	return true;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//draws a wireframe cube with the specified color for each element in the octree at the given index

	if (m_uID == a_nIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}

	for (int x = 0; x < m_uChildren; x++)
	{
		m_pChild[x]->Display(a_nIndex);
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//draws a wireframe cube with the specified color 
	for (int x = 0; x < m_uChildren; x++)
	{
		m_pChild[x]->Display(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//draws a wireframe cube around each leaf with the specified color
	int numberOfLeafs = m_lChild.size();

	for (int x = 0; x < numberOfLeafs; x++)
	{
		m_lChild[x]->DisplayLeafs(a_v3Color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList(void)
{
	//recursively deletes the list of entities from each child
	for (int x = 0; x < m_uChildren; x++)
	{
		m_pChild[x]->ClearEntityList();
	}
	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	//if this node has reached the maximum level for the octree, return
	if (m_uLevel >= m_uMaxLevel)
	{
		return;
	}

	//if this node has already been subdivided, return 
	if (m_uChildren != 0)
	{
		return;
	}

	//give this subdivision 8 children
	m_uChildren = 8;

	float size = m_fSize / 4.0f;
	float sizeD = m_fSize / 2.0f;
	vector3 center;

	//bottom left back
	center = m_v3Center;
	center.x -= size;
	center.y -= size;
	center.z -= size;
	m_pChild[0] = new MyOctant(center, sizeD);

	//bottom right back
	center.x += sizeD;
	m_pChild[1] = new MyOctant(center, sizeD);

	//bottom right front
	center.z += sizeD;
	m_pChild[2] = new MyOctant(center, sizeD);

	//bottom left front
	center.x -= sizeD;
	m_pChild[3] = new MyOctant(center, sizeD);

	//top left front
	center.y += sizeD;
	m_pChild[4] = new MyOctant(center, sizeD);

	//top left back
	center.z -= sizeD;
	m_pChild[5] = new MyOctant(center, sizeD);

	//top right back
	center.x += sizeD;
	m_pChild[6] = new MyOctant(center, sizeD);

	//top right front
	center.z += sizeD;
	m_pChild[7] = new MyOctant(center, sizeD);

	//recursively move through each level needed for octree
	for (int x = 0; x < 8; x++)
	{
		m_pChild[x]->m_pRoot = m_pRoot;
		m_pChild[x]->m_pParent = this;
		m_pChild[x]->m_uLevel = m_uLevel + 1;
		if (m_pChild[x]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[x]->Subdivide();
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
	//checks to see if this section of the tree contains has more collisions than entities in it
	int count = 0;
	int objectCount = m_pEntityMngr->GetEntityCount();
	for (int x = 0; x < objectCount; x++)
	{
		if (IsColliding(x))
		{
			count++;
		}
		if (count > a_nEntities)
		{
			return true;
		}
	}
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	//recursively destroys all branches of the tree
	for (int x = 0; x < m_uChildren; x++)
	{
		m_pChild[x]->KillBranches();
		delete m_pChild[x];
		m_pChild[x] = nullptr;
	}
	m_uChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//check if this isn't the root 
	if (m_uLevel != 0)
	{
		return;
	}

	//creates the tree at an initial octant count of 1
	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();

	//subdivide the tree if needed
	if (ContainsMoreThan(m_uIdealEntityCount))
	{
		Subdivide();
	}

	AssignIDtoEntity();
	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//travers the tree until a leaf is reached
	for (int x = 0; x < m_uChildren; x++)
	{
		m_pChild[x]->AssignIDtoEntity();
	}
	//when a leaf is found
	//gets all the entities in the leaf
	//then sets the objects in the leaf to the index
	if (m_uChildren == 0)
	{
		int entityCount = m_pEntityMngr->GetEntityCount();
		for (int x = 0; x < entityCount; x++)
		{
			if (IsColliding(x))
			{
				m_EntityList.push_back(x);
				m_pEntityMngr->AddDimension(x, m_uID);
			}
		}
	}
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
	//clears the tree of all leafs, branches, and children
	//preps the tree for deletion

	//this release only happens for the root of the octree
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	//reset the octree to base state
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}

void Simplex::MyOctant::Init(void)
{
	//initializes all of the variables
	//gets refernce to the mesh and entity managers

	m_uChildren = 0;
	m_fSize = 0.0f;
	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_v3Min = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;

	for (int x = 0; x < 8; x++)
	{
		m_pChild[x] = nullptr;
	}
}

void Simplex::MyOctant::ConstructList(void)
{
	//recursively creates the list of children
	for (int x = 0; x < m_uChildren; x++)
	{
		m_pChild[x]->ConstructList();
	}

	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}

}
