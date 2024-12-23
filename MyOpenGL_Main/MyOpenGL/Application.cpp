#include "Application.h"

std::string vs = ShaderLoader::LoadShaderFromFile("Shaders/VertexShader.vs");
std::string fs = ShaderLoader::LoadShaderFromFile("Shaders/FragmentShader.fs");

Application::~Application()
{
    delete mScene;
    mScene = nullptr;
    glfwDestroyWindow(mWindow);
}

Application* Application::GetAPP()
{
    static Application* app = new Application("Test Run", 1920, 1080);
    return app;
}

///Init functions
void Application::GLFW_Init()
{
    ///Initializing the GLFWWindow
    std::cout << "Program Running\n";
    std::cout << "------------------------------------------------------------------------------------" << "\n\n";

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::Window_Init()
{   ///Creating the actual window
    mWindow = glfwCreateWindow(mWidth, mHeight, "Test Win", NULL, NULL);

    if (mWindow == NULL)
    {
        /*std::cout << "Failed to create GLFW window" << std::endl;*/
        assert(mWindow && "Failed to Load GLFWwindow!\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
 

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //std::cout << "Failed to initialize GLAD" << std::endl
        assert("Failed to Load GladLoad!\n");
    }

    glEnable(GL_DEPTH_TEST); 
}

void Application::Run_App()
{
    ///Creating the Window for display and registering callbacks
    GLFW_Init();
    Window_Init();
    RegisterWindowCallbacks();
  
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Scene* scene1 = new Scene("Scene1"); 

    ///Fillig the unordered map
    mScene = scene1;
    mScene->LoadContent();
 
    
    ///Creating the deltaTime variable
    float lastFrame = static_cast<float>(glfwGetTime());
    float timer = 0;
  
    // render loop
    // -----------
    while (!glfwWindowShouldClose(mWindow)) 
    {
        if (mScene->uActorMap.find("Player")->second->Restart())
        {
            std::cout << "\n\n";
            std::cout << "[LOG]:Player is dead. Exiting application \n";
            mScene->UnloadContent();
            this->~Application();

            // glfw: terminate, clearing all previously allocated GLFW resources.
            // ------------------------------------------------------------------
            glfwTerminate();
        };

        //DeltaTime Controller
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timer += deltaTime;

        if (timer > 1)
        {
            timer = 0;
            std::cout << "\n";
            std::cout << 1 / deltaTime << "\n";
            std::cout << "\n";

        }

        UpdateCameraController(deltaTime); 
        UpdateActorMovement(deltaTime);   
        UpdateCameraPlacement(deltaTime);

        // input
        // -----
        UtilyKeyBinds(deltaTime);    

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // render the Actores
        mScene->RenderScene(deltaTime);  
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    mScene->UnloadContent();
    this->~Application();
  
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}



///Callback Controlls
void Application::RegisterWindowCallbacks()
{
    glfwSetFramebufferSizeCallback
    (mWindow, [](GLFWwindow* window, int width, int height)
        {
            auto windowApplication = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
            if (windowApplication)
            {
                windowApplication->FramebufferSizeCallback(window, width, height);
                windowApplication->SetHeight(height);
                windowApplication->SetWidth(width);
            }
        }
    );

    glfwSetKeyCallback
    (mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto windowApplication = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
            if (windowApplication)
            {
                windowApplication->KeyCallback(window, key, scancode, action, mods);
            }
        }
    );

    glfwSetMouseButtonCallback
    (mWindow, [](GLFWwindow* window, int button, int action, int mods)
        {
            auto windowApplication = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
            if (windowApplication)
            {
                windowApplication->MouseButtonCallback(window, button, action, mods);
            }
        }
    );

    glfwSetCursorPosCallback
    (mWindow, [](GLFWwindow* window, double xPos, double yPos) 
        {
            auto windowApplication = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
            if (windowApplication)
            {
                windowApplication->CursorPosCallback(window, xPos, yPos);  
            }
        }
    );

    glfwSetWindowUserPointer(mWindow,this);  
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) const
{
    glViewport(0, 0, width, height);
    mScene->mSceneCamera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height)); //set Framebuffer according to aspect ratio
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //Using map for stateflow on key-callbacks
    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
        mKeyState[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        mKeyState[key] = false; 
    }

    UpdateCameraController(0);
    UpdateActorMovement(0);
    UpdateCameraPlacement(0);
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //Using map for stateflow on button-callbacks
    if (action == GLFW_PRESS)
    {
        mButtonState[button] = true;
        
    }
    else if (action == GLFW_RELEASE)
    {
        mButtonState[button] = false; 
    }
}

void Application::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
    //Checking if spesific button is called. Takes integer "button" anc checks in MousButtonCallback
    if (mButtonState[GLFW_MOUSE_BUTTON_RIGHT])
    {
        //Disabling the visible cursor for when rotating the camera
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &xPos, &yPos);  
        mScene->mSceneCamera->CameraMouseMovement(xPos, yPos);  
        mScene->mSceneCamera->CameraMouseButton(xPos, yPos);   
    }
    else
    {
        //Enabling the visible cursor for when not rotating the camera
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mScene->mSceneCamera->mRightMouseButtonPressed = false; 
    }

}

///Setters
void Application::SetHeight(int height)
{
    mHeight = height;
}

void Application::SetWidth(int width)
{
    mWidth = width;
}

///Camera Updta Controller
void Application::UpdateCameraController(float dt)
{
    //Forward and Backwards
    if (!mScene->mSceneCamera->mUseCameraMovement) return;

    if (mKeyState[GLFW_KEY_W]) mScene->mSceneCamera->CameraMovement(Direction::Forward,dt)/*, std::cout << "W pressed\n" << std::endl*/;
    if (mKeyState[GLFW_KEY_S]) mScene->mSceneCamera->CameraMovement(Direction::Backwards, dt)/*, std::cout << "S pressed\n" << std::endl*/;

    //Left and right
    if (mKeyState[GLFW_KEY_D]) mScene->mSceneCamera->CameraMovement(Direction::Right, dt)/*, std::cout << "D pressed\n" << std::endl*/;
    if (mKeyState[GLFW_KEY_A]) mScene->mSceneCamera->CameraMovement(Direction::Left, dt)/*, std::cout << "A pressed\n" << std::endl*/;

    //Up and Down
    if (mKeyState[GLFW_KEY_SPACE]) mScene->mSceneCamera->CameraMovement(Direction::Up, dt)/*, std::cout << "Space pressed\n" << std::endl*/;
    if (mKeyState[GLFW_KEY_LEFT_ALT]) mScene->mSceneCamera->CameraMovement(Direction::Down, dt)/*, std::cout << "Alt pressed\n" << std::endl*/;

    //Increase of Speed
    if (mKeyState[GLFW_KEY_LEFT_SHIFT]) mScene->mSceneCamera->CameraMovement(Direction::IncreaseSpeed, dt)/*, std::cout << "Shift pressed\n" << std::endl*/;

}
 
void Application::UpdateActorMovement(float dt) 
{    
        if (mScene->uActorMap.find("Player")->second->isPlayer)
        {
            //Movement Keys
            if (mKeyState[GLFW_KEY_W]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::Forward, mScene->mSceneCamera, dt);
            if (mKeyState[GLFW_KEY_A]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::Left, mScene->mSceneCamera, dt);
            if (mKeyState[GLFW_KEY_S]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::Backwards, mScene->mSceneCamera, dt);
            if (mKeyState[GLFW_KEY_D]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::Right, mScene->mSceneCamera, dt);
            if (mKeyState[GLFW_KEY_SPACE]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::Up, mScene->mSceneCamera, dt);
            if (mKeyState[GLFW_KEY_LEFT_ALT]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::Down, mScene->mSceneCamera, dt);

            //Increase Speed
            if (mKeyState[GLFW_KEY_LEFT_SHIFT]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::IncreaseSpeed, mScene->mSceneCamera, dt);
            if (!mKeyState[GLFW_KEY_LEFT_SHIFT]) mScene->uActorMap.find("Player")->second->ActorMovement(Direction::NormalSpeed, mScene->mSceneCamera, dt);
        }

        if (mScene->uActorMap.find("Player")->second->isPlayer)
        {
            if(mButtonState[GLFW_MOUSE_BUTTON_LEFT]) mScene->uActorMap.find("Player")->second->Shoot(Mouse::LeftMouseButton,dt);  
        }


}

void Application::UpdateCameraPlacement(float dt)
{ 
    if (mScene->uActorMap.find("Player")->second->isPlayer)
    {
        if (mKeyState[GLFW_KEY_1]) mScene->uActorMap.find("Player")->second->CameraStateControll(CameraState::CameraFreeMovement_1, mScene->mSceneCamera, dt);
        if (mKeyState[GLFW_KEY_2]) mScene->uActorMap.find("Player")->second->CameraStateControll(CameraState::CameraStatic_CharacterMovement_2, mScene->mSceneCamera, dt);
        if (mKeyState[GLFW_KEY_3]) mScene->uActorMap.find("Player")->second->CameraStateControll(CameraState::CameraStatic_FollowPlayer_3, mScene->mSceneCamera, dt);
    
    }
}

void Application::UtilyKeyBinds(float dt)
{
    if (mKeyState[GLFW_KEY_ESCAPE])
    {
        std::cout << "Exit-Key Pressed" << std::endl;
        glfwSetWindowShouldClose(mWindow, true);
    }
    if (mKeyState[GLFW_KEY_8] && seeWireFrame == false) 
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        seeWireFrame = true;
        return;
    }
    if (mKeyState[GLFW_KEY_9] && seeWireFrame == true)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
        seeWireFrame = false;
        return;
    }
}







