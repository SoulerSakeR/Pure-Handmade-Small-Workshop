# Class Texture2D
纹理类，读取图片后创建的纹理数据
## field
```
Color32[][] data //图片数据，颜色
const int width //图片宽度(px)
const int height //图片高度(px)
```
## constructor
``` 
Texture2D(int width,int height) //创建空纹理
```
## method
```
void setPixel(int x,int y)
```
# Struct Color32
颜色结构体
## field
```
float r
float g
float b
float a
```
# Class TextAsset
## field
```
byte[] data //原始数据
string text //文本字符串
```

# Class GameObject
游戏对象基类
## field
```
bool isActive //当前游戏对象的激活状态
string name //名字
int id //唯一id
Transform transform //游戏对象的transform组件
```
## method
```
void setActive(bool value) //设置对象的激活状态
T getComponent<T>() //获取当前对象下挂载的指定组件
```

# Class Component
组件基类
## field
```
GameObject gameObject //组件挂载的游戏对象
```

# Class Camera:Componet
渲染时会根据照相机的位置、视野范围、缩放等级，确定需要渲染的游戏世界区域，将其渲染到窗口中。
## field
```
float width //视野宽度
float height //视野高度
float zoomLevel //缩放等级
float maxZoomLevel //最大缩放等级
float minZoomLevel //最小缩放等级
``` 
## method
```
void setZoomLevel(float value)
vector2D screenPositionToWorldPosition(vector2D screenPosition)
```

# Struct Vector2D
坐标结构体
## field
```
float x 坐标X
float y 坐标Y
```
## constructor
```
vector2D(float x, float y)
```
## method
```
Vector2D zero()
```

# Class Transform:Component
控制游戏对象位置，旋转的组件，每一个游戏对象都应该有一个Transform
## field
```
Vector2D localPosition //本地坐标位置
Vector3D localRotation //本地旋转角度
vector<Transform> children //子节点
Transform parent //父节点
```
## method
```
Vector2D getWorldPosition() //获取世界坐标位置
Vector3D getWorldRotation() //获取世界渲染角度
Transform translate(vector2D value) //朝给定向量移动
```

# Class Image:Component
图片组件
## field
```
Texture src //纹理
Color32 color //颜色
```

# Class Renderer:Component
## method
```
void render() //找到当前游戏对象中的所有需要渲染的对象并将其传给渲染模块进行渲染
```
