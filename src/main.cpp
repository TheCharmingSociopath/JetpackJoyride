#include "main.h"
#include "timer.h"
#include "ball.h"
#include "platform.h"
#include "coin.h"
#include "firebeam.h"
#include "fireline.h"
#include "magnet.h"
#include "boomerang.h"
#include "speed.h"
#include "baloon.h"
#include "life.h"
#include "shield.h"
#include "dragon.h"
#include "freeze.h"
#include "score.h"
#include "ring.h"

#define GLM_ENABLE_EXPERIMENTAL

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball;
Firebeam firebeam;
Fireline fireline;
Magnet magnet;
Boomerang boomerang;
Speed speed;
Life life;
Shield shield;
Dragon dragon;
Score p_score, p_life;
Ring ring;

bounding_box_t b_ball, b_coin, b_fireline, b_firebeam, b_boomerang, b_speed, b_baloon, b_life, b_shield, b_freeze, b_ring;
vector <Platform> platform;
vector <Coin> coin;
vector <Baloon> baloon;
vector <Freeze> freeze;

float screen_zoom = 1.0f, screen_center_x = 0.0f, screen_center_y = 0.0f, camera_center_x = 0.0f, camera_center_y = 0.0f;
float camera_rotation_angle = 0.0f;
long long int score = 0, lives = 5, tock = 0, speed_time = 0, coins_collected = 0, last_baloon;
float HORIZONTAL_MOVEMENT_VALUE = 0.1f;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    glm::vec3 eye ( camera_center_x, camera_center_y, 5 );

    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (camera_center_x, camera_center_y, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    for (int i=0; i < platform.size(); ++i)
    {
        platform[i].draw(VP);
    }
    for (int i=0; i < coin.size(); ++i)
    {
        coin[i].draw(VP);
    }
    for (int i=0; i < baloon.size(); ++i)
    {
        baloon[i].draw(VP);
    }
    for (int i=0; i < freeze.size(); ++i)
    {
        freeze[i].draw(VP);
    }
    firebeam.draw(VP);
    fireline.draw(VP);
    magnet.draw(VP);
    boomerang.draw(VP);
    speed.draw(VP);
    life.draw(VP);
    shield.draw(VP);
    dragon.draw(VP);
    ring.draw(VP);
    ball.draw(VP);
    p_score.set_position(camera_center_x + 7, camera_center_y + 7);
    p_score.print_score(score, VP);
    p_life.set_position(camera_center_x - 7, camera_center_y + 7);
    p_life.print_score(lives, VP);
}

void tick_input(GLFWwindow *window) {
    if (!ball.is_frozen and !ball.in_ring)
    {
        int left  = glfwGetKey(window, GLFW_KEY_LEFT);
        int right = glfwGetKey(window, GLFW_KEY_RIGHT);
        int space = glfwGetKey(window, GLFW_KEY_SPACE);
        int up = glfwGetKey(window, GLFW_KEY_UP);
        int down = glfwGetKey(window, GLFW_KEY_DOWN);
        int b = glfwGetKey(window, GLFW_KEY_B);

        if (left) {
            camera_center_x -= HORIZONTAL_MOVEMENT_VALUE;
            ball.position.x = camera_center_x;
        }
        if (right) {
            camera_center_x += HORIZONTAL_MOVEMENT_VALUE;
            ball.position.x = camera_center_x;
        }
        if (space) {
            ball.speed = 0.1f;
        }
        if (up) {
            screen_zoom += 0.01f;
        }
        if (down) {
            screen_zoom -= 0.01f;
        }
        if (b and tock - last_baloon > 30) {
            baloon.push_back(Baloon(ball.position.x + 0.8, ball.position.y, COLOR_BLUE));
            last_baloon = tock;
        }
    }
    if (ball.in_ring)
    {
        int left  = glfwGetKey(window, GLFW_KEY_LEFT);
        int right = glfwGetKey(window, GLFW_KEY_RIGHT);
        int up = glfwGetKey(window, GLFW_KEY_UP);
        int down = glfwGetKey(window, GLFW_KEY_DOWN);

        if (left) {
            ball.arg -= 0.05f;
            if(ball.arg < pi) ball.arg = pi;
            camera_center_x = ring.position.x + 5.0f * cos(ball.arg);
            ball.position.x = camera_center_x;
            ball.position.y = ring.position.y + 4.0f * sin(ball.arg);
        }
        if (right) {
            ball.arg += 0.05f;
            camera_center_x = ring.position.x + 5.0f * cos(ball.arg);
            ball.position.x = camera_center_x;
            ball.position.y = ring.position.y + 4.0f * sin(ball.arg);
            cout << ball.arg << " " << ball.in_ring << endl;
        }
        if (up) {
            screen_zoom += 0.01f;
        }
        if (down) {
            screen_zoom -= 0.01f;
        }
    }
}

void tick_elements() {
    ball.tick();
    firebeam.tick();
    boomerang.tick();
    speed.tick();
    life.tick();
    shield.tick();
    dragon.tick();

    for(int i=0; i<baloon.size(); ++i)
    {
        baloon[i].tick();
        if(baloon[i].position.x - ball.position.x >= 15)
            baloon.erase(baloon.begin() + i);
    }
    for(int i=0; i<freeze.size(); ++i)
    {
        freeze[i].tick();
        if(dragon.position.x - freeze[i].position.x >= 15)
            freeze.erase(freeze.begin() + i);
    }
    if(!ball.is_frozen and !ball.in_ring)
    {
        float r = sqrt((ball.position.x - magnet.position.x) * (ball.position.x - magnet.position.x) + (ball.position.y - magnet.position.y) * (ball.position.y - magnet.position.y));

        if (r <= 50.0f and r >= 3.0f)
        {
            ball.position.x -= 8 * ((ball.position.x - magnet.position.x) / pow(r, 3));
            ball.position.y -= 8 * ((ball.position.y - magnet.position.y) / pow(r, 3));
        }
        if(r < 3.0f)
        {
            ball.position.y = magnet.position.y - 1.0f;
        }
    }
    if (ball.is_frozen and tock - ball.freeze_time > 300)
        ball.is_frozen = false;

    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball = Ball(screen_center_x, -6, COLOR_BODY);

    for (int i=0; i < 1000; ++i)
    {
        platform.push_back(Platform(-7 + i, -7, COLOR_PLATFORM));
    }
    for (int i=0; i<10; i += 2)
    {
        coin.push_back(Coin(10 + i, 1, COLOR_COIN_YELLOW));
        coin.push_back(Coin(11 + i, 2, COLOR_COIN_YELLOW));
    }

    firebeam = Firebeam(camera_center_x + 30, 1, COLOR_RED);
    fireline = Fireline(camera_center_x + 40, 1, COLOR_RED, rand() % 90);
    magnet = Magnet(camera_center_x + 50, 7, COLOR_RED);
    boomerang = Boomerang(camera_center_x + 10, 2, COLOR_BLACK);
    speed = Speed(camera_center_x + 1000, -5, COLOR_COIN_YELLOW);
    life = Life(camera_center_x, 50, COLOR_LIFE);
    shield = Shield(camera_center_x, 50, COLOR_SHIELD);
    dragon = Dragon(camera_center_x + 50, 0, COLOR_EVIL);
    ring = Ring(camera_center_x + 70, -2);
    p_score = Score(7, 7, COLOR_BLACK);
    p_life = Score(-7, 7, COLOR_BLACK);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        b_ball.x = ball.position.x;
        b_ball.y = ball.position.y;
        b_ball.width = 2;
        b_ball.height = 3;

        if (t60.processTick()) {
            ++tock;
            // 60 fps
            // OpenGL Draw commands

            check_collisions();

            generate_obstacles();

            if ((int)camera_center_x % 10 == 0)
            {
                cout << "Score: " << score << endl;
                cout << "Lives: " << lives << endl;
            }
            if (lives == 0)
            {
                cout << "Game Over :(" << endl;
                cout << "Score: " << score << endl;
                cout << "Lives: " << lives << endl;
                return 0;
            }
            if(magnet.time >= 600)
            {
                magnet = Magnet(camera_center_x + 10, 7, COLOR_RED);
            }
            if(boomerang.time >= 800)
            {
                boomerang = Boomerang(camera_center_x + 15, 2, COLOR_BLACK);
            }

            if(HORIZONTAL_MOVEMENT_VALUE > 0.1f and tock - speed_time > 300.0)
            {
                HORIZONTAL_MOVEMENT_VALUE = 0.1f;
            }

            if(tock % 60 == 0)
                ++score;

            if(ball.shield and tock - ball.shield_time > 300)
                ball.shield = false;

            if (!ball.is_frozen and tock - dragon.freeze_fire_time >= 60)
            {
                freeze.push_back(Freeze(dragon.position.x - 0.8, dragon.position.y, COLOR_FREEZE));
                dragon.freeze_fire_time = tock;
            }

            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
            reset_screen();
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {

    camera_center_y = 0;
    if(screen_zoom > 1.5)
    {
        camera_center_y = ball.position.y;
    }
    float top    = screen_center_y + 8 / screen_zoom;
    float bottom = screen_center_y - 8 / screen_zoom;
    float left   = screen_center_x - 8 / screen_zoom;
    float right  = screen_center_x + 8 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

bool detect_collision_fireline()
{
    float a = tan(fireline.rotation / 180 * pi), x1 = ball.position.x + 1, x2 = ball.position.x - 1,
    x3 = ball.position.x + 1, x4 = ball.position.x - 1, y1 = ball.position.y + 2, y2 = ball.position.y + 2,
    y3 = ball.position.y - 1, y4 = ball.position.y - 1, b = -1,
    c = fireline.position.y - (a * fireline.position.x);

    if ( abs((a * x1 + b * y1 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x2 + b * y2 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x3 + b * y3 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x4 + b * y4 + c) / sqrt(a*a + b*b)) <= 0.5 )
        return true;

    return false;
}

bool detect_collision_firebeam()
{
    float a = tan(fireline.rotation / 180 * pi), x1 = ball.position.x + 1, x2 = ball.position.x - 1,
    x3 = ball.position.x + 1, x4 = ball.position.x - 1, y1 = ball.position.y + 2, y2 = ball.position.y + 2,
    y3 = ball.position.y - 1, y4 = ball.position.y - 1, b = -1,
    c = fireline.position.y - (a * fireline.position.x);

    if ( abs((a * x1 + b * y1 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x2 + b * y2 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x3 + b * y3 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x4 + b * y4 + c) / sqrt(a*a + b*b)) <= 0.5 )
        return true;

    return false;
}

bool detect_collision_baloon_fireline(Baloon bal)
{
    float a = 0, x1 = bal.position.x + 1, x2 = bal.position.x - 1,
    x3 = bal.position.x + 1, x4 = bal.position.x - 1, y1 = bal.position.y + 2, y2 = bal.position.y + 2,
    y3 = bal.position.y - 1, y4 = bal.position.y - 1, b = -1,
    c = fireline.position.y - (a * fireline.position.x);

    if ( abs((a * x1 + b * y1 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x2 + b * y2 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x3 + b * y3 + c) / sqrt(a*a + b*b)) <= 0.5 or
    abs((a * x4 + b * y4 + c) / sqrt(a*a + b*b)) <= 0.5 )
        return true;

    return false;
}

bool detect_collision_baloon_firebeam(Baloon bal)
{
    float x1 = ball.position.x + 1, x2 = ball.position.x - 1,
    x3 = ball.position.x + 1, x4 = ball.position.x - 1, y1 = ball.position.y + 2, y2 = ball.position.y + 2,
    y3 = ball.position.y - 1, y4 = ball.position.y - 1, b = -1;

    if ( bal.position.x <= firebeam.position.x + 4.6 and bal.position.x >= firebeam.position.x - 4.6
    and bal.position.y <= firebeam.position.y + 0.3 and bal.position.y >= firebeam.position.y - 0.3)
        return true;

    return false;
}

void check_collisions()
{
    // Coin's collission with player
    for(int i=0; i<coin.size(); ++i)
    {
        b_coin.x = coin[i].position.x;
        b_coin.y = coin[i].position.y;
        b_coin.height = 0.5;
        b_coin.width = 0.5;

        if (detect_collision(b_ball, b_coin))
        {
            if (coin[i].color.g == 0)
                coins_collected += 10;
            else if(coin[i].color.r == 0)
                coins_collected += 5;
            else
                ++coins_collected;
            coin.erase(coin.begin() + i);
            score += 5;
        }
    }
    //======================================================

    // Collission with firebeams
    if(!ball.shield)
    {
        b_firebeam.x = firebeam.position.x;
        b_firebeam.y = firebeam.position.y;
        b_firebeam.height = 0.8;
        b_firebeam.width = 9.2;

        if (detect_collision(b_firebeam, b_ball) and detect_collision_firebeam())
        {
            --score;
            if (score < 0) score = 0;
            // ball = Ball(screen_center_x, -6, COLOR_BODY);
            draw();
        }
    }
    //======================================================

    // Collission with firelines
    if(!ball.shield)
    {
        b_fireline.x = fireline.position.x;
        b_fireline.y = fireline.position.y;
        b_fireline.width = 7.2 * cos(fireline.rotation / 180 * pi);
        b_fireline.height = 7.2 * sin(fireline.rotation / 180 * pi);

        if(detect_collision(b_ball, b_fireline) and detect_collision_fireline())
        {
            --score;
            if (score < 0) score = 0;
            // ball = Ball(screen_center_x, -6, COLOR_BODY);
            draw();
        }
    }
    //======================================================

    // Collission with boomerang
    if (!ball.shield)
    {
        b_boomerang.x = boomerang.position.x;
        b_boomerang.y = boomerang.position.y;
        b_boomerang.width = 1.4f;
        b_boomerang.height = 1.4f;

        if(detect_collision(b_ball, b_boomerang))
        {
            --lives;
            ball = Ball(screen_center_x, -6, COLOR_BODY);
        }
    }
    //======================================================

    // Collission with speed
    b_speed.x = speed.position.x;
    b_speed.y = speed.position.y;
    b_speed.width = 1.6f;
    b_speed.height = 2.8f;

    if(detect_collision(b_ball, b_speed))
    {
        HORIZONTAL_MOVEMENT_VALUE += 0.1f;
        speed_time = tock;
        speed.position.y = 50;
    }
    //======================================================

    // Collission with life
    b_life.x = life.position.x;
    b_life.y = life.position.y;
    b_life.width = 1.0f;
    b_life.height = 1.0f;

    if(detect_collision(b_ball, b_life))
    {
        life.position.y = 50;
        ++lives;
    }
    //======================================================

    // Collission with shield
    b_shield.x = shield.position.x;
    b_shield.y = shield.position.y;
    b_shield.width = 2.0f;
    b_shield.height = 2.0f;

    if(detect_collision(b_ball, b_shield))
    {
        shield.position.y = 50;
        ball.shield = true;
        ball.shield_time = tock;
    }
    //======================================================

    // Collission of baloon with lazer
    for (int i=0; i < baloon.size(); ++i)
    {
        b_baloon.x = baloon[i].position.x;
        b_baloon.y = baloon[i].position.y;
        b_baloon.width = 1.6f;
        b_baloon.height = 1.6f;

        if(detect_collision_baloon_fireline(baloon[i]))
        {
            fireline.set_position(fireline.position.x, 60);
            baloon.erase(baloon.begin() + i);
        }
        if(detect_collision_baloon_firebeam(baloon[i]))
        {
            firebeam.set_position(firebeam.position.x, 60);
            baloon.erase(baloon.begin() + i);
        }
    }
    //======================================================

    // Collission of freeze with player
    for (int i=0; i < freeze.size(); ++i)
    {
        b_freeze.x = freeze[i].position.x;
        b_freeze.y = freeze[i].position.y;
        b_freeze.width = 1.6f;
        b_freeze.height = 1.6f;

        if(!ball.shield and detect_collision(b_ball, b_freeze))
        {
            ball.freeze_time = tock;
            ball.is_frozen = true;
            freeze.erase(freeze.begin() + i);
        }
    }
    //======================================================

    // Collission of ring with player in beginning 
    b_ring.x = ring.position.x - 5;
    b_ring.y = ring.position.y;
    b_ring.width = 2.0f;
    b_ring.height = 2.0f;

    if(!ball.in_ring and detect_collision(b_ball, b_ring))
    {
        ball.arg = pi + 0.1f;
        ball.shield = true;
        ball.in_ring = true;
    }
    //======================================================

    // Collission of ring with player at end
    b_ring.x = ring.position.x + 5;
    b_ring.y = ring.position.y;
    b_ring.width = 2.0f;
    b_ring.height = 2.0f;

    if(ball.in_ring and detect_collision(b_ball, b_ring))
    {
        ball.arg = 0;
        ball.shield = false;
        ball.in_ring = false;
    }
    //======================================================
}

void generate_obstacles()
{
    // Generate objects
    if ((int)(camera_center_x * 10) % 2500 == 0 and (int)camera_center_x != 0)
    {
        for (int i=0; i<10; i += 2)
        {
            coin.push_back(Coin(camera_center_x + 10 + i, 1, COLOR_COIN_RED));
            coin.push_back(Coin(camera_center_x + 11 + i, 2, COLOR_COIN_RED));
        }
    }
    else if ((int)(camera_center_x * 10) % 1500 == 0 and (int)camera_center_x != 0)
    {
        for (int i=0; i<10; i += 2)
        {
            coin.push_back(Coin(camera_center_x + 10 + i, 1, COLOR_COIN_GREEN));
            coin.push_back(Coin(camera_center_x + 11 + i, 2, COLOR_COIN_GREEN));
        }
        magnet = Magnet(camera_center_x + 20, 7, COLOR_RED);                
    }
    else if ((int)(camera_center_x * 10) % 1000 == 0 and camera_center_x != 0)
    {
        //make fire line
        fireline = Fireline(camera_center_x + 10, rand() % 7, COLOR_RED, rand() % 70);
    }
    else if ((int)(camera_center_x * 10) % 500 == 0 and (int)camera_center_x != 0)
    {
        for (int i=0; i<10; i += 2)
        {
            coin.push_back(Coin(camera_center_x + 10 + i, 1, COLOR_COIN_YELLOW));
            coin.push_back(Coin(camera_center_x + 11 + i, 2, COLOR_COIN_YELLOW));
        }
    }
    else if ((int)(camera_center_x * 10) % 250 == 0 and camera_center_x != 0 and (int)camera_center_x != 25)
    {
        //make fire beam
        firebeam = Firebeam(camera_center_x + 8, rand() % 7, COLOR_RED);
    }
    else if ((int)(camera_center_x * 10) % 50 == 0 and camera_center_x != 0 and (int)camera_center_x != 25)
    {
        coin.push_back(Coin(camera_center_x + 10, (rand() % 5) - 2, COLOR_COIN_YELLOW));
    }
    // ------------------------------------------------------

    if(tock % 600 == 0)
    {
        speed = Speed(camera_center_x + 10, (rand() % 5) - 2, COLOR_COIN_YELLOW);
    }
    if(tock % 800 == 0)
    {
        shield = Shield(camera_center_x + 15, (rand() % 5) - 2, COLOR_SHIELD);
    }
    if(tock % 1000 == 0)
    {
        life = Life(camera_center_x + 20, (rand() % 5) - 2, COLOR_LIFE);
    }
    if(tock % 2400 == 0)
    {
        dragon = Dragon(camera_center_x + 20, 0, COLOR_EVIL);
    }
}
