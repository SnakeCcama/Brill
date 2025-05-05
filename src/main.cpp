#include <iostream>
#include <filesystem>

#include "includes/gl.h"
#include "includes/gl.c"

#include "includes/sheader.hpp"
#include "includes/cam.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

float width = 1000.0, height = 800.0;
Camera camera(0.0, 0.0, 3.0, 0.0, 1.0, 0.0, -90.0f, 0.0f);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::vector<float>* vecPtr = nullptr;

std::vector<float> cubeVertices = {
    
     // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f

};

std::vector<int> cubeIndices = {
    
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

std::vector<float> genPolygon(float radius, int noSectors, int noStacks)
{
    std::vector<float> vertices;
   
    float sectorStepSize=  2.0f * M_PI/noSectors;
    float stackStepSize = M_PI/ noStacks;
    float sectorAngle, stackAngle;

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s,t;
    
    
    /*
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);*/

    for (int i=0; i<=noStacks ; ++i)
    {
        stackAngle = M_PI/2 - stackStepSize * i;
        z= radius * sinf(stackAngle);
        xy= radius * cosf(stackAngle);

        for (int j=0; j<=noSectors; ++j)
        {
            sectorAngle= j * sectorStepSize;
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz); 

            s = (float)j/noSectors;
            t= (float)i/noStacks;
            vertices.push_back(s);
            vertices.push_back(t);
            
        }
    }
   
   
    return vertices;
}

std::vector<int> genIndices(int sectorCount, int stackCount)
{
    std::vector<int>indices;
    std::vector<int>lineIndices; 
    int k1, k2;

    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            
            if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
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
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));


    GLuint iboId;
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    //glDeleteBuffers(1, &txVBO);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &iboId);
    glDeleteVertexArrays(1, &vaoId); 


}

void drawCube(std::vector<float>& vertices)
{
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(float), vertices.data(), GL_STATIC_DRAW);

    /*
    GLuint iboId;
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);*/

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);

    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);    
}

void cubeStuff(Shader& aShader, glm::vec3 lightColor, glm::vec3 lightPos)
{
    
    
    aShader.Use();
    aShader.setInt("material.diffuse", 0);
    aShader.setInt("material.specular", 1);
    aShader.setInt("emissionTexture", 2);
    aShader.setVec3("viewPos", camera.Position);
    
    float aspectRatio = width/height;
    float time = (float)glfwGetTime() * 50.0;
    
    glm::mat4 model = glm::mat4(1.0f); 
    
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::translate(model, glm::vec3(-1.5f, -0.2f, 0.5f));
    model = glm::rotate(model, glm::radians(time), glm::vec3(1.0, 0.0, 0.0));

    aShader.setMat4("modelM", model);
    
    
    glm::mat4 view = camera.GetViewMatrix();
    aShader.setMat4("viewM", view);
            
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
    aShader.setMat4("projectionM", projection);
    
    
    aShader.setFloat("material.shine", 32.0); 
    aShader.setVec3("light.position", lightPos);
    
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); 
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 
            
    aShader.setVec3("light.ambient", ambientColor);
    aShader.setVec3("light.diffuse", diffuseColor);
    aShader.setVec3("light.specular", glm::vec3 (1.0f)); 


}


void sphereStuff(Shader& myShader, glm::vec3 lightColor)
{

    myShader.Use();
    myShader.setVec3("light.position", glm::vec3(0.75f, 0.75f, 1.0f));
    
    float aspectRatio = width/height;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(float(glfwGetTime() * 50)), glm::vec3(1.0, 0.0, 0.0));
    myShader.setMat4("modelM", model);

    glm::mat4 view = camera.GetViewMatrix();
    myShader.setMat4("viewM", view);
    
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
    myShader.setMat4("projectionM", projection);

    myShader.setVec3("polColor", glm::vec3(1.0f, 0.80f, 0.31f));
    myShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    myShader.setVec3("viewPos", camera.Position);
    
    myShader.setVec3("material.ambient", glm::vec3 (1.0f, 0.5f, 0.31f));
    myShader.setVec3("material.diffuse", glm::vec3 (1.0f, 0.5f, 0.31f));
    myShader.setVec3("material.specular", glm::vec3 (0.5f, 0.5f, 0.5f));
    myShader.setFloat("material.shine", 64.0); 


    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); 
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 

    
    myShader.setVec3("light.ambient", ambientColor);
    myShader.setVec3("light.diffuse", diffuseColor);
    myShader.setVec3("light.specular", glm::vec3 (1.0f));

}



glm::vec3 lightStuff(Shader& aShader, glm::vec3 lightColor)
{
    
    aShader.Use();
    aShader.setVec3("lightColor", lightColor);
    
    float time = (float)glfwGetTime() * 50.0;


    glm::mat4 model = glm:: mat4(1.0f);
    
    model = glm::translate(model, glm::vec3(-1.5f, -0.2f, 0.5f));
    model = glm::rotate(model, glm::radians(time), glm::vec3 (0.0, 1.0, 0.0));
    model = glm::translate(model, glm::vec3(1.5f, 0.0, 0.0));
    
    aShader.setMat4("modelM", model);
    glm::vec3 lightPos = glm::vec3(model[3]);
    


    glm::mat4 view = camera.GetViewMatrix();
    aShader.setMat4("viewM", view);    

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), width/ height, 0.1f, 100.0f);
    aShader.setMat4("projectionM", projection);

    return lightPos;

}



unsigned int loadTexture(const std::string& texturefilePath)
{

    unsigned int texture;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);


    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(std::filesystem::path(texturefilePath).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {

        GLenum format;
        if(nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return texture;

   
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

    
    Shader myShader("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/shaders/shader0.vert", "/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/shaders/shader0.frag");   
    Shader mylightsourceShader("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/shaders/shaderLight.vert", "/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/shaders/shaderLight.frag");
    Shader mycubeShader("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/shaders/shader1.vert", "/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/shaders/shader1.frag");
    
    unsigned int diffuseMap = loadTexture("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/textures/container2.png");
    unsigned int specularMap = loadTexture("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/textures/specular.png");
    unsigned int specolorMap = loadTexture("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/textures/specolor.png");
    unsigned int emmissionMap = loadTexture("/home/magsgl/Desktop/OpenGl/lighting/brilliant/glBrill/src/textures/matrix.jpg");
    
    std::vector<float> polygonVertices; 
    std::vector <int> indices;


    std::vector <float> sphereVertices;
    std::vector <int> sphereIndices; 


    int sCount = 3;
    int count = 0;

    while(!glfwWindowShouldClose(window))
    { 
        
        processInput(window);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        if (sCount >= 19)
        {
            sCount = 3;
        }
        
        //cloc(window, count);
        count += 1;

        if (count % 200 == 0)
        {
            sCount += 1;
        }
        
        sphereVertices = genPolygon(0.15, 15, 15);
        sphereIndices = genIndices(15, 15);
        
        polygonVertices = genPolygon(0.5, sCount, sCount);
        indices = genIndices(sCount, sCount);

        glm::vec3 lightColor = glm::vec3(1.0f);
        
        
    
        glm::vec3 lightPosition = lightStuff(mylightsourceShader, lightColor);
        drawPolygon(sphereVertices, sphereIndices); 

        cubeStuff(mycubeShader, lightColor, lightPosition);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emmissionMap);


        drawCube(cubeVertices);
    
        
        
        /*
        sphereStuff(myShader, lightColor);
        drawPolygon(polygonVertices, indices); */
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}



























