#include "Model3D.h" // From Video Lectures
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


int main(void)
{
    GLFWwindow* window;

    // for the general shader
    std::string path = "3D/creeper.obj";
   
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material1;
    std::string warning, error;
    float clock = 0;
    bool startup = false;
    bool q = true;
    tinyobj::attrib_t attributes1;

    bool success = tinyobj::LoadObj(&attributes1,
        &shapes, &material1,
        &warning, &error, path.c_str());
 

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
    

    // Shader ===================================================
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
    //        ===================================================

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float width = 800;
    float height = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Particle Hw Jardin Pangilinan", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // For linking the shader program
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    
    GLuint VAO, VBO, EBO; // addresses for both

    // Creating obj
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind Buffer 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the data
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(attributes1.vertices) * attributes1.vertices.size(),
        attributes1.vertices.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0

    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),

        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    

    /* ======================= Initialize Values ======================= */
    // Initailize Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(60.f), height / width, 0.f, 100.f);



    GLfloat pitch = 0.f;
    GLfloat yaw = -90.f;


    // Computing for the F, R, U, and View
    glm::vec3 cameraPos = glm::vec3(0, 0, 0);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    glm::vec3 F = glm::vec3(0,0,0);
    F.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    F.y = sin(glm::radians(pitch));
    F.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    F = glm::normalize(F);
    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
    glm::vec3 U = glm::normalize(glm::cross(R, F));

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + F, WorldUp);

    // Initialize Model3D as object
    std::vector <Model3D*> object;
    Model3D *temp = new Model3D();
    temp->initVariables(F * 5.f + cameraPos, glm::vec3(1,1,1), glm::vec3(0, 0, 0));
    temp->init();
    object.push_back(temp);

    // Used for deltaTime computation
    float lastTime = glfwGetTime();
    // Used for Cooldown 
    float lastCDTime = glfwGetTime();

    /* Loop until the user closes the window or user presses the Escape key*/
    while (!glfwWindowShouldClose(window))
    {
        /* Current Time */
        GLfloat currTime = glfwGetTime();
        float cooldownTimer = glfwGetTime();

        /* Time that has passed */
        float deltaTime = currTime - lastTime;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Renders the vector of objects/Model3Ds
        for (int i = 0; i < object.size(); i++) {
            object[i]->render(shaderProgram);
        }   

        // Uniform
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Uses the shader program
        glUseProgram(shaderProgram);

        // Bind
        glBindVertexArray(VAO);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();


        /* Keyboard Input */
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { 
            if (startup == false) {
                startup = true;
            }
           
        }
        else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            clock = 0.0f;
            temp->initVariables(F * 5.f + cameraPos, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
            startup = false;
            q = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { 
            
        }

        if (startup == false) {
            temp->integrate(1.f);
            startup = true;
        std::cout << "This is being called\n";
        }

        //if (startup == true && q == true) {
        //    //clock += 0.000001f;
        //temp->integrate(0.001f);
        //startup = false;
        //q == false;

        //}
        //lastTime = currTime;
    }

    // Delete Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Delete the buffer in each object
    for (int i = 0; i < object.size(); i++) {
        object[i]->deleteVertex();
    }

    glfwTerminate();
    return 0;
}