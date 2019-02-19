#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(vector3(5.0f,3.0f,15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");
	
	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;
	




	//your code goes here
	v3CurrentPos = vector3(0.0f, 0.0f, 0.0f);
	//-------------------
	float xDistance;
	float yDistance;
	float zDistance;

	vector3 startPos = vector3(0.0f, 0.0f, 0.0f);
	vector3 endPos = vector3(0.0f, 0.0f, 0.0f);
	if (nextPoint)
	{
		//LERP calculation
		startPos = m_stopsList[currentIndex];
		endPos = m_stopsList[0];
		v3CurrentPos = (startPos + fTimer * (endPos - startPos));

		//curremt x, y, z location of object
		xDistance = m_stopsList[0].x - v3CurrentPos.x;
		yDistance = m_stopsList[0].y - v3CurrentPos.y;
		zDistance = m_stopsList[0].z - v3CurrentPos.z;
	}
	else
	{
		//LERP calculation
		startPos = m_stopsList[currentIndex];
		endPos = m_stopsList[currentIndex + 1];
		v3CurrentPos = (startPos + fTimer * (endPos - startPos));

		//curremt x, y, z location of object
		xDistance = m_stopsList[currentIndex + 1].x - v3CurrentPos.x;
		yDistance = m_stopsList[currentIndex + 1].y - v3CurrentPos.y;
		zDistance = m_stopsList[currentIndex + 1].z - v3CurrentPos.z;
	}
	
	//calculate distance from the point the object is LERPing to
	float distance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));

	if (distance < .1)
	{
		if (nextPoint)
		{
			nextPoint = false;
		}
		else 
		{
			if (currentIndex == m_stopsList.size() - 2)
			{
				currentIndex = 0;
				nextPoint = true;
			}
			else if (!nextPoint)
			{
				currentIndex++;
			}
		}
		//restart timer for LERPing to next point
		fTimer -= 1;
	}

	
	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
	}
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}