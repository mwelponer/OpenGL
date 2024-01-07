#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders sources
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

/**
 * @brief function to compile the source code for a shader
 * @param type of shader to create
 * @param source of the shader
 * @return unsigned int the id of the shader
 */
static GLuint CompileShader(GLuint type, const std::string& source) {
    GLuint id = glCreateShader(type); 
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // check compile Errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

/**
 * @brief Create a Shader, attach it to a Program and return the program id.
 * @param vertexShader the vertex shader source
 * @param fragmentShader the fragment shader source
 * @return unsigned int the id of the program
 */
static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    GLuint program = glCreateProgram(); 
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); 

    // now we attach the shaders to our program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // clean up 
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context" << std::endl;
    
    // Init GLFW
    if (!glfwInit())
        return -1;

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modern OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving 
    // function pointers and extensions
    glewExperimental = GL_TRUE;
    
    // Initialize GLEW to setup the OpenGL Function pointers
    GLenum err = glewInit();
    if (err != GLEW_OK)
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    else 
        std::cout << "GLVersion: " << glGetString(GL_VERSION) << std::endl;
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);  
    glViewport(0, 0, width, height);

    // Create the shader program from the shader sources
    GLuint shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);


    //////// 1 SIMPLE TRIANGLE

    // write a vertex buffer and vertex array
    // see documentation at https://docs.gl/
    float positions[6] = {
        -0.5f, -0.5f, // each line is a couple (x, y)
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    GLuint VBO, VAO; // ids of the vertex buffer and vertex array 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*)0);
    // explanation: 
    // index: Specifies the index of the generic vertex attribute to be modified.
    // size: Specifies the number of components per generic vertex attribute.
    // Must be 1, 2, 3, 4.
    // type: Specifies the data type of each component in the array.
    // normalized: For glVertexAttribPointer, specifies whether fixed-point data 
    // values should be normalized (GL_TRUE) or converted directly as fixed-point 
    // values (GL_FALSE) when they are accessed.
    // stride: the amount of bytes between each vertex, i.e. the size of each vertex
    // pointer: is a pointer into the actual attribute. It's a pointer inside the 
    // vertex. For example the vertex is made of 3 attributes: position (from byte 0 
    // to byte 12), texture coordinates (from byte 12 to byte 20) and normals 
    // (from byte 20 to byte 32)
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered 
    // VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // Unbind VAO (it's always a good thing to unbind any buffer/array to 
    // prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindVertexArray(0); 


/* 2 triangles
    // Set up vertex data (and buffer(s)) and attribute pointers
    //GLfloat vertices[] = {
    //  // First triangle
    //   0.5f,  0.5f,  // Top Right
    //   0.5f, -0.5f,  // Bottom Right
    //  -0.5f,  0.5f,  // Top Left 
    //  // Second triangle
    //   0.5f, -0.5f,  // Bottom Right
    //  -0.5f, -0.5f,  // Bottom Left
    //  -0.5f,  0.5f   // Top Left
    //}; 
    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left 
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
*/

    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

/* 2 triangles
        // Draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
*/

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

/* 2 triangles
    glDeleteBuffers(1, &EBO);
*/
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
