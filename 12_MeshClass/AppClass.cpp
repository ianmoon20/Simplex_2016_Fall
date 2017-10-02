#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh[48];

	for (uint i = 0; i < 48; ++i)
	{
		m_pMesh[i].GenerateCube(1.0f, C_BROWN);
	}
	
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
		
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	vector3 v3Position(3, -1, 0);
	static matrix4 m4Model;

	m4Model *= glm::translate(IDENTITY_M4, vector3(0.1f, 0.0f, 0.0f));

	m_pMesh[0].Render(m4Projection, m4View, m4Model);
	m_pMesh[1].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1,0,0)));
	m_pMesh[2].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2, 0, 0)));
	
	for (int i = 0; i < 11; i++)
	{
		int numUsed = 0;
		switch (i)
		{
		case 0:
			m_pMesh[numUsed].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 4, 0)));
			m_pMesh[numUsed].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 4, 0)));
		}
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
	if (m_pMesh != nullptr)
	{
		delete[] m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}