# Android_Native_Surface
[此项目基于 Android_Native_Surface](https://github.com/fgsqme/Android_Native_Surface)

## 更改
我在他这个项目的基础上修复了两个bug
<br>
0: 部分机型触摸问题
<br>
这个问题的原因就是部分机型有两个触摸输入设备，但是原版却获取到了另一个虚拟输入设备，我在他的基础上增加了最大触控点检测修复了这个bug
1: 屏幕旋转崩溃
<br>
这个是因为在屏幕旋转后，原本会调用shutdown来清除imgui资源，然后重新创建native窗口，在执行shutdown函数的期间，进程崩了
<br>
我用了一个简陋的方法，就是在创建native window和初始化imgui时，把原本的x改成x + y，y改成 y + x，然后无论屏幕正反都这样，有点简陋，不过能用(doge)
我只是在创建native Window时用了两倍xy，并没有更改displayInfo的值，如果你在其他地方要用到它，仍然正常使用
<br>
最后，我还弃用了它原本从库里导出判断屏幕正反的函数，重新写了一套逻辑。
因为我本来以为旋转屏幕崩溃是它的原因。。。。
注::请把wz2.jpg放到内部存储根目录，如果对我写的这两个控件有兴趣可以去看我另一个imgui项目的widget类(doge