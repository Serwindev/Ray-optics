#include <SFML/Graphics.hpp>
#include <math.h>
#include <cstdlib>
#include <algorithm>

bool intersect(sf::Vector2f start, sf::Vector2f end, sf::Vector2f m_start, sf::Vector2f m_end, sf::Vector2f &hitPoint) {
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

    if (t > 0 && t < 1 && u > 0 && u < 1) {
        hitPoint.x = x1 + t*(x2-x1);
        hitPoint.y = y1 + t*(y2-y1);
        return true;
    }

    return false;
};

sf::Vector2f reflect(sf::Vector2f D, sf::Vector2f N) {
    float dot = D.x * N.x + D.y * N.y;

    if (dot > 0) {
        N = -N;
        dot = D.x * N.x + D.y * N.y;
    }

    sf::Vector2f R;
    R.x = D.x - 2*dot*N.x;
    R.y = D.y - 2*dot*N.y;

    return R;
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(800, 600)),
        "Test"
    );

    std::srand(std::time(0));
    int min = -5;
    int max = 5;


    struct Ray
    {
        sf::Vector2f start;
        sf::Vector2f end;
        sf::Vector2f direction;
        float speed;
    };

    struct Mirror
    {
        sf::Vector2f a;
        sf::Vector2f b;
        sf::Vector2f edge;
        sf::Vector2f Normal;
    };


    std::vector<Ray> rays;
    std::vector<Mirror> mirrors;

    sf::Vector2f hit;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();   

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Space) {
                    Mirror mirror;

                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    mirror.a = {mousePos.x, mousePos.y + 75};
                    mirror.b = {mousePos.x, mousePos.y - 75};

                    mirror.edge = mirror.b - mirror.a;
                    mirror.Normal = {-mirror.edge.y, mirror.edge.x};

                    float n_len = sqrt(mirror.Normal.x*mirror.Normal.x + mirror.Normal.y*mirror.Normal.y);
                    mirror.Normal /= n_len;

                    mirrors.push_back(mirror);
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                Ray ray;
                int randomNum = (std::rand() % (max-min + 1)) + min;
                ray.start = {0.f,300.f + randomNum};
                ray.end = {75.f, 300.f};

                ray.direction = ray.end - ray.start;

                float length = sqrt(ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y);
                ray.direction /= length;

                ray.speed = 5.f;

                rays.push_back(ray);
            }
        
        

            for (auto &ray : rays) {
                for (auto &mirror : mirrors) {
                    if (intersect(ray.start, ray.end, mirror.a, mirror.b, hit)) {
                        sf::Vector2f reflected = reflect(ray.direction, mirror.Normal);

                        ray.direction = reflected;
                        ray.start = hit + ray.direction * 0.1f;
                        ray.end = ray.start + ray.direction * 75.f;
                    }
                }
            }

            for (auto &ray : rays) {
                sf::Vector2f movement {
                    ray.direction.x * ray.speed,
                    ray.direction.y * ray.speed
                };

                ray.start += movement;
                ray.end += movement;
            }


        window.clear();
        for (auto &ray : rays) {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = ray.start;
            line[1].position = ray.end;
            window.draw(line);
            

            rays.erase(
                std::remove_if(rays.begin(), rays.end(), [](const Ray &r) {
                    return r.start.x > 800 || r.start.y > 600 || r.start.x < 0 || r.start.y <0;
                }),
                rays.end()
            );
        }
        
        for (auto &mirror : mirrors) {
            sf::VertexArray mirror_line(sf::PrimitiveType::Lines, 2);
            mirror_line[0].position = mirror.a;
            mirror_line[1].position = mirror.b;
            
            window.draw(mirror_line);
        }

        window.display();
        
    }

    return 0;
}