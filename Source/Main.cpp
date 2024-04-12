

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imconfig.h"
#include <iostream>



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "..\Resources\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "VertexBuffer.h"
#include <vector>

Camera camera(glm::vec3(0.0f, 6.0f, 15.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

int gridSize = 20;
float cellSize = 1.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//bool isMouseCaptured = false;
//bool isRightMouseButtonPressed = false;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void generate_grid(int gridSize, float cellSize, std::vector<float>& vertices, GLuint grid);
std::vector<float> gridVert;

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "SimpleRender", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
   
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);

  
   
    Shader myShader("shader.vert", "shader.frag");
    
   // glfwSetCursorPosCallback(window, mouse_callback);
  //  glfwSetMouseButtonCallback(window, mouse_button_callback);
   //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  //  ImGui_ImplGlfw_MouseButtonCallback(window, mouse_button_callback);
   // ImGui_ImplGlfw_CursorPosCallback(window, cursor_position_callback);

    //float points[] = {
    //    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    //};
    float cubeVert[] = {
        // Front face
         -1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,

         // Back face
         -1.0f, -1.0f,  1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

         // Left face
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,

         // Right face
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,

          // Top face
          -1.0f,  1.0f, -1.0f,
           1.0f,  1.0f, -1.0f,
           1.0f,  1.0f,  1.0f,
           1.0f,  1.0f,  1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f,  1.0f, -1.0f,

          // Bottom face
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
    };

    
    

    GLuint VAO, grid;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexBuffer cubeVBO(cubeVert, sizeof(cubeVert));   

    glGenBuffers(1, &grid);
    generate_grid(gridSize, cellSize, gridVert, grid);
    
    myShader.use();
    myShader.setVec3("colorStart", glm::vec3(0.8f, 0.0f, 0.9f)); // Start color (e.g., red)
    myShader.setVec3("colorEnd", glm::vec3(0.0f, 0.8f, 0.3f));
    //setting projection matrix
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
    myShader.setMat4("projection", projection);

   //glfwSetMouseButtonCallback(window, mouse_button_callback);
   //glfwSetCursorPosCallback(window, mouse_callback);

    glm::vec3 cubeTranslation(0, 1.0, 0);
    glm::vec3 cubeRotation(0.0f, 0.0f, 0.0f);
    glm::vec3 cubeScale(1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        
        ImGui::NewFrame();

        
        if (ImGui::Begin("Visualization"))
        {
           
            ImGui::Text("Translation");
            ImGui::SliderFloat3("position", &cubeTranslation.x, -25.0f, 25.0f);
            ImGui::SameLine();
            ImGui::Text("     (use WASD to move camera!)");
            ImGui::Text("Rotation");
            ImGui::SliderFloat3("Rotation", &cubeRotation.x, -180.0f, 180.0f);
            ImGui::SameLine();
            ImGui::Text("     (Mouse look in progress.)");
            ImGui::Text("Scale");
            ImGui::SliderFloat3("Scale", &cubeScale.x, -40.0f, 40.0f);
            ImGui::Text("Grid size: ");
            ImGui::InputInt("##InputValue", &gridSize, 0.1f, 1.0f);

            if (ImGui::Button("Submit"))
            {
                generate_grid(gridSize, cellSize, gridVert, grid);
            }
        }ImGui::End();
        ImGuiIO& io = ImGui::GetIO();
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();
        myShader.setVec3("myColor", 1.0f, 1.0f, 1.0f);

        glBindBuffer(GL_ARRAY_BUFFER, grid);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("view", view);
      
        glm::mat4 model = glm::mat4(1.0f);
        myShader.setMat4("model", model);
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);

       glDrawArrays(GL_LINES, 0, gridVert.size()/3);

       cubeVBO.Bind();
       myShader.setVec3("myColor", 0.902f, 0.494f, 0.055f);
       glm::mat4 cubeModel = glm::mat4(1.0f);
       cubeModel = glm::translate(cubeModel, cubeTranslation);
       cubeModel = glm::rotate(cubeModel, glm::radians(cubeRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
       cubeModel = glm::rotate(cubeModel, glm::radians(cubeRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
       cubeModel = glm::rotate(cubeModel, glm::radians(cubeRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis
       cubeModel = glm::scale(cubeModel, cubeScale);
      // cubeModel = glm::scale(cubeModel, scaleVector);  // Scale
       myShader.setMat4("model", cubeModel);

       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
       glDrawArrays(GL_TRIANGLES, 0, 36);

       ImGui::Render();
       ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       glfwSwapBuffers(window);
       glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    

}

// Can't figure out swapping between windows oh well.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Get ImGui's IO
    io.MouseDown[button] = true;

    if (!io.WantCaptureMouse)
    {
        std::cout << "Handling mouse button for application" << std::endl;
        // Your existing mouse button handling
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else if (action == GLFW_RELEASE)
            {
                
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
    // ImGui's mouse button event handling
    if (action == GLFW_PRESS && io.WantCaptureMouse)
    {
        io.MouseDown[button] = true;
        io.AddMouseButtonEvent(button, true);
    }
    else if (action == GLFW_RELEASE)
    {
        io.MouseDown[button] = false;
        io.AddMouseButtonEvent(button, false);
    }
    std::cout << "ImGui capture mouse: " << io.WantCaptureMouse << std::endl;
}
void generate_grid(int gridSize, float cellSize, std::vector<float>& vertices, GLuint grid)
{
    constexpr int maxSize = 250;
    if (gridSize > maxSize)
        gridSize = maxSize;

    vertices.clear(); 

    for (int i = 0; i <= gridSize; ++i)
    {
        float x = -gridSize * cellSize / 2.0f + i * cellSize;
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(-gridSize * cellSize / 2.0f);

        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(gridSize * cellSize / 2.0f);

        float z = -gridSize * cellSize / 2.0f + i * cellSize;
        vertices.push_back(-gridSize * cellSize / 2.0f);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        vertices.push_back(gridSize * cellSize / 2.0f);
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }
    glBindBuffer(GL_ARRAY_BUFFER, grid);
    glBufferData(GL_ARRAY_BUFFER, gridVert.size() * sizeof(float), gridVert.data(), GL_STATIC_DRAW);
}

