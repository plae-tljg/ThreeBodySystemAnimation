#include "three_body.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

// 函数：从用户输入获取初始条件
void getInitialConditions(ThreeBodySystem& system) {
    std::cout << "\n=== 三体问题初始条件设置 ===\n";
    std::cout << "按回车使用默认值（经典8字形轨道），或输入 'c' 自定义：";
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "c" || choice == "C") {
        // 自定义输入
        std::cout << "\n请输入三个物体的初始条件：\n";
        std::cout << "格式：位置x 位置y 速度x 速度y 质量\n\n";
        
        const char* colors[] = {"红色", "绿色", "蓝色"};
        const sf::Color colorValues[] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue};
        
        for (int i = 0; i < 3; i++) {
            std::cout << "物体 " << (i+1) << " (" << colors[i] << "): ";
            double x, y, vx, vy, mass;
            
            if (std::cin >> x >> y >> vx >> vy >> mass) {
                system.addBody(Body(x, y, vx, vy, mass, colorValues[i]));
                std::cout << "  已设置: 位置(" << x << ", " << y << "), 速度(" 
                          << vx << ", " << vy << "), 质量=" << mass << "\n";
            } else {
                std::cout << "输入错误，使用默认值\n";
                // 使用默认值
                if (i == 0) system.addBody(Body(0.97000436, -0.24308753, -0.4662036850, -0.4323657300, 1.0, sf::Color::Red));
                else if (i == 1) system.addBody(Body(-0.97000436, 0.24308753, -0.4662036850, -0.4323657300, 1.0, sf::Color::Green));
                else system.addBody(Body(0.0, 0.0, 0.93240737, 0.86473146, 1.0, sf::Color::Blue));
            }
        }
        std::cin.ignore(); // 清除输入缓冲区
    } else {
        // 使用默认值
        std::cout << "使用默认初始条件（经典8字形轨道）...\n";
        system.addBody(Body(0.97000436, -0.24308753, -0.4662036850, -0.4323657300, 1.0, sf::Color::Red));
        system.addBody(Body(-0.97000436, 0.24308753, -0.4662036850, -0.4323657300, 1.0, sf::Color::Green));
        system.addBody(Body(0.0, 0.0, 0.93240737, 0.86473146, 1.0, sf::Color::Blue));
    }
    
    std::cout << "\n初始条件设置完成！窗口将打开...\n";
    std::cout << "提示：按空格键可重新输入初始条件\n\n";
}

int main() {
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    const double SCALE = 100.0; // 像素/单位距离
    
    // 获取初始条件
    ThreeBodySystem system(1.0, 0.01, 800);
    getInitialConditions(system);
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "三体问题动画");
    window.setFramerateLimit(60);
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // 按空格键重置并重新输入初始条件
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                window.close(); // 关闭窗口以便在控制台输入
                system = ThreeBodySystem(1.0, 0.01, 800);
                getInitialConditions(system);
                window.create(sf::VideoMode(WIDTH, HEIGHT), "三体问题动画");
                window.setFramerateLimit(60);
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

