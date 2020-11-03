#include <cstdio>

#include "WindowWrapper.h"
#include "Shader.h"


int main()
{
    WindowWrapper window_wrapper(Vec3(1280, 720, 0), "Beatsy Visualizer");
    WindowWrapper::colorize(0.1, 0.1, 0.1);

    GLuint programID = LoadShaders("shaders/vertex.vert", "fragment.frag");

    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do {
		WindowWrapper::clear();
        glUseProgram(programID);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

		window_wrapper.draw();
	} while (window_wrapper.isClosed());

//    glDeleteBuffers(1, &vertexbuffer);
//    glDeleteVertexArrays(1, &VertexArrayID);
//    glDeleteProgram(programID);

    WindowWrapper::terminate();

	return 0;
}

