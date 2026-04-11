#include <math.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "raylib.h"
#include <vector>
#include <cmath>
#include <cstdlib>

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

int main()
{
    InitWindow(800,600,"Ray Optics");
    SetTargetFPS(30);

    std::srand(std::time(0));
    int min = -5;
    int max = 5;


    struct Ray
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


    std::vector<Ray> rays;
    std::vector<Mirror> mirrors;

    Vector2 hit;

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Mirror mirror;

            Vector2 mousePos = GetMousePosition();

            mirror.a = {mousePos.x, mousePos.y + 75};
            mirror.b = {mousePos.x, mousePos.y - 75};

            mirror.edge = operate(mirror.b, mirror.a, 2);
            mirror.Normal = {-mirror.edge.y, mirror.edge.x};

            float n_len = sqrt(mirror.Normal.x*mirror.Normal.x + mirror.Normal.y*mirror.Normal.y);
            mirror.Normal.x /= n_len;
            mirror.Normal.y /= n_len;

            mirrors.push_back(mirror);
        
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                Ray ray;
                int randomNum = (std::rand() % (max-min + 1)) + min;
                ray.start = {0.f,300.f + randomNum};
                
                float angle = randomNum * 0.01f;
                ray.direction = {cos(angle), sin(angle)};

                ray.end = operate(ray.start, mul(ray.direction, 500.f), 1);

                float length = sqrt(ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y);
                ray.direction.x /= length;
                ray.direction.y /= length;

                ray.speed = 30.f;

                rays.push_back(ray);
            }
        
        
            BeginDrawing();
            for (auto &ray : rays) {

                Vector2 movement = {
                    ray.direction.x * ray.speed,
                    ray.direction.y * ray.speed
                };

                Vector2 nStart = operate(ray.start, movement, 1);
               // Vector2 nEnd = operate(ray.end, movement, 1);
                bool reflectedFrame = false;

                for (auto &mirror : mirrors) {
                    if (intersect(ray.start, nStart, mirror.a, mirror.b, hit)) {
                        Vector2 reflected = reflect(ray.direction, mirror.Normal);

                        ray.direction = reflected;
                        ray.start = operate(hit, mul(ray.direction, .1f), 1);
                        ray.end = operate(ray.start , mul(ray.direction, 500.f), 1);

                        reflectedFrame = true;
                        break;
                    }
                }
                if (!reflectedFrame) {
                    ray.start = nStart;
                    ray.end = operate(ray.start, mul(ray.direction, 500.f), 1);
                }
            }

            

            ClearBackground(BLACK);
            for (auto &ray : rays) {
                DrawLine(ray.start.x, ray.start.y, ray.end.x, ray.end.y, YELLOW);
            }

            rays.erase(
                    std::remove_if(rays.begin(), rays.end(), [](const Ray &r) {
                        return r.start.x > 800 || r.start.y > 600 || r.start.x < 0 || r.start.y <0;
                    }),
                    rays.end()
                );
            
            for (auto &mirror : mirrors) {
                DrawLine(mirror.a.x, mirror.a.y, mirror.b.x, mirror.b.y, WHITE);
            }
        
        
            EndDrawing();
    }
    
    return 0;
}