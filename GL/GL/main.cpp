/*
    Made by Jardin and Pangilinan
*/

#include "Particle.h" 
#include "Firework.h" 
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


int main(void)
{
    GLFWwindow* window;

    // for the general shader
    std::string path = "3D/box.obj";

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material1;
    std::string warning, error;
    float clock = 0;
    bool startup = false;
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

    // window size
    float width = 800;
    float height = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Particle Hw Jardin - Pangilinan", NULL, NULL);
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


    // Computing for the F, R, U, and View
    glm::vec3 cameraPos = glm::vec3(-2 , 0, -10.f);

    glm::mat4 cameraPositionMatrix = glm::translate(glm::mat4(1.f), cameraPos * -1.f);
    cameraPositionMatrix = glm::rotate(cameraPositionMatrix, 100.0f, glm::vec3(0,1,0));
    
    glm::vec3 WorldUp = glm::vec3(0, 1.f, 0);
    glm::vec3 center = glm::vec3(0, 3.f, 0);

    // front vector for the camera
    glm::vec3 F = glm::vec3(center - cameraPos);

    F = glm::normalize(F);
   
    // right vector
    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
    // up vector 
    glm::vec3 U = glm::normalize(glm::cross(R, F));

    glm::mat4 cameraOrientation = glm::mat4(
        glm::vec4(R, 0),
        glm::vec4(U, 0),
        glm::vec4((F * -1.f), 0),
        glm::vec4(glm::vec3(0, 0, 0), 1)
    );
    glm::mat4 view = cameraOrientation * cameraPositionMatrix;

    // Initialize Particle as object
    std::vector<Particle*> particleList1; // pistol 
    std::vector<Particle*> particleList2; // artillery
    std::vector<Particle*> particleList3; // fireball
    std::vector<Particle*> particleList4; // laser
    std::vector<Firework*> particleList5; // firework


    // Used for deltaTime computation
    float lastTime = glfwGetTime();
    // Used for Cooldown 
    float lastCDTime = glfwGetTime();
    float totalDuration = 0;

    enum type {
        PISTOL = 0,
        ARTILLERY,
        FIREBALL,
        LASER,
        FIREWORK
    };

    type projectileType = PISTOL;

    /* Loop until the user closes the window or user presses the Escape key*/
    while (!glfwWindowShouldClose(window))
    {
        /* Current Time */
        GLfloat currTime = glfwGetTime();

        /* Time that has passed */
        float deltaTime = currTime - lastTime;
        float cooldownTimer = glfwGetTime(); // used for cooldown
        totalDuration += deltaTime;

        /* Keyboard Input */
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { // swaps to PISTOL
            projectileType = PISTOL;
            std::cout << "Currently set to Pistol Ammo\n";

        }
        else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) { // swaps to ARTILLERY
            projectileType = ARTILLERY;
            std::cout << "Currently set to Artillery Ammo\n";

        }
        else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) { // swaps to FIREBALL
            projectileType = FIREBALL;
            std::cout << "Currently set to Fireball Ammo\n";

        }
        else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) { // swaps to LASER
            projectileType = LASER;
            std::cout << "Currently set to Laser Ammo\n";

        }
        else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) { // swaps to FIREWORK
            projectileType = FIREWORK;
            std::cout << "Currently set to Firework Ammo\n";

        }
        if (cooldownTimer > lastCDTime + 1) { // cooldown, every 1 second
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { // on mouse button
                lastCDTime = glfwGetTime();
                Particle* temp2 = new Particle();
                temp2->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), projectileType); // instantiates a particle depenting on projectileType
                temp2->init();

                if (projectileType == PISTOL) { // adds the particle to pistol list
                    particleList1.push_back(temp2);
                }
                else if (projectileType == ARTILLERY) { // adds the particle to artillery list
                    particleList2.push_back(temp2);
                }
                else if (projectileType == FIREBALL) { // adds the particle to fireball list
                    particleList3.push_back(temp2);
                }
                else if (projectileType == LASER) { // adds the particle to laser list
                    particleList4.push_back(temp2);
                }
                else if (projectileType == FIREWORK) { // adds the particle to firework list
                    Firework* tempF = new Firework(rand() % 5 + 1);
                    tempF->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), FIREBALL);
                    tempF->init();
                    particleList5.push_back(tempF);
                }
            }
        }
        

        // integrates all of the particle in their lists
        for (int i = 0; i < particleList1.size(); i++) {
            particleList1[i]->integrate(deltaTime);
        }
        for (int i = 0; i < particleList2.size(); i++) {
            particleList2[i]->integrate(deltaTime);
        }
        for (int i = 0; i < particleList3.size(); i++) {
            particleList3[i]->integrate(deltaTime);
        }
        for (int i = 0; i < particleList4.size(); i++) {
            particleList4[i]->integrate(deltaTime);
        }
        for (int i = 0; i < particleList5.size(); i++) {
            particleList5[i]->update(deltaTime, &particleList5);
            particleList5[i]->integrate(deltaTime);
        }


        // Updates the Uniforms
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


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Renders the vector of Particles
        for (int i = 0; i < particleList1.size(); i++) {
            particleList1[i]->render(shaderProgram);
        }
        for (int i = 0; i < particleList2.size(); i++) {
            particleList2[i]->render(shaderProgram);
        }
        for (int i = 0; i < particleList3.size(); i++) {
            particleList3[i]->render(shaderProgram);
        }
        for (int i = 0; i < particleList4.size(); i++) {
            particleList4[i]->render(shaderProgram);
        }
        for (int i = 0; i < particleList5.size(); i++) {
            particleList5[i]->render(shaderProgram);
        }

        lastTime = currTime;
    }

    // Delete Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Delete Buffers for the lists
    for (int i = 0; i < particleList1.size(); i++) {
        particleList1[i]->deleteVertex();
    }
    for (int i = 0; i < particleList2.size(); i++) {
        particleList2[i]->deleteVertex();
    }
    for (int i = 0; i < particleList3.size(); i++) {
        particleList3[i]->deleteVertex();
    }
    for (int i = 0; i < particleList4.size(); i++) {
        particleList4[i]->deleteVertex();
    }
    for (int i = 0; i < particleList5.size(); i++) {
        particleList5[i]->deleteVertex();
    }


    glfwTerminate();
    return 0;
}