#include_directories(../src)

# 指定生成目标
add_executable(helloworld main.cpp)
if (WIN32)
    set_property (TARGET helloworld PROPERTY WIN32_EXECUTABLE TRUE)
endif () 

# 添加链接库
target_link_libraries(helloworld glfw3 glfw3dll opengl32 glew CEGUIBase-0 CEGUIOpenGLRenderer-0)