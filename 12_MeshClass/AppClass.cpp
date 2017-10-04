#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh[48];

	for (uint i = 0; i < 46; ++i)
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

	m_pMesh[0].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3.0f, 4.0f, 0)));
	m_pMesh[1].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3.0f,4.0f,0)));

	m_pMesh[2].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2.0f, 3.0f, 0)));
	m_pMesh[3].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2.0f, 3.0f, 0)));

	m_pMesh[4].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3.0f, 2.0f, 0)));
	m_pMesh[5].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2.0f, 2.0f, 0)));
	m_pMesh[6].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1.0f, 2.0f, 0)));
	m_pMesh[7].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0.0f, 2.0f, 0)));
	m_pMesh[8].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1.0f, 2.0f, 0)));
	m_pMesh[9].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2.0f, 2.0f, 0)));
	m_pMesh[10].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3.0f, 2.0f, 0)));

	m_pMesh[11].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-4.0f, 1.0f, 0)));
	m_pMesh[12].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3.0f, 1.0f, 0)));
	m_pMesh[13].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1.0f, 1.0f, 0)));
	m_pMesh[14].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0.0f, 1.0f, 0)));
	m_pMesh[15].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1.0f, 1.0f, 0)));
	m_pMesh[16].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3.0f, 1.0f, 0)));
	m_pMesh[17].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(4.0f, 1.0f, 0)));

	m_pMesh[18].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-5.0f, 0.0f, 0)));
	m_pMesh[19].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-4.0f, 0.0f, 0)));
	m_pMesh[20].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3.0f, 0.0f, 0)));
	m_pMesh[21].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2.0f, 0.0f, 0)));
	m_pMesh[22].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1.0f, 0.0f, 0)));
	m_pMesh[23].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0)));
	m_pMesh[24].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1.0f, 0.0f, 0)));
	m_pMesh[25].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2.0f, 0.0f, 0)));
	m_pMesh[26].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3.0f, 0.0f, 0)));
	m_pMesh[27].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(4.0f, 0.0f, 0)));
	m_pMesh[28].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(5.0f, 0.0f, 0)));

	m_pMesh[29].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-5.0f, -1.0f, 0)));
	m_pMesh[30].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3.0f, -1.0f, 0)));
	m_pMesh[31].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2.0f, -1.0f, 0)));
	m_pMesh[32].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1.0f, -1.0f, 0)));
	m_pMesh[33].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0.0f, -1.0f, 0)));
	m_pMesh[34].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1.0f, -1.0f, 0)));
	m_pMesh[35].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2.0f, -1.0f, 0)));
	m_pMesh[36].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3.0f, -1.0f, 0)));
	m_pMesh[37].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(5.0f, -1.0f, 0)));

	m_pMesh[38].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-5.0f, -2.0f, 0)));
	m_pMesh[39].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3.0f, -2.0f, 0)));
	m_pMesh[40].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3.0f, -2.0f, 0)));
	m_pMesh[41].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(5.0f, -2.0f, 0)));

	m_pMesh[42].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2.0f, -3.0f, 0)));
	m_pMesh[43].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1.0f, -3.0f, 0)));
	m_pMesh[44].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1.0f, -3.0f, 0)));
	m_pMesh[45].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2.0f, -3.0f, 0)));
	
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