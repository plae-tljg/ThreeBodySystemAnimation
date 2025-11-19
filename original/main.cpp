#include "three_body.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main() {
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    const double SCALE = 100.0; // 像素/单位距离
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "三体问题动画");
    window.setFramerateLimit(60);
    
    ThreeBodySystem system(1.0, 0.01, 800);
    
    // 创建三个初始状态的物体（经典的8字形轨道配置）
    // 物体1
    system.addBody(Body(
        0.97000436, -0.24308753,  // 位置
        -0.4662036850, -0.4323657300,  // 速度
        1.0,  // 质量
        sf::Color::Red
    ));
    
    // 物体2
    system.addBody(Body(
        -0.97000436, 0.24308753,  // 位置
        -0.4662036850, -0.4323657300,  // 速度
        1.0,  // 质量
        sf::Color::Green
    ));
    
    // 物体3（中心）
    system.addBody(Body(
        0.0, 0.0,  // 位置
        0.93240737, 0.86473146,  // 速度
        1.0,  // 质量
        sf::Color::Blue
    ));
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // 按空格键重置
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                system = ThreeBodySystem(1.0, 0.01, 800);
                system.addBody(Body(0.97000436, -0.24308753, -0.4662036850, -0.4323657300, 1.0, sf::Color::Red));
                system.addBody(Body(-0.97000436, 0.24308753, -0.4662036850, -0.4323657300, 1.0, sf::Color::Green));
                system.addBody(Body(0.0, 0.0, 0.93240737, 0.86473146, 1.0, sf::Color::Blue));
            }
        }
        
        // 多次更新以提高模拟精度
        for (int i = 0; i < 5; i++) {
            system.update();
        }
        
        window.clear(sf::Color::Black);
        
        // 绘制坐标轴
        sf::VertexArray xAxis(sf::Lines, 2);
        xAxis[0] = sf::Vertex(sf::Vector2f(0, HEIGHT/2), sf::Color(50, 50, 50));
        xAxis[1] = sf::Vertex(sf::Vector2f(WIDTH, HEIGHT/2), sf::Color(50, 50, 50));
        window.draw(xAxis);
        
        sf::VertexArray yAxis(sf::Lines, 2);
        yAxis[0] = sf::Vertex(sf::Vector2f(WIDTH/2, 0), sf::Color(50, 50, 50));
        yAxis[1] = sf::Vertex(sf::Vector2f(WIDTH/2, HEIGHT), sf::Color(50, 50, 50));
        window.draw(yAxis);
        
        // 绘制三体系统
        system.draw(window, WIDTH/2, HEIGHT/2, SCALE);
        
        window.display();
    }
    
    return 0;
}

