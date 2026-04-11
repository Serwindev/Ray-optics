#include <SFML/Graphics.hpp>
#include <math.h>
#include <cstdlib>

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

    sf::VertexArray Mirror(sf::PrimitiveType::Lines, 2);
    Mirror[0].position = {500.f, 250.f};
    Mirror[1].position = {500.f, 350.f};

    std::vector<Ray> rays;
    sf::Vector2f hit;
    
    
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();   
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                Ray ray;
                int randomNum = (std::rand() % (max-min + 1)) + min;
                ray.start = {0.f,300.f + randomNum};
                ray.end = {50.f, 300.f};

                ray.direction = ray.end - ray.start;

                float length = sqrt(ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y);
                ray.direction /= length;

                ray.speed = 5.f;

                rays.push_back(ray);
            }

            for (auto &ray : rays) {
                sf::Vector2f movement {
                    ray.direction.x * ray.speed,
                    ray.direction.y * ray.speed
                };

                ray.start += movement;
                ray.end += movement;

                if (intersect(ray.start, ray.end, Mirror[0].position, Mirror[1].position, hit)) {
                    ray.end = hit;
                    rays.erase(rays.begin());
                }
            }


        window.clear();
        for (auto &ray : rays) {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = ray.start;
            line[1].position = ray.end;
            window.draw(line);
            

            if (ray.start.x > 800 || ray.start.y > 600) {
                rays.erase(rays.begin());
            }
        }
        window.draw(Mirror);
        window.display();
        
    }

    return 0;
}