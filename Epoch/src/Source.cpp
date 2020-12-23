#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

//窗口大小改变时的回调函数（让视口(viewport)同步调整）
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//输入处理函数
void processInput(GLFWwindow* window);

int main()
{
  //初始化GLFW,并进行相关配置
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//主版本号
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//从版本号
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//模式

  //创建窗口对象
  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
  if (NULL == window)
  {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();	//清除所有window和cursor1
	return -1;
  }
  //关联窗口上下文
  glfwMakeContextCurrent(window);
  //注册回调函数，这样窗口size改变的时候，视口size也改变，创建窗口时也会调用回调函数
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //初始化GLAD（管理opengl函数指针）
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
  }
  //渲染视口
  glViewport(0, 0, 800, 600);	//X,Y控制窗口左下角的位置，但视口也可比窗口小，这样可以考虑将来在窗口中显示多个信息

  //循环渲染，避免主动关闭
  while (!glfwWindowShouldClose(window))
  {
	//输入
	processInput(window);

	/*
	 * 渲染
	 * ...
	*/
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);	//设置清空状态，即清空屏幕后的颜色，参数为三基色家阿尔法值
	glClear(GL_COLOR_BUFFER_BIT);	//使用清空状态，即清空表屏幕

	//检查事件
	glfwPollEvents();	//检测IO事件,窗口更新，并调用其回调函数
	glfwSwapBuffers(window);	//交换缓冲（双缓冲）
  }

  glfwTerminate();	//释放所有分配的资源
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  //判断Esc是否按下，设置渲染循环结束条件
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
}