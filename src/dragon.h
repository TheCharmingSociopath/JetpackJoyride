#include "main.h"

#ifndef DRAGON_H
#define DRAGON_H
#define GLM_ENABLE_EXPERIMENTAL


class Dragon {
public:
    Dragon() {}
    Dragon(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bool direction = true;
    int freeze_fire_time = 0;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // DRAGON_H
