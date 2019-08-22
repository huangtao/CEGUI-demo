// http://cegui.org.uk/wiki/Using_CEGUI_with_GLUT

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <GL/freeglut.h>
#include <GL/glew.h>

using namespace std;

int window_id;
bool keep_running = true;

void render(void);
void keyFunc(unsigned char key, int x, int y);
void resource_group();
void load_data_files();
void create_window();

int main(int argc, char **argv)
{
    // 创建OpenGL窗口
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    window_id = glutCreateWindow("Hello World");

    // 回调函数
    glutKeyboardFunc(keyFunc);
    glutDisplayFunc(render);

    GLenum err = glewInit();

    // 初始化CEGUI
    CEGUI::OpenGLRenderer &myRenderer =
        CEGUI::OpenGLRenderer::bootstrapSystem();
    // 或使用下面两句
    // CEGUI::OpenGLRenderer& myRenderer = CEGUI::OpenGLRenderer::create();
    // CEGUI::System::create(myRenderer);

    // 资源设定
    resource_group();
    load_data_files();
    create_window();

    // 消息循环
    while (keep_running) {
        glutMainLoopEvent();
        render();

        // 渲染GUI
        CEGUI::System::getSingleton().renderAllGUIContexts();

        glutSwapBuffers();
    }

    // 清理
    CEGUI::System::destroy();
    CEGUI::OpenGLRenderer::destroy(myRenderer);
    glutDestroyWindow(window_id);

    return 0;
}

// 渲染函数
void render()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutWireTeapot(0.5);
}

// 按键响应
void keyFunc(unsigned char key, int x, int y)
{
    switch (key) {
    case 113: // 'q'
    case 81:  // 'Q'
    case 27:  // 'ESC'
        keep_running = false;
        break;
    }
}

void resource_group()
{
    // 为默认资源组初始化需要的目录
    CEGUI::DefaultResourceProvider *rp =
        static_cast<CEGUI::DefaultResourceProvider *>(
            CEGUI::System::getSingleton().getResourceProvider());

    rp->setResourceGroupDirectory("schemes", "../../../datafiles/schemes/");
    rp->setResourceGroupDirectory("imagesets", "../../../datafiles/imagesets/");
    rp->setResourceGroupDirectory("fonts", "../../../datafiles/fonts/");
    rp->setResourceGroupDirectory("layouts", "../../../datafiles/layouts/");
    rp->setResourceGroupDirectory("looknfeels",
                                  "../../../datafiles/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts",
                                  "../../../datafiles/lua_scripts/");

    // 使用默认资源组
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
}

// 加载资源
void load_data_files()
{
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
}

void create_window()
{
    using namespace CEGUI;
    WindowManager &wmgr = WindowManager::getSingleton();

    Window *myRoot = wmgr.createWindow("DefaultWindow", "root");
    System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

    FrameWindow *fWnd = static_cast<FrameWindow *>(
        wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));
    myRoot->addChild(fWnd);

    // 窗口位置和大小
    fWnd->setPosition(UVector2(UDim(0.25f, 0.0f), UDim(0.25f, 0.0f)));
    fWnd->setSize(USize(UDim(0.5f, 0.0f), UDim(0.5f, 0.0f)));

    // 标题
    fWnd->setText("Hello World!");
}