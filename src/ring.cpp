#include "ring.h"
#include "main.h"

#define GLM_ENABLE_EXPERIMENTAL

Ring::Ring(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

	double arg = 0;

    GLfloat vertex_buffer_data1[4500], vertex_buffer_data2[4500];

	for(int i=0; i<4500;)
	{
		vertex_buffer_data1[i++] = 0.0f;
		vertex_buffer_data1[i++] = 0.0f;
		vertex_buffer_data1[i++] = 0.0f;

		vertex_buffer_data1[i++] = 4 * cos(arg);
		vertex_buffer_data1[i++] = 3 * sin(arg);
		vertex_buffer_data1[i++] = 0.0f;

		arg -= (2 * pi) / (double) 1000;

		vertex_buffer_data1[i++] = 4 * cos(arg);
		vertex_buffer_data1[i++] = 3 * sin(arg);
		vertex_buffer_data1[i++] = 0.0f;
	}
    this->object[0] = create3DObject(GL_TRIANGLES, 1500, vertex_buffer_data1, color, GL_FILL);

    arg = 0.0f;
    for(int i=0; i<4500;)
	{
		vertex_buffer_data2[i++] = 0.0f;
		vertex_buffer_data2[i++] = 0.0f;
		vertex_buffer_data2[i++] = 0.0f;

		vertex_buffer_data2[i++] = 6 * cos(arg);
		vertex_buffer_data2[i++] = 5 * sin(arg);
		vertex_buffer_data2[i++] = 0.0f;

		arg -= (2 * pi) / (double) 1000;

		vertex_buffer_data2[i++] = 6 * cos(arg);
		vertex_buffer_data2[i++] = 5 * sin(arg);
		vertex_buffer_data2[i++] = 0.0f;
	}
    this->object[1] = create3DObject(GL_TRIANGLES, 1500, vertex_buffer_data2, COLOR_COIN_YELLOW, GL_FILL);
}

void Ring::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object[1]);
    draw3DObject(this->object[0]);
}

void Ring::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ring::tick() {
    // this->position.x += 0.1;
}
