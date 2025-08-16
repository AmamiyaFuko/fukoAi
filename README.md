# 前言
这是个很粗糙的库，但实现了些基本功能。

主要实现了对http 请求的封装，以及利用cJSON 进行了一些管理。

zhu.c 是这个项目的一个案例代码， 你可以使用makefile 来编译它。

fukoAi.config 是必须要设置的，它利用了预处理器进行配置。

如果你运行zhu 文件出现了段错误，说明你没有正确配置fukoAi.config

# 说明
比如说struct fukoAi 的response 属性是为了避免fukoAi_easy_MemoryChat()返回野指针的问题，不过我很难说起到了什么效果，因为我在这个函数里还使用了fukoAi_SetChat_SysChat()来设置对话（这里是参考了libcurl的设计），如果cJSON_AddStringToObject()不是复制整个字符串的话，就会出现问题，不过幸运的是它最终是可以运行的。

这个结构体的messagesSize 属性则是为了fukoAo_easy_NoMemoryChat() 设计的，很难说这种为了某个功能而去修改结构体的做法有什么好处，也许我应该在这里多使用cJSON的方法

对于智能体的初始化采用了fukoAi_config 的方法，这是为了可以复用设置而采用的设计，不过改成传入json也许会好些，这样就变成了类似oop的设计

将智能体的设定设计成从文件读取也许会好些

--2025.8.16 Amamiya Fuko.

# 展望
准备对着liboai大抄特抄
