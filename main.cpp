#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <iostream>


/* Global variables */
// WASD Movement variables
float x_mod = 0.f;
float y_mod = 0.f;
// Rotating variables
float x_axis_rotate_mod = 0.f;
float y_axis_rotate_mod = 0.f;
// Scale variables
float scale_mod = 0.125f;
// Zoom variables
float z_mod = -5.f;

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
        y_axis_rotate_mod -= speed * 30;
    if (rotatingLeft) // Turn left (CCW)
        y_axis_rotate_mod += speed * 30;
    if (rotatingUp) // Tilt nose up
        x_axis_rotate_mod -= speed * 30;
    if (rotatingDown) // Tilt nose down
        x_axis_rotate_mod += speed * 30;

    // Q/E
    //if (decreasingScale)
    //    scale_mod -= speed;
    //if (increasingScale)
    //    scale_mod += speed;

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

    /* User Input */
    glfwSetKeyCallback(window, Key_Callback);

    glViewport(0, 0, (int)width, (int)height);

    /* Variablles for our texture */
    int img_width, img_height, colorChannels;

    // Fix the flipped texture (by default it is flipped)
    stbi_set_flip_vertically_on_load(true);

    /* Load Texture */
    // Texture retrieved from: https://wallpapers.com/wallpapers/hippie-aesthetic-b8apui9pm9orwz20.html
    unsigned char* tex_bytes =
        stbi_load("3D/texture.jpg", // texture path
            &img_width, // fills out the width
            &img_height, // fills out the height
            &colorChannels, //fills out the color channel
            0);

    /* Setup Texture */
    // Open Gl reference to texture
    GLuint texture;
    // Generate a reference
    glGenTextures(1, &texture);
    // Set the current texture we're working on to Texture 0
    glActiveTexture(GL_TEXTURE);
    // Bind our next tasks to Tex0
    // To our current reference similar to what we're doing to VBOs
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Assign Texture to Reference */
    // Assign the loaded texture to the OpenGL reference
    glTexImage2D(
        GL_TEXTURE_2D,
        0, // Texture 0
        GL_RGB, // Target color format of the texture
        img_width, // Texture width
        img_height, // Texture height
        0,
        GL_RGB, // Color format of the textyre
        GL_UNSIGNED_BYTE,
        tex_bytes // Loaded textures in bytes
    );

    /* Clean up texture (free memory) */
    // Generate the mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free up the loaded bytes
    stbi_image_free(tex_bytes);

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    /* Load the vertex shader file into a string stream */
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    // Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();
    // Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    /* Load the fragment shader file into a string stream */
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    // Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();
    // Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    /* Create a vertex shader */
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    // Compile the vertex shader
    glCompileShader(vertexShader);

    /* Create a fragment shader */
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Assign the source to the fragment shader
    glShaderSource(fragShader, 1, &f, NULL);
    // Compile the fragment shader
    glCompileShader(fragShader);

    /* Create the shader program */
    GLuint shaderProgram = glCreateProgram();
    // Attach the compiled vertex shader
    glAttachShader(shaderProgram, vertexShader);
    // Attach the compiled fragment shader
    glAttachShader(shaderProgram, fragShader);
    // Finalize the compilation of the shaders
    glLinkProgram(shaderProgram);

    /* Apply the shaders */
    glUseProgram(shaderProgram);

    /* Initialize variables for tiny obj loader */
    // Relative path to the mesh
    std::string path = "3D/djSword.obj";
    // Will contain the mesh's shapes
    std::vector<tinyobj::shape_t> shapes;
    // Will contain the mesh's material
    std::vector<tinyobj::material_t> materials;
    // Some error messages might popup
    std::string warning, error;
    // Basic attributes related to the mesh
    tinyobj::attrib_t attributes;

    /* Load the obj */
    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, path.c_str());

    /* Initialize the Array of Vertex Data */
    std::vector<GLfloat> fullVertexData;
    // Loop thorugh all thee vertex indices
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        // Assign the index data for easier access
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        // X
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
        // Y
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
        // Z
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

        /* Normals */
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

        // U
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
        // V
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
    }

    /* Create VAO / VBO variables */
    GLuint VAO, VBO;
  
    /* initialize VAO, VBO */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    /* Bind the VAO */
    glBindVertexArray(VAO);

    /* Create an Array Buffer to store vertex positions */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /* Add the size of our Vertex Array in bytes and the contents to the buffer */
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );

    /* Position Data */
    glVertexAttribPointer(
        0, // Index 0 is vertex position
        3, // X Y Z 
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),
        (void*)0
    );

    /* Normal Data */
    GLint normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1, // Index 1 for normals
        3, // Normals has 3 values
        GL_FLOAT, // Data type of normals
        GL_FALSE,
        8 * sizeof(GL_FLOAT),
        (void*)normalPtr
    );

    /* UV Data */
    GLintptr uvptr = 6 * sizeof(float);
    glVertexAttribPointer(
        2, // Index 2 for tex coordinates / UV
        2, // UV is 2 floats (U,V)
        GL_FLOAT, // Data type of array
        GL_FALSE,
        8 * sizeof(GL_FLOAT), 
        (void*)uvptr // Add in the offset
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* IDENTITY MATRIX */
    glm::mat4 identity = glm::mat4(1.0f);

    /* PROJECTION MATRIX */
    // Perspective
    glm::mat4 projection = glm::perspective(
        glm::radians(60.f), // Field of View
        height / width, // aspect Ratio
        0.1f,           // near
        100.f);         // far

    /* Camera Variables */
    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 10.f);
    glm::vec3 worldUp = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 cameraCenter = glm::vec3(0.f, 0.f, -1.f);

    /* Light variables */
    glm::vec3 lightPos = glm::vec3(-10, 3, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    /* Ambient variables */
    // Ambient strength
    float ambientStr = 0.1f;
    // Ambient Color
    glm::vec3 ambientColor = lightColor;

    /* Specular variables */
    // Spec strength
    float specStr = 0.5f;
    // Spec phong
    float specPhong = 16;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the depth buffer as well

        /* VIEW MATRIX */
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);

        /* TRANSFORMATION MATRIX */
        // Translate
        glm::mat4 transform = glm::translate(identity, glm::vec3(x_mod, y_mod, z_mod));
        // Scale
        transform = glm::scale(transform, glm::vec3(scale_mod));
        // Rotate
        transform = glm::rotate(transform, glm::radians(x_axis_rotate_mod), glm::vec3(1.f, 0.0f, 0.f));
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

        /* Lighting variables */
        // Light pos address
        GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
        // Light Color 
        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        /* Ambient variables */
        // Ambient str address
        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);
        // Light Color 
        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        /* Specular variables */
        // Get the address of the camera pos from the shader
        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
        // Get the address of the spec str from the shader
        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);
        // Get the address of the spec phong from the shader
        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        // Get the location of the tex 0 in the fragment shader
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        // Tell OpenGl to use the texture
        glBindTexture(GL_TEXTURE_2D, texture);
        // Use the texture at index 0
        glUniform1i(tex0Address, 0);

        /* Draw */
        glBindVertexArray(VAO);
        // Apply Shaders
        glUseProgram(shaderProgram);
        
        // Draw using the vertex array
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}