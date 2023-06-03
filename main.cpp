#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>


// WASD Movement variables
float x_mod = 0.f;
float y_mod = 0.f;

// Rotating variables
float x_axis_rotate_mod = 0.f;
float y_axis_rotate_mod = 0.f;

// Scale variables
float scale_mod = 10.f;

// Zoom variables
float z_mod = -0.5f;

// moving flags
bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = false;

// rotating flags
bool rotatingLeft = false;
bool rotatingRight = false;
bool rotatingUp = false;
bool rotatingDown = false;

// scaling flags
bool decreasingScale = false;
bool increasingScale = false;

// zooming flags
bool zoomingIn = false;
bool zoomingOut = false;

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
) {

    float speed = 0.05f;

    /* Press Key */
    // WASD
    if (key == GLFW_KEY_D && action == GLFW_PRESS) 
        movingRight = true;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) 
        movingLeft = true;
    if (key == GLFW_KEY_W && action == GLFW_PRESS) 
        movingUp = true;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) 
        movingDown = true;

    // Arroy Keys
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        rotatingRight = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        rotatingLeft = true;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        rotatingUp = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        rotatingDown = true;

    // Q/E
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        decreasingScale = true;
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        increasingScale = true;

    // Z/X
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        zoomingIn = true;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        zoomingOut = true;

    /* Release Key */
    // WASD
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        movingRight = false;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        movingLeft = false;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        movingUp = false;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        movingDown = false;

    // Arroy Keys
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        rotatingRight = false;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        rotatingLeft = false;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        rotatingUp = false;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        rotatingDown = false;

    // Q/E
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        decreasingScale = false;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE)
        increasingScale = false;

    // Z/X
    if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
        zoomingIn = false;
    if (key == GLFW_KEY_X && action == GLFW_RELEASE)
        zoomingOut = false;

    /* Update */
    // WASD
    if (movingRight)
        x_mod += speed;
    if (movingLeft)
        x_mod -= speed;
    if (movingDown)
        y_mod -= speed;
    if (movingUp)
        y_mod += speed;

    // Arrow Keys
    if (rotatingRight) // Turn right (CW)
        y_axis_rotate_mod -= speed * 15;
    if (rotatingLeft) // Turn left (CCW)
        y_axis_rotate_mod += speed * 15;
    if (rotatingUp) // Tilt nose up
        x_axis_rotate_mod -= speed * 15;
    if (rotatingDown) // Tilt nose down
        x_axis_rotate_mod += speed * 15;

    // Q/E
    if (decreasingScale)
        scale_mod -= speed;
    if (increasingScale)
        scale_mod += speed;
   
    // Z/X
    if (zoomingIn)
        z_mod += speed;
    if (zoomingOut)
        z_mod -= speed;
}


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float width = 600.f;
    float height = 600.f;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((int)width, (int)height, "Zachary Que", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, (int)width, (int)height);

    // User input
    glfwSetKeyCallback(window, Key_Callback);

    // Vertex shader
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Fragment shader
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    //GLfloat vertices[] {
    //    //  x      y      z 
    //        0.f,   0.5f,  0.f, //0
    //        -0.5f, -0.5f, 0.f, //1
    //        0.5f, -0.5f,  0.f  //2
    //};

    //GLuint indices[]{
    //    0, 1, 2
    //};

    GLuint VAO, VBO, EBO;

    // initialize VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        //attributes.vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, // X Y Z 
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* IDENTITY MATRIX */
    glm::mat4 identity = glm::mat4(1.0f);

    /* PROJECTION MATRIX */
    // Perspective
    glm::mat4 projection = glm::perspective(
        glm::radians(60.f), // Field of View
        height / width, // aspect Ratio
        0.1f,           // near
        100.f);         // far

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* CAMERA MATRIX */
        glm::vec3 cameraPos = glm::vec3(x_mod, y_mod, 10.f);
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.f), cameraPos * -1.f);

        /* ORIENTATION MATRIX */
        glm::vec3 worldUp = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
        glm::vec3 cameraCenter = glm::vec3(0.f, 3.f, 0.f);
        //// Forward Vector
        //glm::vec3 F = (cameraCenter - cameraPos);
        //F = glm::normalize(F);
        //// Right Vector
        //glm::vec3 R = glm::normalize(glm::cross(F, worldUp));
        //// Up Vector
        //glm::vec3 U = glm::normalize(glm::cross(R, F));
        //// Camera Orientation
        //glm::mat4 cameraOrientation = glm::mat4(1.f);
        //// Manually assign the matrix, Matrix[Col][Row]
        //// Row 1
        //cameraOrientation[0][0] = R.x;
        //cameraOrientation[1][0] = R.y;
        //cameraOrientation[2][0] = R.z;
        //// Row 2
        //cameraOrientation[0][1] = U.x;
        //cameraOrientation[1][1] = U.y;
        //cameraOrientation[2][1] = U.z;
        //// Row 3
        //cameraOrientation[0][2] = -F.x;
        //cameraOrientation[1][2] = -F.y;
        //cameraOrientation[2][2] = -F.z;
        //
        ///* CAMERA VIEW MATRIX */
        //glm::mat4 viewMatrix = cameraOrientation * cameraPosMatrix;
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);

        /* TRANSFORMATION MATRIX */
        // Translate
        glm::mat4 transform = glm::translate(identity, glm::vec3(x_mod, y_mod, z_mod));
        // Scale
        transform = glm::scale(transform, glm::vec3(scale_mod, scale_mod, scale_mod));
        // Rotate x
        transform = glm::rotate(transform, glm::radians(x_axis_rotate_mod), glm::vec3(1.0f, 0.f, 0.f));
        // Rotate y
        transform = glm::rotate(transform, glm::radians(y_axis_rotate_mod), glm::vec3(0.f, 1.0f, 0.f));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
                               glUniformMatrix4fv(viewLoc,
                                   1,
                                   GL_FALSE,
                                   glm::value_ptr(viewMatrix)
        );

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(transform)
        ); 
        
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection)
        );

        // Draw
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);

        glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            (void*)0
        );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}