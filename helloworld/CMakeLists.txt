#include_directories(../src)

# 指定生成目标
add_executable(helloworld main.cpp)

# 添加链接库
target_link_libraries(helloworld freeglut glew CEGUIBase-0 CEGUIOpenGLRenderer-0)