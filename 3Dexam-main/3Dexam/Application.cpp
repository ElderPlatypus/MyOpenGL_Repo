#include "Application.h"

//Includes
#include "Shaders/ShaderFileLoader.h"
#include "Shaders/Shader.h"

//GLad/GLM etc
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Midelertidig
#include "SceneFolder/Actor.h"
#include "CameraFolder/Camera.h"
#include "Utility/Transform.h"
#include "SceneFolder/Scene.h"

std::string vs = ShaderLoader::LoadShaderFromFile("Shaders/Triangle.vs");
std::string fs = ShaderLoader::LoadShaderFromFile("Shaders/Triangle.fs");

Application::~Application()
{
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
  
    // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Scene* scene1 = new Scene("Scene1");
    scene1->LoadContent();


    ///Test
    unsigned int texture1; 
    // texture 1
    // ---------
    glGenTextures(1, &texture1); 
    glBindTexture(GL_TEXTURE_2D, texture1); 
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("Shaders/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl; 
    } 
    stbi_image_free(data); 
    scene1->mShader->use(); 
    scene1->mShader->setInt("texture1",0);

    ///Creating the deltaTime variable
    float lastFrame = static_cast<float>(glfwGetTime());
  
    // render loop
    // -----------
    while (!glfwWindowShouldClose(mWindow)) 
    {
        ///DeltaTime Controller
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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render the Actores
        scene1->mShader->use();
       
        scene1->RenderScene(deltaTime);  
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    scene1->UnloadContent();
  
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void Application::Original_Run_App()
{
    ///Creating the Window for display and registering callbacks
    GLFW_Init();
    Window_Init();
    RegisterWindowCallbacks();

    Shader orgShader("Shaders/Triangle.vs", "Shaders/Triangle.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float Triangle2D[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f,1.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,1.0f, // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,1.0f // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle2D), Triangle2D, GL_STATIC_DRAW); 
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); 
    glEnableVertexAttribArray(2); 
   /* 
     You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
     VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.*/
     glBindVertexArray(0);
    
    ///Creating the deltaTime variable
    float lastFrame = static_cast<float>(glfwGetTime());
    // render loop
    // -----------
    while (!glfwWindowShouldClose(mWindow))
    {
        ///DeltaTime Controller
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----

        ExitApplication(deltaTime);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        orgShader.use(); 

        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);  

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO); 
    glDeleteBuffers(1, &VBO); 

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




