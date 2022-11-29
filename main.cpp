/* 
 * Created 2022 Baltazarus.
 *
 * This is one example of 3D cube in OpenGL.
 * Press space to free-rotate or enable auto-rotation of the cube.
 * That space is used as toggle between those two modes.
 * When in free-rotate mode, use arrows or AWSD to rotate the cube :)
 * 
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>

void Callback_FrameBufferSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    return;
}

void LoadShaderFromFile(std::string& ref_str, const char* path)
{
    std::ifstream file(path);
    if(file.is_open())
    {
        file.seekg(0, std::ios::end);
        std::size_t sz = file.tellg();
        file.seekg(std::ios::beg);
        char* buffer = new char[sz];
        file.read(buffer, sz);
        ref_str = buffer;
        delete[] buffer;
        file.close();
    }
    else
        std::cout << "Cannot open shader from file!" << std::endl;

    return;
}

void CheckCompileStatus(unsigned int& shader_id, int shader_type)
{
    int status;
    int len;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);

    if(!status)
    {
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
        char* log_msg = (char*)alloca(len);
        glGetShaderInfoLog(shader_id, len, &len, log_msg);
        std::cout << log_msg << std::endl;
    }

    glGetShaderiv(shader_id, GL_LINK_STATUS, &status);
    if(!status)
    {
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
        char* log_msg = (char*)alloca(len);
        glGetProgramInfoLog(shader_id, len, &len, log_msg);
        std::cout << log_msg << std::endl;
    }
    return;
}

int main()
{
    if(!glfwInit())
    {
        std::cout << "Cannot init GLFW!" << std::endl;
        return -1;
    }

    glfwInitHint(GLFW_VERSION_MINOR, 3);
    glfwInitHint(GLFW_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Test Cube :)", nullptr, nullptr);
    if(!window)
    {
        std::cout << "Cannot create window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    if(glewInit() != GLEW_OK)
    {
        std::cout << "Cannot initialize GLEW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // To save up time, I took those from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_05
    // Credit goes to this kind author :)

    float vertices[] = {
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f 
    };

    unsigned int indices[] = {
	    0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3
    };

    unsigned int vs_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fs_id = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vss_str;
    LoadShaderFromFile(vss_str, "vertex_shader.vert");
    const char* src_ptr = vss_str.c_str();
    glShaderSource(vs_id, 1, &src_ptr, nullptr);

    std::string fss_str;
    LoadShaderFromFile(fss_str, "fragment_shader.frag");
    const char* fsrc_ptr = fss_str.c_str();
    glShaderSource(fs_id, 1, &fsrc_ptr, nullptr);

    glCompileShader(vs_id);
    CheckCompileStatus(vs_id, GL_VERTEX_SHADER);
    glCompileShader(fs_id);
    CheckCompileStatus(fs_id, GL_FRAGMENT_SHADER);

    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);
    glLinkProgram(program_id);

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);

    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int ibo_id;
    glGenBuffers(1, &ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0u);
    glUseProgram(0u);

    while(!glfwWindowShouldClose(window))
    {
        static bool bFree = true;
        static bool bReleased = false;

        static float x = 0.01f, y = 0.01f;
        static float angle_x = x;
        static float angle_y = y;

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if(bReleased)
            {
                if(bFree)
                    bFree = false;
                else
                    bFree = true;
                bReleased = false;
            }
        }
        else
            bReleased = true;
        
        if(!bFree)
        {
            if(glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D))
                x += 0.03f;
            if(glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A))
                x -= 0.03f;
            if(glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W))
                y += 0.03f;
            if(glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S))
                y -= 0.03f;
        }

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        glm::mat4 trans = glm::mat4(1.0f);
        if(!bFree)
        {
            trans = glm::rotate(trans, (float)(angle_x = x), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, (float)(angle_y = y), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        else
        {
            trans = glm::rotate(trans, (float)(angle_x = x), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, (float)(angle_y = y), glm::vec3(1.0f, 0.0f, 0.0f));
            x += 0.01f;
            y += 0.01f;
        }

        unsigned int uniloc = glGetUniformLocation(program_id, "new_pos");
        glUniformMatrix4fv(uniloc, 1, GL_FALSE, glm::value_ptr(trans));
        glPolygonMode(GL_FRONT_AND_BACK,  GL_LINE);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	return 0;
}
