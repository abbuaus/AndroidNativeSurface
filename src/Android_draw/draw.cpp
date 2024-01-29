//
// Created by 泓清 on 2022/8/26.
//

#include "Android_draw/draw.h"
#include "Font.h"
// Var
EGLDisplay display = EGL_NO_DISPLAY;
EGLConfig config;
EGLSurface surface = EGL_NO_SURFACE;
ANativeWindow *native_window;
ExternFunction externFunction;
EGLContext context = EGL_NO_CONTEXT;
MDisplayInfo displayInfo;
bool g_Initialized = false;

bool initDraw(bool log)
{
    return initDraw(displayInfo.width, displayInfo.height, log);
}

bool initDraw(uint32_t _screen_x, uint32_t _screen_y, bool log)
{
    if (!init_egl(_screen_x + _screen_y, _screen_y + _screen_x, log))//两倍的xy
    {
        return false;
    }
    return ImGui_init();
}

bool init_egl(uint32_t _screen_x, uint32_t _screen_y, bool log)
{
    native_window = externFunction.createNativeWindow("Ssage",_screen_x, _screen_y, false);//创建native_surface,最后一个值表示是否打印作者信息
    ANativeWindow_acquire(native_window);//注册
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);//获取默认显示屏
    if (display == EGL_NO_DISPLAY)
    {
        return false;
    }
    if (eglInitialize(display, 0, 0) != EGL_TRUE)//初始化
    {
        return false;
    }
    EGLint num_config = 0;
    const EGLint attribList[] =
            {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 5,   //-->delete
            EGL_GREEN_SIZE, 6,  //-->delete
            EGL_RED_SIZE, 5,    //-->delete
            EGL_BUFFER_SIZE, 32,  //-->new field
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };//EGL属性
    const EGLint attrib_list[] =
            {
            EGL_CONTEXT_CLIENT_VERSION,
            3,
            EGL_NONE
    };//属性
    if (eglChooseConfig(display, attribList, &config, 1, &num_config) != EGL_TRUE)//写入配置,并保存在config中
    {
        return false;
    }
    EGLint egl_format;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &egl_format);//是获取选定配置的本机可视化ID
    ANativeWindow_setBuffersGeometry(native_window, 0, 0, egl_format);//ANativeWindow_setBuffersGeometry 函数是用于设置本机窗口（native_window）的缓冲区几何形状的函数，0表示默认
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);//创建一个 EGL 上下文
    if (context == EGL_NO_CONTEXT)
    {
        return false;
    }
    surface = eglCreateWindowSurface(display, config, native_window, nullptr);//创建surface
    if (surface == EGL_NO_SURFACE)
    {
        return false;
    }
    if (!eglMakeCurrent(display, surface, surface, context))//关联
    {
        return false;
    }
    return true;
}


std::string exec(std::string command)
{
    char buffer[128];
    string result = "";

    FILE* pipe = popen(command.c_str(), "r");
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != nullptr)
        {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}

void screen_config()
{
    std::string window_size = exec("wm size");
    int screen_x,screen_y;
    sscanf(window_size.c_str(),"Physical size: %dx%d",&screen_x, &screen_y);
    while(true)
    {
        displayInfo.orientation = atoi(exec("dumpsys display | grep 'mCurrentOrientation' | cut -d'=' -f2").c_str());
        if(displayInfo.orientation == 0 || displayInfo.orientation == 2)
        {
            displayInfo.width = screen_x;
            displayInfo.height = screen_y;
        }
        if(displayInfo.orientation == 1 || displayInfo.orientation == 3)
        {
            displayInfo.width = screen_y;
            displayInfo.height = screen_x;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    //orithread->detach();
    //displayInfo.orientation = atoi(exec("dumpsys display | grep 'mCurrentOrientation' | cut -d'=' -f2").c_str());
    //displayInfo = MDisplayInfo{2560,1600,1};
    //std::cout<<"初始化"<<std::endl;
    //displayInfo = MDisplayInfo{1600,2560,0};
    //displayInfo = externFunction.getDisplayInfo();
    //displayInfo.orientation = atoi(exec("dumpsys display | grep 'mCurrentOrientation' | cut -d'=' -f2").c_str());
}

bool ImGui_init()
{
    if (g_Initialized)//如果已经初始化就直接返回
    {
        return true;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    ImGui_ImplAndroid_Init(native_window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 60.0f, &font_cfg, io.Fonts->GetGlyphRangesChineseFull());
    style.ScaleAllSizes(3.0f);
    style.FrameRounding = 12;
    ImGui::StyleColorsLight();
    style.Colors[ImGuiCol_Header] = ImVec4(36/255, 54/255, 74/255, 79/255);
    g_Initialized = true;
    return true;
}

void drawBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(displayInfo.width + displayInfo.height, displayInfo.height + displayInfo.width);
    ImGui::NewFrame();
}

void drawEnd()
{
    ImGuiIO &io = ImGui::GetIO();
    glViewport(0.0f, 0.0f, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT
    glFlush();
    if (display == EGL_NO_DISPLAY)
    {
        return;
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(display, surface);
}
