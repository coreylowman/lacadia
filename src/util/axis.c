#include <GL/glew.h>
#include <GL/glfw3.h>
#include "axis.h"
#include "mat4.h"

static float lines_vertices[] = {
    -1000.0, 0.0, 0.0, //x axis left
    1000.0, 0.0, 0.0, //x axis right
    0.0, -1000.0, 0.0, //y axis left
    0.0, 1000.0, 0.0, //y axis right
    0.0, 0.0, -1000.0, //z axis left
    0.0, 0.0, 1000.0 //z axis right
};

static float lines_colors[] = {
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
};

static float model_matrix[] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

void axis_init(Axis *self){
    glGenVertexArrays(1, &self->vao);    
    glBindVertexArray(self->vao);

    glGenBuffers(3, &self->vbo[0]);

    glBindBuffer(GL_ARRAY_BUFFER, self->vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), &lines_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
    glBindBuffer(GL_ARRAY_BUFFER, self->vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), &lines_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, self->vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &model_matrix[0], GL_STATIC_DRAW);

    //instance model matrix buffer object
    int i;
    for(i = 0;i < 4;i++){
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(4 * i * sizeof(float)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void axis_render(Axis self, Shader shader){
    glBindVertexArray(self.vao);
    glDrawArraysInstanced(GL_LINES, 0, 2 * 3 * 3, 1);
    glBindVertexArray(0);
}
