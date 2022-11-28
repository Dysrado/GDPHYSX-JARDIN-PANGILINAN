/*
    Made by Jardin and Pangilinan
*/

#include "Particle.h" 
#include "Firework.h" 
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "ParticleForceRegistry.h"
#include "ParticleGravity.h"
#include "ParticleSpring.h"
#include "ParticleAnchoredSpring.h"
#include "ParticleBungee.h"
#include "ParticleWorld.h"
#include "MassAggregateCube.h"


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
   /* Particle* box = new Particle();
    box->initVariables(glm::vec3(0, 0, 5), glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), 2);
    box->init();*/
    MassAggregateCube* Cube = new MassAggregateCube();


    //std::vector<Particle*> particleList;

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

    /*enum springType {
        NONE = 0,
        BASIC,
        ANCHORED,
        BUNGEE
    };*/

    type projectileType = ARTILLERY;
    //springType spring = NONE;

    /* ======================= Force Values ======================= */
    //ParticleForceRegistry springReg;
    ParticleGravity* pg = new ParticleGravity(glm::vec3(0,-20,0));

    const static unsigned maxContacts = 256;
    ParticleWorld world(maxContacts);
    Cube->pushToWorld(world);

    //world.push_back(box);
    /* Loop until the user closes the window or user presses the Escape key*/
    while (!glfwWindowShouldClose(window))
    {
        world.startFrame();
        /* Current Time */
        GLfloat currTime = glfwGetTime();

        /* Time that has passed */
        float deltaTime = currTime - lastTime;
        float cooldownTimer = glfwGetTime(); // used for cooldown
        totalDuration += deltaTime;


        /* Keyboard Input */
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) { // swaps to ARTILLERY
            projectileType = ARTILLERY;
            //spring = NONE;
            //std::cout << "Currently set to Artillery Ammo\n";

        }
        //else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) { // swaps to Basic Spring
        //    projectileType = ARTILLERY;
        //    spring = BASIC;
        //    std::cout << "Currently set to Basic Spring\n";
        //}
        //else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) { // swaps to Anchored Spring
        //    projectileType = ARTILLERY;
        //    spring = ANCHORED;
        //    std::cout << "Currently set to Anchored Spring\n";
        //}
        //else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) { // swaps to Bungee Spring
        //    projectileType = ARTILLERY;
        //    spring = BUNGEE;
        //    std::cout << "Currently set to Bungee Spring\n";
        //}

        if (cooldownTimer > lastCDTime + 1) { // cooldown, every 1 second
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { // on mouse button
                lastCDTime = glfwGetTime();
  /*              Particle* a = new Particle();
                Particle* b = new Particle();
                b->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), projectileType);
                b->init();*/
                

                if (projectileType == ARTILLERY /* && spring == NONE*/) { // adds the particle artillery
                    Particle* temp2 = new Particle();
                    temp2->initVariables(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), projectileType); // instantiates a particle depenting on projectileType
                    temp2->init();
                    
                    world.push_back(temp2);
                    // Edited =============================================================
                    /*while (world.firstParticle->next != NULL) {
                        world.firstParticle = world.firstParticle->next;
                    }*/
                    world.registry.add(temp2, pg);
                    //world.registry.add(world.firstParticle->particle, pg);
                }

                //if (spring == BASIC) { // adds the particle Basic Spring
                //    a->initVariables(glm::vec3(0, -5, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), projectileType);
                //    a->init();
                //    
                //    ParticleSpring* ps = new ParticleSpring(b, 1.f, 2.f);
                //    springReg.add(a, ps);
                //    particleList.push_back(a);
                //    particleList.push_back(b);
                //}
                //else if(spring == ANCHORED) // adds the particle Anchored Spring
                //{
                //    a->initVariables(glm::vec3(0, -3, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), projectileType);
                //    a->init();

                //    ParticleAnchoredSpring* pAs = new ParticleAnchoredSpring(&cameraPos, 1.f, 2.f);
                //    springReg.add(a, pAs);
                //    particleList.push_back(a);
                //}
                //else if (spring == BUNGEE) { // adds the particle Bungee Spring
                //    a->initVariables(glm::vec3(0, -5, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), projectileType);
                //    a->init();

                //    ParticleBungee* ps = new ParticleBungee(b, 1.f, 2.f);
                //    springReg.add(a, ps);
                //    particleList.push_back(a);
                //    particleList.push_back(b);
                //}
                
            }
        }
        
        //springReg.updateForces(deltaTime);

        //// integrates all of the particle in their lists
        //for (int i = 0; i < particleList.size(); i++) {
        //    particleList[i]->integrate(deltaTime);
        //}

        //box->integrate(deltaTime);

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

        //// Renders the vector of Particles
        //for (int i = 0; i < particleList.size(); i++) {
        //    particleList[i]->render(shaderProgram);
        //}

        /*while (world.firstParticle != NULL) {
            
            world.firstParticle = world.firstParticle->next;
        }*/
        world.render(shaderProgram);


        //box->render(shaderProgram);

        world.runPhysics(deltaTime);
        lastTime = currTime;
    }

    // Delete Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //// Delete Buffers for the lists
    //for (int i = 0; i < particleList.size(); i++) {
    //    particleList[i]->deleteVertex();
    //}

    //box->deleteVertex();

    glfwTerminate();
    return 0;
}