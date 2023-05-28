#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>

float speed = 0.05f;

float x_mod = 0.f;
float y_mod = 0.f;

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
bool increasingScale = false;
bool decreassingScale = false;

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

    float move_speed = 0.02f;

    // Press Key
    if (key == GLFW_KEY_D && action == GLFW_PRESS) 
        movingRight = true;

    if (key == GLFW_KEY_A && action == GLFW_PRESS) 
        movingLeft = true;

    if (key == GLFW_KEY_W && action == GLFW_PRESS) 
        movingUp = true;
    
    if (key == GLFW_KEY_S && action == GLFW_PRESS) 
        movingDown = true;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        rotatingLeft = true;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        rotatingLeft = true;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        rotatingRight = true;


    // Release Key
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        movingRight = false;

    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        movingLeft = false;

    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        movingUp = false;

    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        movingDown = false; 

    // Update
    if (movingRight)
        x_mod += speed;
    if (movingLeft)
        x_mod -= speed;
    if (movingDown)
        y_mod -= speed;
    if (movingUp)
        y_mod += speed;

}


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float height = 600.f;
    float width = 600.f;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(height, width, "Zachary Que", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //// size of viewport (i.e for splitscreen)
    //glViewport(0, // Min X
    //           0, // Min Y
    //           300, // Width
    //           600); // Height

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

    GLfloat vertices[] {
        //  x      y      z 
            0.f,   0.5f,  0.f, //0
            -0.5f, -0.5f, 0.f, //1
            0.5f, -0.5f,  0.f  //2

    };

    GLuint indices[]{
        0, 1, 2
    };

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
    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    /* PROJECTION MATRIX */
    // Orthographic
    //glm::mat4 projection = glm::ortho(-2.f, // left most point
    //                                  2.f,  // right most point
    //                                  -2.f, // bottom most point
    //                                  2.f,  // top most point
    //                                  -1.f, // z near
    //                                  1.f); // z far
    // Perspective
    glm::mat4 projection = glm::perspective(
        glm::radians(60.f), // Field of View
        height / width, // aspect Ratio
        0.1f,           // near
        100.f);         // far


    ///* TRANSLATION MATRIX */
    //glm::mat4 translation =
    //    glm::translate(identity_matrix4, // usually start with identity matrix
    //        glm::vec3(x, // translate x by
    //                  y, // translate y by
    //                  z) // translate z by
    //        );

    ///* SCALE MATRIX */
    //glm::mat4 scale =
    //    glm::scale(identity_matrix4, // usually start with identity matrix
    //        glm::vec3(x, // scale x by
    //                  y, // scale y by
    //                  z) // scale z by
    //    );

    ///* ROTATION MATRIX */
    //glm::mat4 rotate =
    //    glm::rotate(identity_matrix4, // usually start with identity matrix
    //        glm::radians(theta), // rotate by theta degrees
    //        glm::vec3(x, // x component of normalized axis vector
    //                  y, // y component of normalized axis vector
    //                  z) // z component of normalized axis vector
    //    );

    float color = 0.f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        color += 0.1f;

        if (color >= 1.f) color = 0.f;

        unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        glUniform1f(xLoc, x_mod);

        unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        glUniform1f(yLoc, y_mod);


        /* LINEAR TRANSFORMATION */
        /* TRANSLATION MATRIX */
        glm::mat4 transformation_matrix =
            glm::translate(identity_matrix4, // usually start with identity matrix
                glm::vec3(x_mod, // translate x by
                          y_mod, // translate y by
                          -5.f) // translate z by
                );

        /* SCALE MATRIX */
        transformation_matrix = 
            glm::scale(transformation_matrix, // usually start with identity matrix
                glm::vec3(2.f, // scale x by
                          2.f, // scale y by
                          2.f) // scale z by
            );

        ///* ROTATION MATRIX */
        //transformation_matrix =
        //    glm::rotate(transformation_matrix, // usually start with identity matrix
        //        glm::radians(theta), // rotate by theta degrees
        //        glm::vec3(0.f, // x component of normalized axis vector
        //                  1.f, // y component of normalized axis vector
        //                  0.f) // z component of normalized axis vector
        //    );

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(transformation_matrix)
        ); 
        
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection)
        );

        unsigned int xColor = glGetUniformLocation(shaderProgram, "color");
        glUniform1f(xColor, color);

        // Draw
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);

        glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
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