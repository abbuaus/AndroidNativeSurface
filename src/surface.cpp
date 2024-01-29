#include <draw.h>
#include <touch.h>
#include "tools.h"
#include "widget.h"


int main(int argc, char *argv[])
{
        new std::thread(screen_config);
        // 初始化imgui
        sleep(2);
        initDraw(true);
        Init_touch_config();
        bool flag = true;
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiStyle& style = ImGui::GetStyle();

        int my_image_width = 0;
        int my_image_height = 0;

        GLuint my_texture4 = 0;

        bool ret2 = LoadTextureFromFile("/storage/emulated/0/wz2.jpg", &my_texture4, &my_image_width, &my_image_height);
        IM_ASSERT(ret);
        IM_ASSERT(ret2);

        while (flag)
        {
            //////////////////////////////////定义区
            static widget test{};
            static int menutap{1};
            static int style_idx{};
            static bool CheckBoxtest{};
            //////////////////////////////////定义区结束
            drawBegin();

            //ImGui::Begin("by,,muchen",NULL,ImGuiWindowFlags_NoResize);
            ImGui::Begin("by,,muchen");
            ImGui::SetWindowSize({1000,700},ImGuiCond_Once);
            ImGui::BeginChild("菜单", ImVec2(300,0),true);
            if (test.ImageButton("主菜单", my_texture4, ImVec2(250, 150)))
            {
                menutap = 1;
            }
            if (test.ImageButton("其他", my_texture4, ImVec2(250, 150)))
            {
                menutap = 2;
            }
            if (test.ImageButton("设置", my_texture4, ImVec2(250, 150)))
            {
                menutap = 3;
            }

            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("内容", ImVec2(0, 0), true);
            switch (menutap)
            {
                case 1:
                    ImGui::Text("fps ： (%.3f FPS)", io.Framerate);
                    ImGui::Text("屏幕方向 %d",displayInfo.orientation);
                    ImGui::Text("分辨率 x:%d y:%d",displayInfo.width,displayInfo.height);
                    if (ImGui::Button("退出"))
                    {
                        flag = false;
                    }
                    break;
                case 2:
                    test.CheckBox("测试", &CheckBoxtest);
                    break;
                case 3:
                    if (ImGui::Combo("颜色", &style_idx, "白色\0蓝色\0紫色\0"))
                    {
                        switch (style_idx)
                        {
                            case 0:
                                ImGui::StyleColorsLight();
                                style.Colors[ImGuiCol_Header] = ImVec4(36/255, 54/255, 74/255, 79/255);
                                break;
                            case 1:
                                ImGui::StyleColorsDark();
                                break;
                            case 2:
                                ImGui::StyleColorsClassic();
                                break;
                        }
                    }
                    ImGui::ColorEdit3("字体颜色", (float*)&style.Colors[0]);
                    break;
            }

            ImGui::EndChild();
            ImGui::End();
            drawEnd();
            std::this_thread::sleep_for(1ms);
        }
        touchEnd();
    return 0;
}

