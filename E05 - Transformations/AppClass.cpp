#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);
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

	static float fDisplacement = -10.0f;
	fDisplacement += 0.05f;

	//global translation matrix
	matrix4 m4_globalTranslationMatrix = glm::translate(vector3(fDisplacement, 0, 0));

	matrix4 m4_Location1 = m4_globalTranslationMatrix * glm::translate(vector3(-3, 5, 0));
	matrix4 m4_Location2 = m4_globalTranslationMatrix * glm::translate(vector3(3, 5, 0));
	matrix4 m4_Location3 = m4_globalTranslationMatrix * glm::translate(vector3(-2, 4, 0));
	matrix4 m4_Location4 = m4_globalTranslationMatrix * glm::translate(vector3(2, 4, 0));
	matrix4 m4_Location5= m4_globalTranslationMatrix * glm::translate(vector3(-3, 3, 0));
	matrix4 m4_Location6 = m4_globalTranslationMatrix * glm::translate(vector3(-2, 3, 0));
	matrix4 m4_Location7 = m4_globalTranslationMatrix * glm::translate(vector3(-1, 3, 0));
	matrix4 m4_Location8 = m4_globalTranslationMatrix * glm::translate(vector3(0, 3, 0));
	matrix4 m4_Location9 = m4_globalTranslationMatrix * glm::translate(vector3(1, 3, 0));
	matrix4 m4_Location10 = m4_globalTranslationMatrix * glm::translate(vector3(2, 3, 0));
	matrix4 m4_Location11 = m4_globalTranslationMatrix * glm::translate(vector3(3, 3, 0));
	matrix4 m4_Location12 = m4_globalTranslationMatrix * glm::translate(vector3(-4, 2, 0));
	matrix4 m4_Location13 = m4_globalTranslationMatrix * glm::translate(vector3(-3, 2, 0));
	matrix4 m4_Location14 = m4_globalTranslationMatrix * glm::translate(vector3(-1, 2, 0));
	matrix4 m4_Location15 = m4_globalTranslationMatrix * glm::translate(vector3(0, 2, 0));
	matrix4 m4_Location16 = m4_globalTranslationMatrix * glm::translate(vector3(1, 2, 0));
	matrix4 m4_Location17 = m4_globalTranslationMatrix * glm::translate(vector3(3, 2, 0));
	matrix4 m4_Location18 = m4_globalTranslationMatrix * glm::translate(vector3(4, 2, 0));
	matrix4 m4_Location19 = m4_globalTranslationMatrix * glm::translate(vector3(-5, 1, 0));
	matrix4 m4_Location20 = m4_globalTranslationMatrix * glm::translate(vector3(-4, 1, 0));
	matrix4 m4_Location21 = m4_globalTranslationMatrix * glm::translate(vector3(-3, 1, 0));
	matrix4 m4_Location22 = m4_globalTranslationMatrix * glm::translate(vector3(-2, 1, 0));
	matrix4 m4_Location23 = m4_globalTranslationMatrix * glm::translate(vector3(-1, 1, 0));
	matrix4 m4_Location24 = m4_globalTranslationMatrix * glm::translate(vector3(0, 1, 0));
	matrix4 m4_Location25 = m4_globalTranslationMatrix * glm::translate(vector3(1, 1, 0));
	matrix4 m4_Location26 = m4_globalTranslationMatrix * glm::translate(vector3(2, 1, 0));
	matrix4 m4_Location27 = m4_globalTranslationMatrix * glm::translate(vector3(3, 1, 0));
	matrix4 m4_Location28 = m4_globalTranslationMatrix * glm::translate(vector3(4, 1, 0));
	matrix4 m4_Location29 = m4_globalTranslationMatrix * glm::translate(vector3(5, 1, 0));
	matrix4 m4_Location30 = m4_globalTranslationMatrix * glm::translate(vector3(-5, 0, 0));
	matrix4 m4_Location31 = m4_globalTranslationMatrix * glm::translate(vector3(-3, 0, 0));
	matrix4 m4_Location32 = m4_globalTranslationMatrix * glm::translate(vector3(-2, 0, 0));
	matrix4 m4_Location33 = m4_globalTranslationMatrix * glm::translate(vector3(-1, 0, 0));
	matrix4 m4_Location34 = m4_globalTranslationMatrix * glm::translate(vector3(0, 0, 0));
	matrix4 m4_Location35 = m4_globalTranslationMatrix * glm::translate(vector3(1, 0, 0));
	matrix4 m4_Location36 = m4_globalTranslationMatrix * glm::translate(vector3(2, 0, 0));
	matrix4 m4_Location37 = m4_globalTranslationMatrix * glm::translate(vector3(3, 0, 0));
	matrix4 m4_Location38 = m4_globalTranslationMatrix * glm::translate(vector3(5, 0, 0));
	matrix4 m4_Location39 = m4_globalTranslationMatrix * glm::translate(vector3(-5, -1, 0));
	matrix4 m4_Location40 = m4_globalTranslationMatrix * glm::translate(vector3(-3, -1, 0));
	matrix4 m4_Location41 = m4_globalTranslationMatrix * glm::translate(vector3(3, -1, 0));
	matrix4 m4_Location42 = m4_globalTranslationMatrix * glm::translate(vector3(5, -1, 0));
	matrix4 m4_Location43 = m4_globalTranslationMatrix * glm::translate(vector3(-2, -2, 0));
	matrix4 m4_Location44 = m4_globalTranslationMatrix * glm::translate(vector3(-1, -2, 0));
	matrix4 m4_Location45 = m4_globalTranslationMatrix * glm::translate(vector3(1, -2, 0));
	matrix4 m4_Location46 = m4_globalTranslationMatrix * glm::translate(vector3(2, -2, 0));


	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location1);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location2);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location3);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location4);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location5);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location6);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location7);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location8);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location9);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location10);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location11);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location12);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location13);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location14);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location15);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location16);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location17);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location18);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location19);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location20);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location21);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location22);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location23);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location24);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location25);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location26);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location27);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location28);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location29);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location30);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location31);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location32);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location33);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location34);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location35);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location36);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location37);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location38);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location39);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location40);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location41);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location42);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location43);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location44);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location45);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4_Location46);

		
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
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}