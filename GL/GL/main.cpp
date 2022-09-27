#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>


int main(void)
{
    GLFWwindow* window;

    // path for the .obj file
    std::string path = "3D/sus.obj";

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes,
        &shapes, &material,
        &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    // gets the vertices and texcoords data
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        int pos_offset = vData.vertex_index * 3;
        int normal_offset = vData.normal_index * 3;
        int uv_offset = vData.texcoord_index * 2;

        fullVertexData.push_back(attributes.vertices[pos_offset]);// X
        fullVertexData.push_back(attributes.vertices[pos_offset + 1]); // Y
        fullVertexData.push_back(attributes.vertices[pos_offset + 2]); // Z

        fullVertexData.push_back(attributes.normals[normal_offset]); // normal
        fullVertexData.push_back(attributes.normals[normal_offset + 1]); // normal
        fullVertexData.push_back(attributes.normals[normal_offset + 2]); // normal

        fullVertexData.push_back(attributes.texcoords[uv_offset]); // U
        fullVertexData.push_back(attributes.texcoords[uv_offset + 1]); // V

    }

    // loads the texture into the .obj
    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load("3D/susTex.jpg", &img_width, &img_height, &colorChannels, 0);

    // vertex shader
    std::fstream verStream("Shader/sample.vert");
    std::stringstream vertStrStream;

    vertStrStream << verStream.rdbuf();
    std::string vertStr = vertStrStream.str();

    const char* vertSrc = vertStr.c_str();

    // fragment shader
    std::fstream fragStream("Shader/sample.frag");
    std::stringstream fragStrStream;

    fragStrStream << fragStream.rdbuf();
    std::string fragStr = fragStrStream.str();

    const char* fragSrc = fragStr.c_str();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float width = 800;
    float height = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Wayne Michael S. Pangilinan", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // initialize texture values
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );

    // mip map
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    // enables depth test
    glEnable(GL_DEPTH_TEST);

    // vertex Shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, NULL);
    glCompileShader(vertShader);

    // fragment Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, NULL);
    glCompileShader(fragShader);

    // shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);


    GLuint VAO, VBO;
 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind Buffer 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Data for the Vertices, Normals, and UVs
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    GLintptr normPtr = 3 * sizeof(GLfloat);
    GLintptr uvPtr = 6 * sizeof(GLfloat);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)normPtr);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)uvPtr);

    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    /* ======================= Initialize Values ======================= */
    glm::mat4 identity = glm::mat4(1.f);

    // Computation for the perspective and view matrix
    glm::mat4 projection = glm::perspective(glm::radians(60.f), height / width, 0.01f, 100.f);

    glm::vec3 cameraPos = glm::vec3(0, 0, 60.f);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    glm::mat4 cameraPosMatrix = glm::translate(identity, cameraPos * -1.f);

    glm::vec3 centerPos = glm::vec3(0, 0, 0);
    glm::vec3 F = glm::normalize(centerPos - cameraPos);
    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
    glm::vec3 U = glm::normalize(glm::cross(R, F));

    glm::mat4 cameraOrientationMat = glm::mat4(glm::vec4(R, 0), glm::vec4(U, 0), glm::vec4((F * -1.f), 0), glm::vec4(glm::vec3(0, 0, 0), 1));

    glm::mat4 view = cameraOrientationMat * cameraPosMatrix;

    // the object transform
    glm::mat4 entity = identity;
    glm::vec3 pos = centerPos - glm::vec3(0, 0, -1);
    entity = glm::translate(identity, pos);
    //entity = glm::scale(entity, glm::vec3(0.5f, .5f, .5f));
    entity = glm::scale(entity, glm::vec3(0.05f, .05f, .05f));

    // light
    glm::vec3 lightPos = glm::vec3(-10, 0, 5);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f;

    float lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Current Time */
        GLfloat currTime = glfwGetTime();

        /* Time that has passed */
        float deltaTime = currTime - lastTime;

        // Rotates the entity/object
        float angle = 20.f * deltaTime;
        entity = glm::rotate(entity, glm::radians(angle), glm::vec3(0, 1, 0));
        entity = glm::translate(entity, glm::vec3(50 * deltaTime, 0, 0));
        


        // Uniforms
        unsigned int translationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(entity));
        
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint tex0Loc = glGetUniformLocation(shaderProgram, "tex0");
        glUniform1i(tex0Loc, 0);
        glBindTexture(GL_TEXTURE_2D, texture);


        // Lighting
        GLuint lightPosAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);
        
        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);
        
        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);
        
        GLuint camPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(camPosAddress, 1, glm::value_ptr(cameraPos));

        glUseProgram(shaderProgram);

        // Bind
        glBindVertexArray(VAO);
        // Draw
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        lastTime = currTime;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}