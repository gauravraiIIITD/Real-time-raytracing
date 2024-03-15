#include "utils.h"

int width = 1800, height=1000;
#define DIVISIONS 100
#define PI 3.14

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::cout <<  glGetString(GL_VENDOR) << '\n';
    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    
    int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    if(vVertex_attrib == -1) {
        std::cout << "Could not bind location: vVertex\n" ;
        exit(0);
    }else{
        std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    }

    float vertices[] = {
       1.0f,  1.0f, 0.0f,  // top right
       1.0f, -1.0f, 0.0f,  // bottom right
      -1.0f, -1.0f, 0.0f,  // bottom left
      -1.0f,  1.0f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Camera
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;

    float speed = 3.0f; // 3 is optimal. Any faster and it gets messy
    float mouseSpeed = 0.1f;

    float lastTime = glfwGetTime();
    
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glfwSetCursorPos(window, double(display_w)/2, double(display_h)/2);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glUseProgram(shaderProgram);

        glViewport(0, 0, display_w, display_h);
        glUseProgram(shaderProgram);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        horizontalAngle += mouseSpeed * deltaTime * float(double(display_w)/2 - xpos );
        verticalAngle   += mouseSpeed * deltaTime * float(double(display_h)/2 - ypos );
        
        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );

        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );
        glm::vec3 up = glm::cross( right, direction );
        if (glfwGetKey( window,GLFW_KEY_UP ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window,GLFW_KEY_DOWN ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey( window,GLFW_KEY_RIGHT ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey( window,GLFW_KEY_LEFT ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }

        GLuint u_camPositions = glGetUniformLocation(shaderProgram, "cameraPos");
        GLuint u_camTarget = glGetUniformLocation(shaderProgram, "camera_target");
        if (u_camPositions == -1) {
            fprintf(stderr, "Could not bind location: camPosition\n");
            exit(0);
        }
        if (u_camTarget == -1) {
            fprintf(stderr, "Could not bind location: camTarget\n");
            exit(0);
        }

        glm::vec3 camPos = position;
        glUniform3fv(u_camPositions, 1, glm::value_ptr(camPos));

        glm::vec3 camera_target =  position+direction;
        glUniform3fv(u_camTarget, 1, glm::value_ptr(camera_target));

        glBindVertexArray(VAO);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSetCursorPos(window, double(display_w)/2.0, double(display_h)/2.0);
        lastTime = currentTime;
        glfwSwapBuffers(window);
    }
    cleanup(window);
    return 0;
}
