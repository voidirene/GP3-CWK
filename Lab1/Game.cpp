#include "Game.h"

//unsigned int indices[] = { 0, 1, 2 };

Game::Game() : cubemapTexture(0), skyboxVAO(0), skyboxVBO(0)
{
	gameState = GameState::PLAYING; //set the game state to PLAYING
	gameDisplay = new ScreenDisplay(); //create a new display

	//construct shaders
	//Shading shader();
	//Shading fogshader();
	//Shading toonshader();
	//Shading rimshader();
	//Shading toonrimshader();
	//Shading geoshader();
	//Shading reflectionshader();
	//Shading adsshader();
	//Shading shaderSkybox();

	//construct other gameobjects
	//GameObject monkey();
	//Audio* audioDevice();

	counter = 0;
}

Game::~Game()
{

}

void Game::StartGame()
{
	InitializeSystems();
	GameLoop();
}

void Game::Exit(std::string text)
{
	gameDisplay->~ScreenDisplay();
	std::cout << text << '\n';
	SDL_Quit();
}

void Game::InitializeSystems()
{
	srand(time(0)); //NOTE: seeding rand so that it works properly

	gameDisplay->InitializeDisplay(); //initializes the game display

	monkey.LoadModel("..\\res\\monkey3.obj"); //loads a mesh from file
	teapot.LoadModel("..\\res\\teapot.obj");
	capsule.LoadModel("..\\res\\capsule.obj");
	for (int i = 0; i < sizeof(asteroids) / sizeof(GameObject); i++) //TODO: make loop count backwards for increased performance
	{
		asteroids[i].LoadModel("..\\res\\asteroid.obj");

		//initialize position of each asteroid
		float randomX = ((float)rand() / (RAND_MAX)) * 10;
		float randomY = ((float)rand() / (RAND_MAX)) * 10;
		float randomZ = ((float)rand() / (RAND_MAX)) * 10;
		asteroids[i].SetTransformParameters(glm::vec3(randomX, randomY, randomZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.001, 0.001, 0.001));
	}

	shader.InitializeShader("..\\res\\shader"); //create a new shader
	fogshader.InitializeShader("..\\res\\FogShader");
	toonshader.InitializeShader("..\\res\\ToonShader");
	rimshader.InitializeShader("..\\res\\RimLightingShader");
	toonrimshader.InitializeShader("..\\res\\ToonRimShader");
	geoshader.InitializeGeoShader("..\\res\\shaderGeoText");
	reflectionshader.InitializeShader("..\\res\\shaderReflection");
	adsshader.InitializeShader("..\\res\\ADSshader");
	fboshader.InitializeShader("..\\res\\FBOShader");
	fbograyscaleshader.InitializeShader("..\\res\\FBOGrayscaleShader");
	fboinversionshader.InitializeShader("..\\res\\FBOInversionShader");

	textures.InitializeTexture("..\\res\\bricks.jpg"); //load a texture
	textures.InitializeTexture("..\\res\\water.jpg");
	textures.InitializeTexture("..\\res\\grass.jpg");
	textures.InitializeTexture("..\\res\\rock.jpg");

	camera.InitializeCamera(glm::vec3(0, 0, -5), 70.0f, (float) gameDisplay->GetWidth() / gameDisplay->GetHeight(), 0.01f, 1000.0f); //initializes the camera
	fbo.GenerateFBO(gameDisplay->GetWidth(), gameDisplay->GetHeight());
	fbo.GenerateQuad();

	audio.AddNewSound("..\\res\\bang.wav");
	audio.AddNewBackgroundMusic("..\\res\\background.wav");

	shaderSkybox.InitializeShader("..\\res\\SkyboxShader");
	InitializeSkybox();

	counter = 0.0f;
}

void Game::InitializeSkybox()
{
	std::vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	cubemapTexture = skybox.LoadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Game::DisplaySkybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.UseShader();
	shaderSkybox.setInt("skybox", 0);
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetView())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", view);
	shaderSkybox.setMat4("projection", camera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

//TODO: setup multiple mesh support (RimLight, RimShade, Reflect, ADS)
//TODO: combine these methods?
void Game::LinkFogShaderData()
{
	fogshader.setVec3("fogColor", glm::vec3(0.2, 0.2, 0.2));
	fogshader.setFloat("minDist", -5.0f);
	fogshader.setFloat("maxDist", 5.0f);
}

void Game::LinkToonShaderData()
{
	toonshader.setVec3("lightDir", glm::vec3(1, 5, 1));
}

void Game::LinkRimLightingShaderData()
{
	rimshader.setVec3("lightDir", glm::vec3(0, 0, 3));
	rimshader.setMat4("m", capsule.GetModelMatrix());
}

void Game::LinkToonRimShaderData()
{
	toonrimshader.setVec3("lightDir", glm::vec3(0, 0, 3));
	toonrimshader.setMat4("m", capsule.GetModelMatrix());
}

void Game::LinkGeoShaderData()
{
	float randomX = ((float)rand() / (RAND_MAX));
	float randomY = ((float)rand() / (RAND_MAX));
	float randomZ = ((float)rand() / (RAND_MAX));
	geoshader.setFloat("randColourX", randomX);
	geoshader.setFloat("randColourY", randomY);
	geoshader.setFloat("randColourZ", randomZ);
	geoshader.setFloat("time", counter);
}

void Game::LinkReflectionShaderData()
{
	reflectionshader.setMat4("view", camera.GetView());
	reflectionshader.setMat4("projection", camera.GetProjection());
	reflectionshader.setVec3("cameraPos", camera.GetPosition());
	reflectionshader.setMat4("model", monkey.GetModelMatrix());
}

void Game::LinkADSShaderData()
{
	adsshader.setMat4("view", camera.GetView());
	adsshader.setMat4("projection", camera.GetProjection());
	adsshader.setMat4("model", capsule.GetModelMatrix());
	adsshader.setVec3("lightPos", 0.5f, 2.0f, -3.0f);
	adsshader.setVec3("viewPos", camera.GetPosition());
	adsshader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
}

void Game::UpdateDelta()
{
	deltaLast = deltaNow;
	deltaNow = SDL_GetPerformanceCounter();

	deltaTime = (float)((deltaNow - deltaLast) / (float)SDL_GetPerformanceFrequency());
	deltaTimeSeconds = deltaTime * 0.001; //converts and stores deltaTime in seconds format
	//TODO: update model movement using deltatime
}

void Game::GameLoop()
{
	while (gameState == GameState::PLAYING)
	{
		audio.PlayBackgroundMusic();
		ProcessUserInputs();
		UpdateDisplay();

		DetectCollision(monkey.boundingSphere.GetPosition(), monkey.boundingSphere.GetRadius(), teapot.boundingSphere.GetPosition(), teapot.boundingSphere.GetRadius());
		UpdateDelta();
	}

	Exit("Escape key pressed, closing program...");
}

void Game::ProcessUserInputs()
{
	glfwPollEvents(); //poll events

	//for quitting the game
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		gameState = GameState::QUITTING;
		return;
	}

	//for keyboard camera movement
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.MoveCameraVertically(cameraSpeed * deltaTime);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.MoveCameraVertically(-cameraSpeed * deltaTime);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.MoveCameraHorizontally(cameraSpeed * deltaTime);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.MoveCameraHorizontally(-cameraSpeed * deltaTime);
	}
	//for rotating camera around a mesh
	//TODO: setup multiple mesh support
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.RotateCameraAroundMesh(*capsule.GetTransform().GetPosition(), cameraSpeed * deltaTime);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.RotateCameraAroundMesh(*capsule.GetTransform().GetPosition(), -cameraSpeed * deltaTime);
	}
	//for zooming camera in and out
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		camera.ZoomCamera(cameraSpeed * deltaTime);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		camera.ZoomCamera(-cameraSpeed * deltaTime);
	}
	//for centering camera on a mesh
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_1) == GLFW_PRESS)
	{
		camera.CenterCameraOnMesh(*monkey.GetTransform().GetPosition(), -5);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_2) == GLFW_PRESS)
	{
		camera.CenterCameraOnMesh(*teapot.GetTransform().GetPosition(), -10);
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_3) == GLFW_PRESS)
	{
		camera.CenterCameraOnMesh(*capsule.GetTransform().GetPosition(), -15);
	}
	//for pointing camera at a mesh
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_4) == GLFW_PRESS)
	{
		camera.PointCameraAtMesh(*monkey.GetTransform().GetPosition());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_5) == GLFW_PRESS)
	{
		camera.PointCameraAtMesh(*teapot.GetTransform().GetPosition());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_6) == GLFW_PRESS)
	{
		camera.PointCameraAtMesh(*capsule.GetTransform().GetPosition());
	}

	//camera mouse input
	camera.MouseControls(gameDisplay);
}

void Game::UpdateDisplay()
{
	fbo.BindFBO();
	gameDisplay->ClearDisplay(0.0f, 0.0f, 0.0f, 1.0f); //clear the display

	//shader.UseShader();
	//fogshader.UseShader();
	//toonshader.UseShader();
	//LinkRimLightingShaderData();
	//rimshader.UseShader();

	reflectionshader.UseShader();
	LinkReflectionShaderData();
	//MESH1
	//shader.UpdateTransform(mesh1.transform, camera);
	//fogshader.UpdateTransform(mesh1.transform, camera);
	//fogshader.setMat4("ModelMatrix", mesh1.transform.GetModel());
	//toonshader.UpdateTransform(mesh1.transform, camera);
	//rimshader.UpdateTransform(mesh1.transform, camera);
	//toonrimshader.UpdateTransform(mesh1.transform, camera);
	//geoshader.UpdateTransform(mesh1.transform, camera);
	//reflectionshader.UpdateTransform(monkey.GetTransform(), camera);
	fbograyscaleshader.UpdateTransform(monkey.GetTransform(), camera);
	textures.UseTexture(0);
	monkey.SetTransformParameters(glm::vec3(-1.0, 0.0, 0.0), glm::vec3(counter, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	monkey.DisplayMesh();

	geoshader.UseShader();
	LinkGeoShaderData();
	//MESH2
	//shader.UpdateTransform(mesh2.transform, camera);
	//fogshader.UpdateTransform(mesh2.transform, camera);
	//fogshader.setMat4("ModelMatrix", mesh2.transform.GetModel());
	//toonshader.UpdateTransform(mesh2.transform, camera);
	//rimshader.UpdateTransform(mesh2.transform, camera);
	//toonrimshader.UpdateTransform(mesh2.transform, camera);
	geoshader.UpdateTransform(teapot.GetTransform(), camera);
	//reflectionshader.UpdateTransform(mesh2.transform, camera);
	textures.UseTexture(1);
	teapot.SetTransformParameters(glm::vec3(0.0, sinf(counter) * 5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1));
	teapot.DisplayMesh();

	adsshader.UseShader();
	LinkADSShaderData();
	//MESH3
	//shader.UpdateTransform(mesh3.transform, camera);
	//fogshader.UpdateTransform(mesh3.transform, camera);
	//fogshader.setMat4("ModelMatrix", mesh3.transform.GetModel());
	//toonshader.UpdateTransform(mesh3.transform, camera);
	//rimshader.UpdateTransform(mesh3.transform, camera);
	//toonrimshader.UpdateTransform(mesh3.transform, camera);
	//geoshader.UpdateTransform(mesh3.transform, camera);
	//reflectionshader.UpdateTransform(mesh3.transform, camera);
	adsshader.UpdateTransform(capsule.GetTransform(), camera);
	textures.UseTexture(2);
	capsule.SetTransformParameters(glm::vec3(3.0, 0.0, sinf(counter) * 3), glm::vec3(0.0, counter, 0.0), glm::vec3(1.0, 1.0, 1.0));
	capsule.DisplayMesh();

	//ASTEROIDS
	shader.UseShader();
	textures.UseTexture(3);
	for (int i = 0; i < sizeof(asteroids) / sizeof(GameObject); i++) //TODO: make loop count backwards for increased performance
	{
		shader.UpdateTransform(asteroids[i].GetTransform(), camera);
		asteroids[i].SetTransformParameters(*asteroids[i].GetTransform().GetPosition(), glm::vec3(counter, 0.0, 0.0), glm::vec3(0.001, 0.001, 0.001)); //TODO: use deltatime instead of counter?
		asteroids[i].DisplayMesh();
	}

	DisplaySkybox();

	counter += 0.001f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	fbo.UnbindFBO();
	fboshader.UseShader();
	fbo.RenderFBOtoQuad();
	gameDisplay->ChangeBuffer(); //swap the buffers
}

bool Game::DetectCollision(glm::vec3 position1, float radius1, glm::vec3 position2, float radius2)
{
	float distance = glm::sqrt((position2.x - position1.x) * (position2.x - position1.x) + (position2.y - position1.y) * (position2.y - position1.y) + (position2.z - position1.z) * (position2.z - position1.z)); //pythagoras

	if (distance < (radius1 + radius2))
	{
		cout << "collision! : " << distance << '\n';
		audio.PlaySound(0); //plays a sound if sound isn't already playing
		return true;
	}
	else
	{
		cout << "NO collision! : " << distance << '\n';
		return false;
	}
}