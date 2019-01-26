#include "main.h"

#ifndef PROPULSION_H
#define PROPULSION_H
#define GLM_ENABLE_EXPERIMENTAL


class Propulsion {
public:
    Propulsion() {}
    Propulsion(float x, float y);
    glm::vec3 position;
    float rotation, arg;
    int tim;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // PROPULSION_H
