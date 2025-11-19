#ifndef THREE_BODY_H
#define THREE_BODY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Body {
    double x, y;      // 位置
    double vx, vy;   // 速度
    double mass;     // 质量
    sf::Color color; // 颜色
    
    Body(double x, double y, double vx, double vy, double mass, sf::Color color)
        : x(x), y(y), vx(vx), vy(vy), mass(mass), color(color) {}
};

class ThreeBodySystem {
private:
    std::vector<Body> bodies;
    std::vector<std::vector<sf::Vector2f>> trails; // 轨迹记录
    double G; // 万有引力常数
    double dt; // 时间步长
    int maxTrailLength;
    
    // 计算两个物体之间的引力
    void computeForce(const Body& b1, const Body& b2, double& fx, double& fy) {
        double dx = b2.x - b1.x;
        double dy = b2.y - b1.y;
        double r2 = dx * dx + dy * dy;
        double r = sqrt(r2);
        
        // 避免除零，添加软化参数
        double softening = 0.1;
        r2 += softening * softening;
        r = sqrt(r2);
        
        double force = G * b1.mass * b2.mass / (r2 * r);
        fx = force * dx / r;
        fy = force * dy / r;
    }
    
public:
    ThreeBodySystem(double G = 1.0, double dt = 0.01, int maxTrail = 500)
        : G(G), dt(dt), maxTrailLength(maxTrail) {
        trails.resize(3);
    }
    
    void addBody(const Body& body) {
        bodies.push_back(body);
    }
    
    // 使用简化的数值积分方法（改进的Euler方法）
    void update() {
        if (bodies.size() != 3) return;
        
        // 计算每个物体受到的合力
        std::vector<std::pair<double, double>> accelerations(3);
        for (int i = 0; i < 3; i++) {
            double ax = 0, ay = 0;
            for (int j = 0; j < 3; j++) {
                if (i != j) {
                    double fx, fy;
                    computeForce(bodies[i], bodies[j], fx, fy);
                    ax += fx / bodies[i].mass;
                    ay += fy / bodies[i].mass;
                }
            }
            accelerations[i] = {ax, ay};
        }
        
        // 更新速度和位置
        for (int i = 0; i < 3; i++) {
            bodies[i].vx += accelerations[i].first * dt;
            bodies[i].vy += accelerations[i].second * dt;
            bodies[i].x += bodies[i].vx * dt;
            bodies[i].y += bodies[i].vy * dt;
        }
        
        // 更新轨迹
        for (size_t i = 0; i < bodies.size(); i++) {
            trails[i].push_back(sf::Vector2f(bodies[i].x, bodies[i].y));
            if (trails[i].size() > maxTrailLength) {
                trails[i].erase(trails[i].begin());
            }
        }
    }
    
    void draw(sf::RenderWindow& window, int centerX, int centerY, double scale) {
        // 绘制轨迹
        for (size_t i = 0; i < trails.size(); i++) {
            if (trails[i].size() < 2) continue;
            
            sf::VertexArray line(sf::LineStrip, trails[i].size());
            for (size_t j = 0; j < trails[i].size(); j++) {
                float alpha = 255.0f * (j / float(trails[i].size()));
                sf::Color trailColor = bodies[i].color;
                trailColor.a = static_cast<sf::Uint8>(alpha);
                line[j] = sf::Vertex(
                    sf::Vector2f(centerX + trails[i][j].x * scale, 
                                centerY + trails[i][j].y * scale),
                    trailColor
                );
            }
            window.draw(line);
        }
        
        // 绘制物体
        for (const auto& body : bodies) {
            sf::CircleShape circle(5);
            circle.setFillColor(body.color);
            circle.setPosition(centerX + body.x * scale - 5, 
                              centerY + body.y * scale - 5);
            window.draw(circle);
        }
    }
    
    const std::vector<Body>& getBodies() const { return bodies; }
};

#endif

