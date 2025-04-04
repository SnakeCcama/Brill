#include <iostream>

#include "includes/gl.h"
#include "includes/gl.c"

#include "includes/sheader.hpp"
#include "includes/cam.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

float width = 599.0, height = 600.0;
Camera camera(0.0, 0.0, 3.0, 0.0, 1.0, 0.0, -90.0f, 0.0f);
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::vector<float> barVertices = {
    
    // first triangle
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f,  0.5f, 0.0f,  // top left
   -0.5f, -0.5f, 0.0f,  // bottom left

};

std::vector<int> barIndices = {
    
    0,1,3, //first triangle
    0,3,2,   //second triangle    

};



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window)
{
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
        }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
        }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
        }


}

std::vector<float> genPolygon(float radius, int noSides)
{
    std::vector<float> vertices;
   
    float sectorStepSize=  2*M_PI/noSides;
    float stackStepSize = M_PI/ noSides;
    float sectorAngle, stackAngle;

    float x, y, z, xy;
    
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);

    for (int i=0; i<=noSides ; ++i)
    {
        stackAngle = M_PI/2 - sectorStepSize * i;
        z= radius * sinf(stackAngle);
        xy= radius * cosf(stackAngle);

        for (int j=0; j<=noSides; ++j)
        {
            sectorAngle= j * sectorStepSize;
            x = radius * cos(sectorAngle);
            y = radius * sin(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            
        }
    }
   
   
    return vertices;
}

std::vector<int> genIndices(float noSides)
{
    std::vector<int>indices;

    for (int i = 0; i< noSides; ++i)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    return indices;
}

void drawPolygon(std::vector <float>& vertices, std::vector<int>& indices)
{
    
    
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    GLuint iboId;
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    //glDeleteBuffers(1, &txVBO);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &iboId);
    glDeleteVertexArrays(1, &vaoId); 


}

void drawBar(std::vector<float>& vertices, std::vector<int>& indices)
{
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLuint iboId;
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
   

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);

    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);    
}

void barStuff(GLFWwindow* window, Shader& aShader)
{

    glm::mat4 move = glm::mat4(1.0f);
    move = glm::translate(move, glm::vec3(0.0f, 0.6f, 0.0f));
    move = glm::scale(move, glm::vec3(1.0f, 0.08f, 0.0f));

    aShader.setthisMat4("modelM", move);


}

template <typename T>
bool inRange(T value, T min, T max)
{
    return value >= min && value<= max;
}

int lastMouse;

int checkMouse(GLFWwindow* window)
{
    glm::vec2 mousePos;
    double mouseX, mouseY;
    int sCount;
    
    glfwGetCursorPos(window, &mouseX, &mouseY);

    mouseX = 2*mouseX/width - 1;
    mouseY = 1 - 2*mouseY/height;

    if (inRange(mouseX, -0.5, 0.5) && inRange(mouseY, 0.56, 0.64))
    {
        sCount = std::round((1 + mouseX - 0.5) * 15);
        lastMouse = sCount;
        //std::cout << sCount << std::endl;

    }else
    {
        sCount = lastMouse;
    }

    return sCount;    

}

//Camera
float lastX = width/2.0f;
float lastY = height/2.0f;
bool firstMouse = true;



void mouse_callback(GLFWwindow* window, double xposIn, double yposln)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposln);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse= false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);   

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



int main()
{
    //Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Brilliant", NULL, NULL);
    if(!window)
    {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;         
    }

    glfwMakeContextCurrent(window);

    //glad stuff
    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout <<"Failed to initialize GLAD" << std::endl;
        return -1;       
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader myShader("/home/magsgl/Desktop/OpenGl/brill/glBrill/src/shaders/shader0.vert", "/home/magsgl/Desktop/OpenGl/brill/glBrill/src/shaders/shader0.frag");
    Shader mybarShader("/home/magsgl/Desktop/OpenGl/brill/glBrill/src/shaders/shader1.vert", "/home/magsgl/Desktop/OpenGl/brill/glBrill/src/shaders/shader1.frag");
    std::vector<float> polygonVertices; 
    std::vector <int> indices;
    

    int sCount;
    int count;

    while(!glfwWindowShouldClose(window))
    { 
        
        processInput(window);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //cloc(window, count);
        sCount = checkMouse(window);

        polygonVertices = genPolygon(0.5, sCount);
        indices = genIndices(sCount);
        
        
        myShader.Use();
        count += 1;

        float aspectRatio = width/height;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), width/ height, 0.1f, 100.0f);
        myShader.setMat4("projectionM", projection);

        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("viewM", view);



        if (count % 720 == 0)
        {
            for(int i = 0; i< 4 ; ++i)
            {
                for(int j=0; j<4; ++j)
                {
                    std::cout<<view[i][j] << " ";
    
                }
                std::cout<<std::endl;
            }

            std::cout << "Camera Zoom (FOV in degrees): " << camera.Zoom << std::endl;
        }

        myShader.setVec3("polColor", glm::vec3(0.5f, 0.1f, 0.2f));
        drawPolygon(polygonVertices, indices);

        mybarShader.Use();
        mybarShader.setVec3("barColor", glm::vec3(0.3f, 0.6f, 0.1f));
        barStuff(window, mybarShader);
        drawBar(barVertices, barIndices);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}

