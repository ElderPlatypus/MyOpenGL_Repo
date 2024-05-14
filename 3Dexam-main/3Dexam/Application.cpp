#include "Application.h"

//Includes
#include "Shaders/ShaderFileLoader.h"
#include "Shaders/Shader.h"

//GLad/GLM etc

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"


std::string vs = ShaderLoader::LoadShaderFromFile("Shaders/Triangle.vs");
std::string fs = ShaderLoader::LoadShaderFromFile("Shaders/Triangle.fs");

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
    std::cout << "Vertex Shader:\n\n " << vs.c_str() << std::endl;
    std::cout << "\n\n" << "------------------------------------------------------------------------------------" << "\n\n";
    std::cout << "Fragment Shader:\n\n " << fs.c_str() << std::endl;  
    std::cout << "\n\n" << "------------------------------------------------------------------------------------" << "\n\n";

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
        glfwTerminate();
        assert(mWindow && "Failed to Load GLFWwindow!");
    }
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
 

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //std::cout << "Failed to initialize GLAD" << std::endl
        assert("Failed to Load GladLoad!");
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
    /*scene1->LoadContent(); */   

    ///Fillig the unordered map
    mScene = scene1;
    mScene->LoadContent();

    
    
    ///Creating the deltaTime variable
    float lastFrame = static_cast<float>(glfwGetTime());
  
    // render loop
    // -----------
    while (!glfwWindowShouldClose(mWindow)) 
    {
        //DeltaTime Controller
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
       
        // input
        // -----
        ExitApplication(deltaTime);   
        float timer = (float)glfwGetTime();

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

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    mScene->mSceneCamera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
     
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   
    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
        mKeyState[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        mKeyState[key] = false; 
    }

    UpdateActiveController(0); 
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void Application::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
   
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


///Util
void Application::ExitApplication(float dt)
{
    if (mKeyState[GLFW_KEY_ESCAPE])
    { 
        std::cout << "Exit-Key Pressed" << std::endl;
        glfwSetWindowShouldClose(mWindow, true); 
    }
}



///Test
void Application::UpdateActiveController(float dt)
{
    //Forward and Backwards
    if (mKeyState[GLFW_KEY_W]) mScene->mSceneCamera->CameraMovement(forward,dt), std::cout << "W pressed\n" << std::endl;
    if (mKeyState[GLFW_KEY_S]) mScene->mSceneCamera->CameraMovement(backwards, dt), std::cout << "S pressed\n" << std::endl;

    //Left and right
    if (mKeyState[GLFW_KEY_D]) mScene->mSceneCamera->CameraMovement(right, dt), std::cout << "D pressed\n" << std::endl;
    if (mKeyState[GLFW_KEY_A]) mScene->mSceneCamera->CameraMovement(left, dt), std::cout << "A pressed\n" << std::endl;

    //Up and Down
    if (mKeyState[GLFW_KEY_SPACE]) mScene->mSceneCamera->CameraMovement(up, dt), std::cout << "Space pressed\n" << std::endl;
    if (mKeyState[GLFW_KEY_LEFT_ALT]) mScene->mSceneCamera->CameraMovement(down, dt), std::cout << "Alt pressed\n" << std::endl;

    //Increase of Speed
    if (mKeyState[GLFW_KEY_LEFT_SHIFT]) mScene->mSceneCamera->CameraMovement(speed, dt), std::cout << "Shift pressed\n" << std::endl;
    else if (!mKeyState[GLFW_KEY_LEFT_SHIFT]) mScene->mSceneCamera->CameraMovement(speed, dt);

    /*glm::vec3 getPos = mScene->mSceneCamera->mTransform.GetPosition();
    std::cout << "getcameraPos:" << getPos.x << " " << getPos.y << " " << getPos.z << std::endl;*/

}




