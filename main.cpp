#include <math.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "raylib.h"
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

bool intersect(Vector2 start, Vector2 end, Vector2 m_start, Vector2 m_end, Vector2 &hitPoint) {
    float x1 = start.x;
    float y1 = start.y;
    
    float x2 = end.x;
    float y2 = end.y;

    float x3 = m_start.x;
    float y3 = m_start.y;

    float x4 = m_end.x;
    float y4 = m_end.y;

    float den = (x1-x2)*(y3-y4) - (x3-x4)*(y1-y2);

    if (den == 0) return false;

    float t = ((x1-x3)*(y3-y4) - (y1-y3)*(x3-x4))  / den;
    float u = -((x1-x2)*(y1-y3) - (y1-y2)*(x1-x3)) / den;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        hitPoint.x = x1 + t*(x2-x1);
        hitPoint.y = y1 + t*(y2-y1);
        return true;
    }

    return false;
};

Vector2 operate(Vector2 a, Vector2 b, int oper) {
    Vector2 final;
    if (oper == 1) { // Add
        final = {a.x + b.x, a.y + b.y};
    }
    else if (oper == 2) { // Subtract
        final = {a.x - b.x, a.y - b.y};
    }
    return final;
}

Vector2 mul(Vector2 a, float b) {
    return {a.x*b, a.y*b};
}


Vector2 reflect(Vector2 D, Vector2 N) {
    float dot = D.x * N.x + D.y * N.y;

    if (dot > 0) {
        N = operate({0,0}, N,2);
        dot = D.x * N.x + D.y * N.y;
    }

    Vector2 R;
    R.x = D.x - 2*dot*N.x;
    R.y = D.y - 2*dot*N.y;

    return R;
}

int xMax = 800;
int xMin = 0;

int yMax = 600;
int yMin = 0;

int min = -5;
int max = 5;


struct Rays
{
    Vector2 start;
    Vector2 end;
    Vector2 direction;
    float speed;
};

struct Mirror
{
    Vector2 a;
    Vector2 b;
    Vector2 edge;
    Vector2 Normal;
};


std::vector<Rays> rays;
std::vector<Mirror> mirrors;

Vector2 hit;

bool isDrawing = false;
Vector2 tempStart;

bool isWon = false;
float xPos = 400.f;
float yPos = 300.f;

int maxMirrors = 10;
int numMirrors = 0;

int xp = 0;
int noCircle = 0;

bool isFirsttime = true;

void updateFrame() {

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && numMirrors < maxMirrors && !isFirsttime) {
            tempStart = GetMousePosition();
            isDrawing = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && numMirrors < maxMirrors && !isFirsttime) {
            Mirror mirror;

            mirror.a = tempStart;
            mirror.b = GetMousePosition();
            
            mirror.edge = operate(mirror.b, mirror.a, 2);
            mirror.Normal = {-mirror.edge.y, mirror.edge.x};

            float n_len = sqrt(mirror.Normal.x*mirror.Normal.x + mirror.Normal.y*mirror.Normal.y);
            mirror.Normal.x /= n_len;
            mirror.Normal.y /= n_len;

            mirrors.push_back(mirror);
            numMirrors += 1;
            isDrawing = false;
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !isFirsttime) {
                Rays ray;
                int randomNum = (std::rand() % (max-min + 1)) + min;
                ray.start = {0.f,300.f + randomNum};

                ray.direction = {1.f,0.f};
                ray.end = operate(ray.start, mul(ray.direction, 150.f), 1);

                float length = sqrt(ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y);
                ray.direction.x /= length;
                ray.direction.y /= length;

                ray.speed = 40.f;
                rays.push_back(ray);
            }

        if (IsKeyPressed(KEY_R) && numMirrors != 0) {
            mirrors.clear();
            isWon = false;
            rays.clear();
            numMirrors = 0;

            xp -= 10;
        }
        
        
            BeginDrawing();
            ClearBackground(BLACK);

            if (isFirsttime) {
                DrawText("Made by Serwin.dev",320,575,15,GREEN);
                DrawText("WELCOME TO RAY OPTICS",50,225,50, WHITE);
                DrawText("PRESS ENTER TO CONTINUE",200,400,25, WHITE);
                DrawText("TIP: By using mirrors, make the light ray to touch blue circle",225,450,10, RED);

                if (IsKeyPressed(KEY_ENTER)) {
                    isFirsttime = false;
                }
            }   
            else {
                DrawText("RIGHT CLICK to shoot rays", 10,10, 15, LIGHTGRAY);
                DrawText(TextFormat("Mirrors left: %d",maxMirrors - numMirrors), 600,500, 12, LIGHTGRAY);
                DrawText(TextFormat("XP: %d",xp), 600,525, 12, LIGHTGRAY);
                DrawText(TextFormat("Circle Touched: %d",noCircle), 600,550, 12, LIGHTGRAY);
                DrawText("LEFT CLICK & DRAG to place mirrors", 500,10, 15, LIGHTGRAY);
                DrawText("Press R to Restart (-10 xp)", 10,575, 15, LIGHTGRAY);
            }

            for (auto &ray : rays) {
                DrawLineEx({ray.start.x, ray.start.y}, {ray.end.x, ray.end.y},2.0f, YELLOW);
            }

            rays.erase(
                    std::remove_if(rays.begin(), rays.end(), [](const Rays &r) {
                        return r.start.x > 800 || r.start.y > 600 || r.start.x < 0 || r.start.y <0;
                    }),
                    rays.end()
                );
            
            for (auto &mirror : mirrors) {
                DrawLine(mirror.a.x, mirror.a.y, mirror.b.x, mirror.b.y, WHITE);
            }

            if (isDrawing) {
                Vector2 current = GetMousePosition();
                DrawLine(tempStart.x, tempStart.y, current.x, current.y, GRAY);
            }

            if (isWon) {
                xPos = (std::rand() % (xMax-xMin + 1)) + xMin;
                yPos = (std::rand() % (yMax-yMin + 1)) + yMin;
                mirrors.clear();
                rays.clear();
                numMirrors = 0;
                noCircle += 1;

                isWon = false;
            }
            if (!isFirsttime) {
                DrawCircle(20,300,8.f,YELLOW);
                DrawCircle(xPos, yPos, 20.f, BLUE);
            }

            for (auto &ray : rays) {

                Vector2 movement = {
                    ray.direction.x * ray.speed,
                    ray.direction.y * ray.speed
                };

                Vector2 circleCen = {xPos, yPos};
                Vector2 f = operate(ray.start, circleCen, 2);

                float a = movement.x * movement.x + movement.y * movement.y;
                float b = 2 * (f.x*movement.x + f.y*movement.y);
                float c = (f.x * f.x + f.y * f.y) - 500.f;

                float d = b*b - 4*a*c;

                float t = -1;

                if (d >= 0) {
                    d = sqrt(d);

                    float r1 = (-b - d) / (2*a);
                    float r2 = (-b + d) / (2*a);

                    if (r1>=0 && r1<=1) {
                        t = r1;
                    }
                    else if (r2>=0 && r2<=1) {
                        t = r2;
                    }

                    if (t != -1) {
                        isWon = true;
                        xp += 10;
                    }
                }

                ray.start = operate(ray.start, movement, 1);
                ray.end = operate(ray.start, mul(ray.direction, 150.f), 1);                

                for (auto &mirror : mirrors) {
                    if (intersect(ray.start, ray.end, mirror.a, mirror.b, hit)) {
                        Vector2 reflected = reflect(ray.direction, mirror.Normal);

                        ray.direction = reflected;
                        ray.start = operate(hit, mul(ray.direction, .1f), 1);
                        ray.end = operate(ray.start , mul(ray.direction, 150.f), 1);

                        break;
                    }
                }
            }

            EndDrawing();
}

int main()
{
        std::srand(std::time(0));
        InitWindow(800, 600, "Ray Optics");
        SetTargetFPS(30);

        #ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(updateFrame, 0, 1);
        #else
            while (!WindowShouldClose()) updateFrame();
        #endif
            return 0;
}