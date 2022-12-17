/*
    Made by Jardin and Pangilinan
*/

#include "Particle.h" 
#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"
#include "ParticleForceRegistry.h"
#include "ParticleGravity.h"
#include "ParticleWorld.h"
#include "MassAggregateCube.h"
#include "RigidBody.h"
#include "World.h"


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

    // determines if loading the object is successful
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
    window = glfwCreateWindow(width, height, "Physics Engine Phase 3: Jardin - Pangilinan", NULL, NULL);
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
    
    glm::vec3 WorldUp = glm::vec3(0, 1.f, 0);
    glm::vec3 center = glm::vec3(0, 3.f, 0);

    // front vector for the camera
    glm::vec3 F = glm::vec3(center - cameraPos);

    F = glm::normalize(F);
   
    // right vector
    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
    // up vector 
    glm::vec3 U = glm::normalize(glm::cross(R, F));
    // camera orientation
    glm::mat4 cameraOrientation = glm::mat4(
        glm::vec4(R, 0),
        glm::vec4(U, 0),
        glm::vec4((F * -1.f), 0),
        glm::vec4(glm::vec3(0, 0, 0), 1)
    );
    // view matrix for camera
    glm::mat4 view = cameraOrientation * cameraPositionMatrix;
    // Used for deltaTime computation
    float lastTime = glfwGetTime();
    // Used for Cooldown 
    float lastCDTime = glfwGetTime();
    float totalDuration = 0;


    /* ======================= Force Values ======================= */
    ParticleGravity* pg = new ParticleGravity(glm::vec3(0,-20,0)); // Gravity
    const static unsigned maxContacts = 256; // number of maximum possible contacts
    ParticleWorld world(maxContacts); // create a particle world

    Quaternion qIdentity(0, 0, 0, 1);
    World rbworld;
    RigidBody* rb =  new RigidBody(glm::vec3(0, 0, 7), qIdentity);
    rbworld.bodies.push_back(rb);

    /* Loop until the user closes the window or user presses the Escape key*/
    while (!glfwWindowShouldClose(window))
    {
        /* Start of the physics frame */
        world.startFrame();
        rbworld.startFrame();
        /* Current Time */
        GLfloat currTime = glfwGetTime();

        /* Time that has passed */
        float deltaTime = currTime - lastTime;
        float cooldownTimer = glfwGetTime(); // used for cooldown
        totalDuration += deltaTime;

        /* Input */
        if (cooldownTimer > lastCDTime + 1) { // cooldown, every 1 second
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { // on mouse button
                lastCDTime = glfwGetTime();        
                
                // Spawns a projectile
                Particle* temp2 = new Particle();
                temp2->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 1); // instantiates a particle depenting on projectileType
                temp2->init(); // initializes the particle for rendering

                // add the particle to the world
                world.particles.push_back(temp2);
                world.registry.add(temp2, pg);
            }
        }
        
        // Updates the Uniforms for the camera
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Uses the shader program
        glUseProgram(shaderProgram);

        // Bind VAO
        glBindVertexArray(VAO);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        // clears the frame
        glClear(GL_COLOR_BUFFER_BIT);

        // Renders the objects
        rbworld.render(shaderProgram);
        world.render(shaderProgram);

        // Runs physics
        rbworld.runPhysics(deltaTime, world.particles);
        world.runPhysics(deltaTime);

        lastTime = currTime;
    }

    // Delete Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}