#include "platform.h"
#include "main.h"

#define GLM_ENABLE_EXPERIMENTAL

Platform::Platform(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f, 0.0f, // triangle 1 : begin
        -1.0f,2.0f, 0.0f,
        1.0f, 2.0f, 0.0f, // triangle 1 : end
    };
    static const GLfloat vertex_buffer_data1[] = {
        -1.0f,-1.0f,0.0f, // triangle 2 : begin
        1.0f,-1.0f,0.0f,
        1.0f,2.0f,0.0f, // triangle 2 : end
    };
    this->object[0] = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_PLATFORM, GL_FILL);
    this->object[1] = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data1, COLOR_PLATFORM2, GL_FILL);
}

void Platform::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object[0]);
    draw3DObject(this->object[1]);
}

void Platform::tick() {
    // this->position.x += 0.1;
}
