#include "Game.h"

//unsigned int indices[] = { 0, 1, 2 };

Game::Game() : cubemapTexture(0), skyboxVAO(0), skyboxVBO(0)
{
	gameState = GameState::PLAYING; //set the game state to PLAYING
	gameDisplay = new ScreenDisplay(); //create a new display

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

	spaceship.LoadModel("..\\res\\spaceship.obj"); //loads a mesh from file
	spaceship.SetTransformParameters(glm::vec3(0, 0, 0), glm::vec3(-90.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2)); //initialize spaceship position
	laser.LoadModel("..\\res\\bullet.obj");
	for (int i = 0; i < sizeof(asteroids) / sizeof(GameObject); i++) //TODO: make loop count backwards for increased performance
	{
		asteroids[i].LoadModel("..\\res\\asteroid.obj");

		//initialize position of each asteroid
		float randomX = rand() % 11 + (-5); //random integer between -5 and 5
		float randomY = rand() % 11 + (-5);
		float randomZ = rand() % 11 + (-5);
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

	textures.InitializeTexture("..\\res\\spaceshiptexture.jpg"); //load a texture
	textures.InitializeTexture("..\\res\\bullettexture.jpg"); 
	textures.InitializeTexture("..\\res\\rock.jpg");

	camera.InitializeCamera(glm::vec3(0, 10, 0), 70.0f, (float)gameDisplay->GetWidth() / gameDisplay->GetHeight(), 0.01f, 1000.0f); //initializes the camera
	camera.CenterCameraOnMesh(*spaceship.GetTransform().GetPosition(), -10.0f);
	minimapCamera.InitializeCamera(glm::vec3(0, 10, 0), 70.0f, (float)gameDisplay->GetWidth() / gameDisplay->GetHeight(), 0.01f, 1000.0f); //initializes the camera
	minimapCamera.CenterCameraOnMesh(*spaceship.GetTransform().GetPosition(), -20.0f);
	minimapFBO.GenerateFBO(gameDisplay->GetWidth(), gameDisplay->GetHeight());
	minimapFBO.GenerateQuad();
	displayFBO.GenerateFBO(gameDisplay->GetWidth(), gameDisplay->GetHeight());
	displayFBO.GenerateFullQuad();

	audio.AddNewSound("..\\res\\shoot.wav");
	audio.AddNewSound("..\\res\\zap.wav");
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
	rimshader.setMat4("m", laser.GetModelMatrix());
}

void Game::LinkToonRimShaderData(Camera camera)
{
	toonrimshader.setVec3("lightDir", camera.GetPosition());
	toonrimshader.setMat4("m", laser.GetModelMatrix()); //TODO: ...laser?
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
	adsshader.setMat4("model", laser.GetModelMatrix());
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
		UpdateScreen();

		//collision detection
		for (int i = 0; i < sizeof(asteroids) / sizeof(GameObject); i++) //TODO: make loop count backwards for increased performance
		{
			if (DetectCollision(asteroids[i].boundingSphere.GetPosition(), asteroids[i].boundingSphere.GetRadius(), laser.boundingSphere.GetPosition(), laser.boundingSphere.GetRadius()))
			{
				audio.PlaySound(1); //plays a sound if sound isn't already playing
				asteroids[i].SetActive(false);
				break;
			}

			if (DetectCollision(asteroids[i].boundingSphere.GetPosition(), asteroids[i].boundingSphere.GetRadius(), spaceship.boundingSphere.GetPosition(), spaceship.boundingSphere.GetRadius()))
			{
				asteroids[i].SetActive(false);
				spaceship.SetActive(false);
				Exit("\n   -> GAME OVER! Spaceship collided with an asteroid. <-\n");
				return;
			}
		}

		UpdateDelta();
		counter += 0.001f;
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

	//for spaceship movement
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(0, 5, 0) * deltaTime, *spaceship.GetTransform().GetRotation(), *spaceship.GetTransform().GetScale());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(0, -5, 0) * deltaTime, *spaceship.GetTransform().GetRotation(), *spaceship.GetTransform().GetScale());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(5, 0, 0) * deltaTime, *spaceship.GetTransform().GetRotation(), *spaceship.GetTransform().GetScale());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(-5, 0, 0) * deltaTime, *spaceship.GetTransform().GetRotation(), *spaceship.GetTransform().GetScale());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(0, 0, 5) * deltaTime, *spaceship.GetTransform().GetRotation(), *spaceship.GetTransform().GetScale());
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_E) == GLFW_PRESS)
	{
		spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(0, 0, -5) * deltaTime, *spaceship.GetTransform().GetRotation(), *spaceship.GetTransform().GetScale());
	}
	//for spaceship firing
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_F) == GLFW_PRESS)
	{
		laser.SetTransformParameters(*spaceship.GetTransform().GetPosition() + glm::vec3(0, 5, 0) * deltaTime, *laser.GetTransform().GetRotation(), glm::vec3(0.05, 0.05, 0.05));
		audio.PlaySound(0); //plays a sound if sound isn't already playing
	}

	//for switching between camera modes
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_T) == GLFW_PRESS)
	{
		camera.ToggleMode();
	}
	if (glfwGetKey(gameDisplay->window, GLFW_KEY_L) == GLFW_PRESS)
	{
		cameraLock = !cameraLock;
	}

	//for free camera movement
	if (!cameraLock)
	{
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			camera.MoveCameraVertically(cameraSpeed * deltaTime);
		}
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			camera.MoveCameraVertically(-cameraSpeed * deltaTime);
		}
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			camera.MoveCameraHorizontally(cameraSpeed * deltaTime);
		}
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			camera.MoveCameraHorizontally(-cameraSpeed * deltaTime);
		}
		//for centering camera on a mesh
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_1) == GLFW_PRESS)
		{
			camera.CenterCameraOnMesh(*spaceship.GetTransform().GetPosition(), -5);
		}
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_2) == GLFW_PRESS)
		{
			camera.PointCameraAtMesh(*spaceship.GetTransform().GetPosition());
		}
		//for rotating camera around a mesh
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_3) == GLFW_PRESS)
		{
			camera.RotateCameraAroundMesh(*spaceship.GetTransform().GetPosition(), cameraSpeed * deltaTime);
		}
		if (glfwGetKey(gameDisplay->window, GLFW_KEY_4) == GLFW_PRESS)
		{
			camera.RotateCameraAroundMesh(*spaceship.GetTransform().GetPosition(), -cameraSpeed * deltaTime);
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
	}

	//camera mouse input
	camera.MouseControls(gameDisplay);
}

void Game::UpdateScreen()
{
	gameDisplay->ClearDisplay(0.0f, 0.0f, 0.0f, 1.0f); //clear the display


	//MAIN VIEW FBO
	displayFBO.BindFBO();

	DrawDisplay();
	fboshader.UseShader();

	displayFBO.UnbindFBO();
	displayFBO.RenderFBOtoQuad();

	//MINIMAP FBO
	minimapFBO.BindFBO();

	DrawMinimap();
	fbograyscaleshader.UseShader();

	minimapFBO.UnbindFBO();
	minimapFBO.RenderFBOtoQuad();


	gameDisplay->ChangeBuffer(); //swap the buffers
}

void Game::DrawDisplay()
{
	if (cameraLock)
	{
		if (camera.GetMode())
		{
			camera.CenterCameraOnMesh(*spaceship.GetTransform().GetPosition(), -10);
		}
		else
		{
			camera.CenterCameraBehindMesh(*spaceship.GetTransform().GetPosition(), -10);
		}
	}

	toonrimshader.UseShader();
	LinkToonRimShaderData(camera);

	//SPACESHIP
	textures.UseTexture(0);
	toonrimshader.UpdateTransform(spaceship.GetTransform(), camera);
	spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition(), glm::vec3(-90.0, 0.0, 5 * sinf(deltaTime)), glm::vec3(0.25, 0.25, 0.25)); //wobble effect
	spaceship.DisplayMesh();

	//BULLET
	textures.UseTexture(1);
	toonrimshader.UpdateTransform(laser.GetTransform(), camera);
	laser.SetTransformParameters(*laser.GetTransform().GetPosition() + glm::vec3(0, 5, 0) * deltaTime, *laser.GetTransform().GetRotation(), glm::vec3(0.05, 0.05, 0.05));
	laser.DisplayMesh();

	//ASTEROIDS
	textures.UseTexture(2);
	for (int i = 0; i < sizeof(asteroids) / sizeof(GameObject); i++) //TODO: make loop count backwards for increased performance
	{
		toonrimshader.UpdateTransform(asteroids[i].GetTransform(), camera);
		asteroids[i].SetTransformParameters(*asteroids[i].GetTransform().GetPosition(), glm::vec3(counter, counter, counter), glm::vec3(0.001, 0.001, 0.001)); //TODO: use deltatime instead of counter?
		asteroids[i].DisplayMesh();
	}

	DisplaySkybox();
}

void Game::DrawMinimap()
{
	//shader.UseShader();
	toonrimshader.UseShader();
	LinkToonRimShaderData(minimapCamera);

	//SPACESHIP
	textures.UseTexture(0);
	toonrimshader.UpdateTransform(spaceship.GetTransform(), minimapCamera);
	spaceship.SetTransformParameters(*spaceship.GetTransform().GetPosition(), glm::vec3(-90.0, 0.0, 5 * sinf(deltaTime)), glm::vec3(0.25, 0.25, 0.25)); //wobble effect
	spaceship.DisplayMesh();

	//BULLET
	textures.UseTexture(1);
	toonrimshader.UpdateTransform(laser.GetTransform(), minimapCamera);
	laser.SetTransformParameters(*laser.GetTransform().GetPosition() + glm::vec3(0, 5, 0) * deltaTime, *laser.GetTransform().GetRotation(), glm::vec3(0.05, 0.05, 0.05));
	laser.DisplayMesh();

	//ASTEROIDS
	textures.UseTexture(2);
	for (int i = 0; i < sizeof(asteroids) / sizeof(GameObject); i++) //TODO: make loop count backwards for increased performance
	{
		toonrimshader.UpdateTransform(asteroids[i].GetTransform(), minimapCamera);
		asteroids[i].SetTransformParameters(*asteroids[i].GetTransform().GetPosition(), glm::vec3(counter, counter, counter), glm::vec3(0.001, 0.001, 0.001)); //TODO: use deltatime instead of counter?
		asteroids[i].DisplayMesh();
	}

	DisplaySkybox();
}

bool Game::DetectCollision(glm::vec3 position1, float radius1, glm::vec3 position2, float radius2)
{
	float distance = glm::sqrt((position2.x - position1.x) * (position2.x - position1.x) + (position2.y - position1.y) * (position2.y - position1.y) + (position2.z - position1.z) * (position2.z - position1.z)); //pythagoras

	if (distance < (radius1 + radius2))
	{
		return true;
	}
	else
	{
		return false;
	}
}