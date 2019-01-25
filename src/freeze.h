#include "main.h"

#ifndef FREEZE_H
#define FREEZE_H
#define GLM_ENABLE_EXPERIMENTAL


class Freeze {
public:
    Freeze() {}
    Freeze(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float speed_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // FREEZE_H
