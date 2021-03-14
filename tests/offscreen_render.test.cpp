
#include <doctest/doctest.h>
#if defined(_WIN32)
#include <windows.h>
#endif

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include "invoke_external.hpp"

void error_callback(int code ,const char* message)
{
    std::cerr << "[ c" << code << " ] " << message << std::endl;
}

TEST_CASE("GLFW can open a hidden window")
{
   glfwSetErrorCallback(&error_callback);
   CHECK_EQ(glfwInit(),GLFW_TRUE);

   glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
   glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow* window = glfwCreateWindow(640,480,"",nullptr,nullptr);

   CHECK_NE(window, nullptr);

   glfwDestroyWindow(window);
   glfwTerminate();

}

TEST_CASE("GLAD can open a context in a hidden window")
{
    glfwSetErrorCallback(&error_callback);
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640,480,"",nullptr,nullptr);

    glfwMakeContextCurrent(window);
    CHECK_EQ(gladLoadGL(),1);

    glfwDestroyWindow(window);
    glfwTerminate();

}



static void create_ppm(const std::string& prefix, int frame_id, unsigned int width, unsigned int height,
                       unsigned int color_max, unsigned int pixel_nbytes, GLubyte *pixels) {
    size_t i, j, cur;
    std::ofstream ofs(prefix+std::to_string(frame_id)+".pmm");

    ofs << "P3\n" << width << " " << height << "\n" << color_max << "\n";

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            cur = pixel_nbytes * ((height - i - 1) * width + j);
            ofs << std::fixed << std::setfill('0') << std::setw(3) << int(pixels[cur])    << " "
                << std::fixed << std::setfill('0') << std::setw(3)<< int(pixels[cur + 1]) << " "
                << std::fixed << std::setfill('0') << std::setw(3)<< int(pixels[cur + 2]) << " ";
        }
        ofs << "\n";
    }

    ofs.close();
}


TEST_CASE("OpenGL can render a triangle")
{
    glfwSetErrorCallback(&error_callback);
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640,480,"",nullptr,nullptr);

    glfwMakeContextCurrent(window);
    CHECK_EQ(gladLoadGL(),1);

    constexpr std::array points = {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f
    };


    // create Vertex Buffer
    GLuint vbo = 0;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float) * points.size(),points.data(),GL_STATIC_DRAW);

    // create Vertex Array & set buffer layout
    GLuint vao = 0;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);

    CHECK_NE(vbo,0);
    CHECK_NE(vao,0);

    const char* vertex_shader =
            "#version 440\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp, 1.0);"
            "}";
    const char* fragment_shader =
            "#version 440\n"
            "out vec4 frag_colour;"
            "void main() {"
            "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
            "}";

    //create, compile & link shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    //dispose linked shaders
    glDeleteShader(fs);
    glDeleteShader(vs);

    CHECK_NE(shader_program,0);

    int generation = 0;

    while(!glfwWindowShouldClose(window)) {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // update other events like input handling
        glfwPollEvents();

        generation++;
        if(generation >= 2)
        {
            std::vector<GLubyte> pixels;
            pixels.resize(640*480*4,255);
            glReadPixels(0, 0, 640, 480, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
            create_ppm("capture", 0, 640, 480, 255, 4, pixels.data());
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

#if WIN32
    int rc = invoke_external(R"(.\venv\Scripts\python tools\img_compare.py capture0.pmm tools\data\sanity.png 90)").value_or(1);

#else
    int rc = invoke_external("./venv/Scripts/python ./tools/img_compare.py capture0.pmm ./tools/data/sanity.png 90").value_or(1);
#endif
    CHECK_EQ(rc,0);


    glfwDestroyWindow(window);
    glfwTerminate();

}