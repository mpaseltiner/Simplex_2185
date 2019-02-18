#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float angle = (2.0f * (float)PI) / a_nSubdivisions;
	float currentAngle = 0;
	float nextAngle = currentAngle + angle;
	vector3 baseCenter = vector3(0, 0, 0);
	vector3 coneTop = vector3(0, 0, a_fHeight);

	for (int x = 0; x < a_nSubdivisions; x++)
	{
		if (x == a_nSubdivisions - 1)
		{
			nextAngle = 0;
		}
		float x1 = glm::cos(currentAngle);
		float x2 = glm::cos(nextAngle);
		float y1 = glm::sin(currentAngle);
		float y2 = glm::sin(nextAngle);

		vector3 firstPoint = (vector3(x1, y1, 0) * a_fRadius);
		vector3 secondPoint = (vector3(x2, y2, 0) * a_fRadius);

		AddTri(secondPoint, firstPoint, baseCenter);
		AddTri(coneTop, firstPoint, secondPoint);

		currentAngle += angle;
		nextAngle += angle;
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float angle = (2.0f * (float)PI) / a_nSubdivisions;
	float currentAngle = 0;
	float nextAngle = currentAngle + angle;
	vector3 bottomCenter = vector3(0, -(a_fHeight/2), 0);
	vector3 topCenter = vector3(0, a_fHeight/2, 0);

	for (int x = 0; x < a_nSubdivisions; x++)
	{
		if (x == a_nSubdivisions - 1)
		{
			nextAngle = 0;
		}
		float x1 = glm::cos(currentAngle);
		float x2 = glm::cos(nextAngle);
		float y1 = glm::sin(currentAngle);
		float y2 = glm::sin(nextAngle);

		vector3 bottomFirstPoint = (vector3(x1, -(a_fHeight/2), y1) * a_fRadius);
		vector3 bottomSecondPoint = (vector3(x2, -(a_fHeight/2), y2) * a_fRadius);
		vector3 topFirstPoint = (vector3(x1, a_fHeight/2, y1) * a_fRadius);
		vector3 topSecondPoint = (vector3(x2, a_fHeight/2, y2) * a_fRadius);

		//bottom tri
		AddTri(bottomCenter, bottomFirstPoint, bottomSecondPoint);
		//top tri
		AddTri(topSecondPoint, topFirstPoint, topCenter);

		//add sides
		AddTri(topFirstPoint, bottomSecondPoint, bottomFirstPoint);
		AddTri(topFirstPoint, topSecondPoint, bottomSecondPoint);
		//AddQuad(topFirstPoint, topSecondPoint, bottomFirstPoint, bottomSecondPoint);


		currentAngle += angle;
		nextAngle += angle;
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float angle = (2.0f * (float)PI) / a_nSubdivisions;
	float currentAngle = 0;
	float nextAngle = currentAngle + angle;
	vector3 bottomCenter = vector3(0, -(a_fHeight / 2), 0);
	vector3 topCenter = vector3(0, a_fHeight / 2, 0);

	for (int x = 0; x < a_nSubdivisions; x++)
	{
		if (x == a_nSubdivisions - 1)
		{
			nextAngle = 0;
		}
		float x1 = glm::cos(currentAngle);
		float x2 = glm::cos(nextAngle);
		float y1 = glm::sin(currentAngle);
		float y2 = glm::sin(nextAngle);

		//outer points
		vector3 bottomFirstPoint = (vector3(x1, -(a_fHeight / 2), y1) * a_fOuterRadius);
		vector3 bottomSecondPoint = (vector3(x2, -(a_fHeight / 2), y2) * a_fOuterRadius);
		vector3 topFirstPoint = (vector3(x1, a_fHeight / 2, y1) * a_fOuterRadius);
		vector3 topSecondPoint = (vector3(x2, a_fHeight / 2, y2) * a_fOuterRadius);

		//innner points
		vector3 bottomThirdPoint = (vector3(x1, -(a_fHeight / 2), y1) * a_fInnerRadius);
		vector3 bottomFourthPoint = (vector3(x2, -(a_fHeight / 2), y2) * a_fInnerRadius);
		vector3 topThirdPoint = (vector3(x1, a_fHeight / 2, y1) * a_fInnerRadius);
		vector3 topFourthPoint = (vector3(x2, a_fHeight / 2, y2) * a_fInnerRadius);

		//bottom quad
		AddTri(bottomFirstPoint, bottomSecondPoint, bottomThirdPoint);
		AddTri(bottomSecondPoint, bottomFourthPoint, bottomThirdPoint);

		//top quad
		AddTri(topThirdPoint, topSecondPoint, topFirstPoint);
		AddTri(topThirdPoint, topFourthPoint, topSecondPoint);

		//outer side
		AddTri(topFirstPoint, bottomSecondPoint, bottomFirstPoint);
		AddTri(topFirstPoint, topSecondPoint, bottomSecondPoint);

		//inner side
		AddTri(bottomThirdPoint, bottomFourthPoint, topThirdPoint);
		AddTri(bottomFourthPoint, topFourthPoint, topThirdPoint);

		currentAngle += angle;
		nextAngle += angle;
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	float x, y, z; //vertex coordinates

	float theta = 2 * (float)PI / a_nSubdivisions;
	float phi = (float)PI / a_nSubdivisions;
	float currentTheta = 0;
	float currentPhi = phi;
	vector3 currentLayerPoints[6];
	vector3 previousLayerPoints[6];
	vector3 top = vector3(0, 0, a_fRadius);
	vector3 bottom = vector3(0, 0, -a_fRadius);
	vector3 currentPoint;

	for (int r = 0; r < a_nSubdivisions; r++)
	{
		//find the points for the current layer
		for (int c = 0; c < a_nSubdivisions; c++)
		{
			if (r != 0)
			{
				previousLayerPoints[c] = currentLayerPoints[c];
			}
			if (r <= a_nSubdivisions - 1);
			{
				x = glm::sin(currentPhi) * glm::cos(currentTheta);   //r * sin(u) * cos(v)
				y = glm::sin(currentPhi) * glm::sin(currentTheta);   //r * sin(u) * sin(v)
				z = glm::cos(currentPhi);  //r * cos(u)

				currentPoint = vector3(x, y, z) * a_fRadius;
				currentLayerPoints[c] = currentPoint;
			}
			currentTheta += theta;
		}
		currentPhi += phi;

		//add the faces for the current layer
		for (int c = 0; c < a_nSubdivisions; c++)
		{
			//top of the sphere
			if (r == 0)
			{
				if (c == a_nSubdivisions - 1)
				{
					AddTri(currentLayerPoints[c], currentLayerPoints[0], top);
				}
				else
				{
					AddTri(currentLayerPoints[c], currentLayerPoints[c+1], top);

				}
			}
			//between top and bottom layer
			else if (r > 0 && r < a_nSubdivisions - 1)
			{
				if (c == a_nSubdivisions - 1)
				{
					AddTri(currentLayerPoints[c], currentLayerPoints[0], previousLayerPoints[c]);
					AddTri(currentLayerPoints[0], previousLayerPoints[0], previousLayerPoints[c]);
				}
				else
				{
					AddTri(currentLayerPoints[c], currentLayerPoints[c+1], previousLayerPoints[c]);
					AddTri(currentLayerPoints[c+1], previousLayerPoints[c+1], previousLayerPoints[c]);
				}
			}
			//bottom of the sphere
			else
			{
				if (c == a_nSubdivisions - 1)
				{
					AddTri(previousLayerPoints[c], bottom, previousLayerPoints[0]);
				}
				else
				{
					AddTri(previousLayerPoints[c], bottom, previousLayerPoints[c + 1]);

				}
			}
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}