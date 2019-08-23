// http://cegui.org.uk/wiki/Using_CEGUI_with_GLUT

#include <iostream>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace CEGUI;

static GLFWwindow *window;

const int win_w = 640;
const int win_h = 480;

MouseButton toCEGUIButton(int button)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        return LeftButton;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return MiddleButton;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return RightButton;
    default:
        return MouseButtonCount;
    }
}

void charCallback(GLFWwindow *window, unsigned int char_pressed)
{
    System::getSingleton().getDefaultGUIContext().injectChar(char_pressed);
}

void cursorPosCallback(GLFWwindow *window, double x, double y)
{
    System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y);
}

static void keyCallback(GLFWwindow *window, int key, int scan, int action, int mod) 
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod)
{
    if (state == GLFW_PRESS) {
        System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
            toCEGUIButton(button));
    } else {
        System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
            toCEGUIButton(button));
    }
}

void mouseWheelCallback(GLFWwindow *window, double x, double y)
{
    if (y < 0.f) {
        System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(
            -1.f);
    } else {
        System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(
            +1.f);
    }
}

void windowResizedCallback(GLFWwindow *window, int width, int height)
{
    System::getSingleton().notifyDisplaySizeChanged(
        Sizef(static_cast<float>(width), static_cast<float>(height)));
    glViewport(0, 0, width, height);
}

void errorCallback(int error, const char *message)
{
    Logger::getSingleton().logEvent(message, Errors);
}

void setupCallbacks()
{
    // 输入回调
    glfwSetCharCallback(window, charCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseWheelCallback);

    // 窗口回调
    glfwSetWindowSizeCallback(window, windowResizedCallback);

    // 错误回调
    glfwSetErrorCallback(errorCallback);
}

void initGLFW()
{
    // glfw库初始化
    if (!glfwInit()) {
        std::cerr << "glfw could not be initialized!" << std::endl;
        exit(1);
    }

    // 创建窗口和OpenGL设备上下文
    window = glfwCreateWindow(win_w, win_h, "Hello World", NULL, NULL);
    if (!window) {
        std::cerr << "Could not create glfw window!" << std::endl;
        glfwTerminate();
        exit(1);
    }

    // 窗口设备指定为当前
    glfwMakeContextCurrent(window);

    // 隐藏系统鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // 禁用VSYNC
    glfwSwapInterval(0);

    // 清除错误信息
    glGetError();
}

void initCEGUI()
{
    // 创建渲染器并打开附加状态
    OpenGLRenderer &myRenderer =
        CEGUI::OpenGLRenderer::bootstrapSystem();
    myRenderer.enableExtraStateSettings(true);

    // 为默认资源组初始化需要的目录
    DefaultResourceProvider *rp = static_cast<DefaultResourceProvider *>(
        System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");
    rp->setResourceGroupDirectory("imagesets", "datafiles/imagesets/");
    rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");
    rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");
    rp->setResourceGroupDirectory("looknfeels",
                                  "datafiles/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts",
                                  "datafiles/lua_scripts/");
    rp->setResourceGroupDirectory("schemas", "datafiles/xml_schemas/");

    // 使用默认资源组
    ImageManager::setImagesetDefaultResourceGroup("imagesets");
    Font::setDefaultResourceGroup("fonts");
    Scheme::setDefaultResourceGroup("schemes");
    WidgetLookManager::setDefaultResourceGroup("looknfeels");
    WindowManager::setDefaultResourceGroup("layouts");
    ScriptModule::setDefaultResourceGroup("lua_scripts");

    XMLParser *parse = System::getSingleton().getXMLParser();
    if (parse->isPropertyPresent("SchemaDefaultResourceGroup"))
        parse->setProperty("SchemaDefaultResourceGroup", "schemas");

    // 加载TaharezLook清单和字体
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme",
                                                 "schemes");
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

    // 设置默认字体、鼠标和提示
    System::getSingleton().getDefaultGUIContext().setDefaultFont(
        "DejaVuSans-10");
    System::getSingleton()
        .getDefaultGUIContext()
        .getMouseCursor()
        .setDefaultImage("TaharezLook/MouseArrow");
    System::getSingleton().getDefaultGUIContext().setDefaultTooltipType(
        "TaharezLook/Tooltip");
}

void initWindows()
{
    WindowManager &wmgr = WindowManager::getSingleton();

    // root是铺满整个窗口的，它是一个空窗口(全透明)
    Window *myRoot = wmgr.createWindow("DefaultWindow", "root");
    System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

    FrameWindow *fWnd = static_cast<FrameWindow *>(
        wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));
    myRoot->addChild(fWnd);

    // 窗口位置(窗口左上角位于父窗口的位置)
    // UDim(a,b)
    // a - 相对量 b - 绝对量
    // 本例子中窗口宽度为640
    // UVector2(UDim(0.0f, 10.0f), UDim(0.0f, 0.0f)) - 窗口左上角位于(10,0)
    // UVector2(UDim(0.5f, 10.0f), UDim(0.0f, 0.0f)) -
    // 窗口左上角位于(640*0.5+10,0)
    fWnd->setPosition(UVector2(UDim(0.25f, 0.0f), UDim(0.25f, 0.0f)));
    // 窗口大小
    fWnd->setSize(USize(UDim(0.5f, 0.0f), UDim(0.5f, 0.0f)));
    // fWnd->setMaxSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));

    // 标题
    fWnd->setText("Hello World!");
}

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    // 初始化glfw
    initGLFW();

    // 初始化cegui
    initCEGUI();

    // 设置glfw回调
    setupCallbacks();

    // 将窗口尺寸通知系统
    System::getSingleton().notifyDisplaySizeChanged(Sizef(win_w, win_h));
    glViewport(0, 0, win_w, win_h);

    // 初始化窗口和布局
    initWindows();

    // 设定清理颜色
    glClearColor(0, 0, 0, 255);

    double time = glfwGetTime();

    OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(System::getSingleton().getRenderer());
    // 消息循环
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // 清除屏幕
        glClear(GL_COLOR_BUFFER_BIT);

        // 注入定时器
        //const float newtime = glfwGetTime();
        //const float time_elapsed = newtime - time;
        //System::getSingleton().injectTimePulse(time_elapsed);
        //System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);
        //time = newtime;

        // 渲染GUI
        renderer->beginRendering();
        System::getSingleton().renderAllGUIContexts();
        renderer->endRendering();

        // 交换缓冲
        glfwSwapBuffers(window);

        // 处理事件
        glfwPollEvents();
    }

    // 清理
    System::destroy();
    OpenGLRenderer::destroy(*renderer);
    renderer = nullptr;

    // 销毁窗口
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}



