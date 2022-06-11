# 着色器

## 顶点属性：每个顶点着色器的输入vec4

### 顶点属性需要特殊的表示

```glsl
layout(location=0)
```

## uniform 关键字让变量成为全局变量

```glsl
uniform vec4 fragColor
```

## 在程序中设置uniform必须线glUseProgram

```glsl
    // 记得激活着色器
    glUseProgram(shaderProgram);

    // 更新uniform颜色
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

## glEnableVertexArray(location的值)

```glsl
glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
```

# 纹理

## 纹理坐标在x和y轴上，范围在0到1之间使用纹理坐标获取纹理颜色叫做采样(Sampling)。纹理坐标起始于(0, 0)，也就是纹理图片的左下角，终始于(1, 1)，即纹理图片的右上角。下面的图片展示了我们是如何把纹理坐标映射到三角形上的。

# 但图片的0位置一般在照片的顶部，所以图片要变换

![image-20211006165720323](C:\Users\SunHe\AppData\Roaming\Typora\typora-user-images\image-20211006165720323.png)

## 我们为三角形指定了3个纹理坐标点。如上图所示，我们希望三角形的左下角对应纹理的左下角，因此我们把三角形左下角顶点的纹理坐标设置为(0, 0)；三角形的上顶点对应于图片的上中位置所以我们把它的纹理坐标设置为(0.5, 1.0)；同理右下方的顶点设置为(1, 0)。我们只要给顶点着色器传递这三个纹理坐标就行了，接下来它们会被传片段着色器中，它会为每个片段进行纹理坐标的插值。

```glsl
float texCoords[] = {
    0.0f, 0.0f, // 左下角
    1.0f, 0.0f, // 右下角
    0.5f, 1.0f // 上中
};
```

# glbind的之后必须直接赋完值，才可以创建下一个，再glbind，不然第二个glbind就直接绑定到第一块内存而不是第二块

# 变换需要缩放后旋转最后位移

# 顶点着色器运作过程

## 局部坐标转换为世界坐标 model matrix

## 世界坐标转换为观察坐标 view matrix

## 观察坐标转换为裁剪坐标 projection matrix

## 得到的裁剪坐标会最终进行透视除法，转换为标准的3D坐标

# 摄像机

## lookat矩阵，创建一个看向目标位置的矩阵

# 鼠标输入

+ @param[in] window The window that received the event.
+ @param[in] xpos The new cursor x-coordinate, relative to the left edge of the content area.
+ @param[in] ypos The new cursor y-coordinate, relative to the top edge of the content area.



# 颜色相乘，颜色值即为反射的值

# In glm we access elements as mat[col][row] due to column-major layout

# 光照最少由环境光照、漫反射光照和镜面光照三种分量组成

# 法向量是垂直于改顶点所在的三角形中

# 环境光，当场景为黑的时候也能存在一些亮度，用一个较小的常数乘光的颜色表示

# 所有的光照都在世界坐标进行的计算的，所以计算漫反射的光强时，需要片段的坐标位置，得是model*vertices后的坐标

# struct material定义了不同分量反射的颜色

```glsl
struct Material{
    vec3 ambient;//定义为物体的颜色
    vec3 diffuse;//定义为物体的颜色
    vec3 specular;
    float shininess;
};
```

# struct Light 定义为光的颜色，当三个数值一样，只是强度变了，变为暗光用来表示光的强度intensity*vec3（1.0f）

```glsl
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```

# 相当于lightColor * light.ambient  * material.ambient,lightColor =vec3(1.0f),所以忽略

# main中不要用output命名，会产生问题

# C++结构体是连续储存的，就是结构体初始化后，在内存中是连续储存的

![image-20211028225819382](C:\Users\SunHe\AppData\Roaming\Typora\typora-user-images\image-20211028225819382.png)

# 深度缓冲将z值转换为0-1，有线性变换以及非线性变换

# 每个片段处理完后，先进行模板测试（模板值为1才被渲染），被保留的片段进行深度测试。未被渲染的位置模板值为0

# 混合：颜色的混合

## 当alpha值为0.5时，物体的颜色有50%是来自物体自身的颜色，50%来自背后物体的颜色

## 半透明的绿色在红色之上，红色为目标颜色（先在颜色缓冲中），绿色为原颜色，alpha为原颜色因子

## 在着色器中对alpha值小于0.1的进行discard，取消透明背景，防止白色遮挡后面，边框用GL_CLAMP_TO_EDGE让边框不像GL_REPEAT一样，出现白色边框

## <font color="red">混合与深度检测</font>:深度检测会导致前面的先绘制，后面的后绘制，但是对于混合需要先知道后面的颜色值，所以需要按照先远后近的方式绘制半透明物体

# 面剔除

```glsl
glEnable(GL_CULL_FACE);//启用面剔除
glCullFace(GL_BACK);//剔除背向面
glFrontFace(GL_CW);//定义顺时针为正向面，默认逆时针为正向面
```

# 延迟渲染：帧缓冲：缓冲到一张纹理，屏幕渲染是一个图片

# Blinn-Phong ： 当前视线与入射光的夹角一半向量与法线向量的夹角，当夹角越小，反射光越强

# 冯氏模型测量的是观察方向与反射向量间的夹角，一般比Blinn-Phong夹角大，所以Blinn-Phong使用的指数应该更大

![](Picture\advanced_lighting_halfway_vector.png)

# gamma矫正，图片的数值记为pow（value， 1 / 2.2），阴极管再pow（value， 2.2），即可还原本身颜色，<font color="red">由于需要尽量节省空间，并让亮度平滑过渡，采用gamma编码让颜色过度更加流畅，符合人眼颜色规律，1/2.2,巧合的是让颜色乘pow（2.2）还原成真实颜色</font> sRGB空间（1/2.2）
