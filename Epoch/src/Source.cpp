#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

//���ڴ�С�ı�ʱ�Ļص����������ӿ�(viewport)ͬ��������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//���봦����
void processInput(GLFWwindow* window);

int main()
{
  //��ʼ��GLFW,�������������
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//���汾��
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//�Ӱ汾��
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//ģʽ

  //�������ڶ���
  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
  if (NULL == window)
  {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();	//�������window��cursor1
	return -1;
  }
  //��������������
  glfwMakeContextCurrent(window);
  //ע��ص���������������size�ı��ʱ���ӿ�sizeҲ�ı䣬��������ʱҲ����ûص�����
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //��ʼ��GLAD������opengl����ָ�룩
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
  }
  //��Ⱦ�ӿ�
  glViewport(0, 0, 800, 600);	//X,Y���ƴ������½ǵ�λ�ã����ӿ�Ҳ�ɱȴ���С���������Կ��ǽ����ڴ�������ʾ�����Ϣ

  //ѭ����Ⱦ�����������ر�
  while (!glfwWindowShouldClose(window))
  {
	//����
	processInput(window);

	/*
	 * ��Ⱦ
	 * ...
	*/
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);	//�������״̬���������Ļ�����ɫ������Ϊ����ɫ�Ұ�����ֵ
	glClear(GL_COLOR_BUFFER_BIT);	//ʹ�����״̬������ձ���Ļ

	//����¼�
	glfwPollEvents();	//���IO�¼�,���ڸ��£���������ص�����
	glfwSwapBuffers(window);	//�������壨˫���壩
  }

  glfwTerminate();	//�ͷ����з������Դ
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  //�ж�Esc�Ƿ��£�������Ⱦѭ����������
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
}