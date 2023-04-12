/*在此示例中，我创建了两个物体，一个向右运动，一个向左运动，
并将它们添加到了物理引擎中进行模拟。每一次循环都会更新物理引擎并输出两个物体的位置。
如果发现两个物体发生了碰撞，则停止运动，并退出循环。
这是一个非简单的示例程序，
#include <iostream>
#include "Vector2.h"
#include "GameObject.h"
#include "PhysicsEngine.h"

using namespace std;

int main() {
    // 创建物体1
    GameObject obj1;
    obj1.position = Vector2(0, 0);
    obj1.velocity = Vector2(10, 0);
    obj1.acceleration = Vector2(0, 0);
    obj1.mass = 1.0f;
    obj1.width = 1;
    obj1.height = 1;

    // 创建物体2
    GameObject obj2;
    obj2.position = Vector2(100, 0);
    obj2.velocity = Vector2(-10, 0);
    obj2.acceleration = Vector2(0, 0);
    obj2.mass = 1.0f;
    obj2.width = 1;
    obj2.height = 1;

    // 创建物理引擎并添加物体
    PhysicsEngine engine;
    engine.AddObject(&obj1);
    engine.AddObject(&obj2);

    // 模拟物理引擎运行过程
    for (int i = 0; i < 100; i++) {
        engine.update(0.1f);

        // 输出物体的位置
        cout << "Object 1: (" << obj1.position.x << ", " << obj1.position.y << ")" << endl;
        cout << "Object 2: (" << obj2.position.x << ", " << obj2.position.y << ")" << endl;

        // 检查是否碰撞
        if (engine.checkCollision(&obj1, &obj2)) {
            // 处理碰撞后的停止运动
            obj1.velocity = Vector2(0, 0);
            obj1.acceleration = Vector2(0, 0);
            obj2.velocity = Vector2(0, 0);
            obj2.acceleration = Vector2(0, 0);
            break;
        }
    }

    return 0;
}
*/
