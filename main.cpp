#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main(int, char**){
    std::cout << "Hello, from  openGL!\n";

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );

    // After we have a context we can Init Glew 
    GLenum err = glewInit();
    if (err != GLEW_OK)
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    else 
        std::cout << "GLVersion: " << glGetString(GL_VERSION) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // draw a triangle using opengl legacy (not the modern opengl!)
        /*
        glBegin(GL_TRIANGLES);
        glVertex2d(-0.5f, -0.5f);
        glVertex2d(0.0f, 0.5f);
        glVertex2d(0.5f, -0.5f);
        glEnd();
        */

        // to use the modern opengl we need:
        // 1. a vertex buffer in the GPU: a bunch of data to be interpreted
        // 2. a shader: a program that runs in the GPU that interprets the 
        // buffer and draw it on the screen.

        // modern opengl is a state machine: basically I have to say hey select 
        // this specific buffer and this specific shader. Based on the selections 
        // that's what determines what kind of triangle is drawn and where..
        


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
