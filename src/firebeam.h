#include "main.h"

#ifndef FIREBEAM_H
#define FIREBEAM_H
#define GLM_ENABLE_EXPERIMENTAL


class Firebeam {
public:
    Firebeam() {}
    Firebeam(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool direction;
private:
    VAO *object[2];
};

#endif // FIREBEAM_H
