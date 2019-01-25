#include "main.h"

#ifndef BALL_H
#define BALL_H
#define GLM_ENABLE_EXPERIMENTAL


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    int shield_time;
    bool shield = false;
    int freeze_time = 0;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
