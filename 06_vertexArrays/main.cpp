#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))



/**
 * @brief used to clear all errors
 * 
 */
static void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

/**
 * @brief used to print all errors
 * 
 */
static bool GLLogCall(const char* function, const char* file, int line){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGl Error] (" << error << "): " << function << 
        " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource {
    std::string VertexShader;
    std::string FragmentShader;
};

/**
 * @brief parse the shader file that contains both vertex and fragment 
 * shader
 * 
 * @param filePath of the shader file 
 * @return ShaderProgramSource containing vertex and fragments code strings
 */
static ShaderProgramSource parseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream ss[2];

    // Check if the file is open, which indicates that it exists
    if (stream.is_open()) {
        std::cout << "File exists." << std::endl;
        
        std::string line;
        ShaderType type = ShaderType::NONE;

        while(getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    type = ShaderType::VERTEX;
                } else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            } else {
                ss[(int)type] << line << "\n";
            }
        }

        // Close the file after using it
        stream.close();
    } else {
        std::cout << "File does not exist." << std::endl;
    }

    return {ss[0].str(), ss[1].str()};
}

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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

    glfwSwapInterval(1);

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



    //////// 2 TRIANGLES

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

    GLuint VBO, VAO, IBO;
    GLCall( glGenVertexArrays(1, &VAO) ); // generate 1 vertex array 
    GLCall( glGenBuffers(1, &VBO) ); // generate 1 vertex buffer
    GLCall( glGenBuffers(1, &IBO) ); // generate 1 index buffer

    // Bind the Vertex Array Object first
    GLCall( glBindVertexArray(VAO) );

    // Select vertex buffer
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, VBO) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW) );
    // bind index 0 of vertex array with the currently bound GL_ARRAY_BUFFER, i.e. VBO
    GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0) );
    GLCall( glEnableVertexAttribArray(0) );

    // Select index buffer
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO) );
    GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW) );

    // Unbind
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) ); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    GLCall( glBindVertexArray(0) ); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    ShaderProgramSource source = parseShader("../res/shaders/Basic.shader");
    // // Create the shader program from the shader sources
    GLuint shaderProgram = CreateShader(source.VertexShader, source.FragmentShader);
    GLCall( glUseProgram(shaderProgram) );

    // I retrieve the location of the color variable
    GLCall( int location = glGetUniformLocation(shaderProgram, "u_Color") );
    ASSERT(location != -1);
    // once I have the location I set my data in my shader
    GLCall( glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f) );


    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float r = 0.0f;
    float increment = 0.05f;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        GLCall( glfwPollEvents() );

        // Clear the colorbuffer
        GLCall( glClearColor(0.1f, 0.1f, 0.1f, 1.0f) );
        GLCall( glClear(GL_COLOR_BUFFER_BIT) );



        // 2 TRIENGLES
        GLCall( glBindVertexArray(VAO) );

        // once I have the location I set my data in my shader
        GLCall( glUniform4f(location, r, 0.3f, 0.8f, 1.0f) );
        GLCall( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        GLCall( glBindVertexArray(0) ); // unbind



        // Swap the screen buffers
        GLCall( glfwSwapBuffers(window) );
    }
    // Properly de-allocate all resources once they've outlived their purpose
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    GLCall( glDeleteProgram(shaderProgram) );

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate() ;
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
