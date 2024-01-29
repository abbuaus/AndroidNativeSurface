#ifndef WIDGET_H
#define WIDGET_H
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
class widget
{
public:
    widget();
    bool ImageButton(std::string text, ImTextureID texture, ImVec2 size = ImVec2(0, 0));// ImageButton方法创建一个带有文本的图像按钮
    void CheckBox(std::string text, bool* b);// Checkbox函数创建一个复选框。参数b是一个指向bool的指针，用于存储复选框的状态，注意：调用者负责管理b指向的内存
private:
    ImGuiStyle& style;
};

#endif
