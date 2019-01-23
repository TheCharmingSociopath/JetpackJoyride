#include "ball.h"
#include "main.h"

#define GLM_ENABLE_EXPERIMENTAL
using namespace std;

double gravity = 0.5, t = (double) 1/60;

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f, 0.0f, // triangle 1 : begin
        -1.0f,2.0f, 0.0f,
        1.0f, 2.0f, 0.0f, // triangle 1 : end

        -1.0f,-1.0f,0.0f, // triangle 2 : begin
        1.0f,-1.0f,0.0f,
        1.0f,2.0f,0.0f, // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    // this->rotation += speed;
    if (this->position.y < 6 or this -> speed < 0)
        this->position.y += this->speed;

    if (this->position.y > -ground)
        this->speed = this->speed - gravity * t;

    if (this->position.y <= -ground)
    {
        this->speed = 0;
        this->position.y = -ground;
    }
}

