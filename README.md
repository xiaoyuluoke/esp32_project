# esp32_project
+/********************************************************************************
+**:ESP32_project                                                                                                                         
+**: 作者：李波                                                                                                                              
+**: 目的：建立ESP32工程树结构                                                                                                              
+**:修改时间:2017、09、10                                                                                                                   
+**:修改内容:Makefile文件、project_gatts.mk文件、                                                                                               
+**:项目集成：蓝牙的gatts_server、wifi配置                                                                                                                                                                         
+*********************************************************************************/                                                                         
+乐鑫官方提供的demo，供用户选择的程序结构只有一个main主函数文件夹，这样的结构在做demo的时候是合适的\                                                       
+在进行项目工程的时候，就不太适合了，本项目目的在于建立一套类似在Keil里开发STM32一样的工程树结构\                                                          
+将项目进行模块化设计，                                                                                                                              
+第一步：将ESP-IDF中的demo复制一份改名为自己工程的名字，我将/esp-idf/examples/bluetooth/gatt_server\                                                        
+  复制修改成esp32_project                                                                                                                         
+第二步：将ESP-IDF中的make文件夹中project.mk文件复制一份到工程目录下esp32_project目录下                                                                     
+第三步：我用mv指令将project.mk文件改名为 project_gatts.mk                                                                                          
+第四步：修改Makefile文件，
    1.工程名字：PROJECT_NAME := gatt_server_ 
    2.添加一条指令,得到当前目录路径：mypath := $(shell pwd )  \                            
+  修改Makefile文件中需要包含的.mk文件的文件路径:include  $(mypath)/project_gatts.mk，这样，就可以通过修改.mk文件控制工程变异链接规则                 
+第五步：在bobo_esp32目录下mkdir一个gatts文件夹，将gatts_demo.c移植修改成gatts_server.c和gatts_server.h模块，移动到gatts目录下，\                          
+  并复制一份component.mk文件到该目录下                                                                                                           
+第六步：修改project_gatts.mk文件，
+大约在100行下BUILD_DIR_BASE := $(PROJECT_PATH)/build 这一句指定了编译后的文件路径                                                                        
+我将其修改成                                                                                                                                             
+BUILD_DIR_BASE := $(PROJECT_PATH)/output                                                                                                     
+指定输出目录为output                                                                                                                              
+大约在108行下COMPONENT_DIRS := $(PROJECT_PATH)/components $(EXTRA_COMPONENT_DIRS) $(IDF_PATH)/components $(PROJECT_PATH)/main,
  这一句这里指定了源文件的路径 ，我将其修改成如下语句                                                  

+mp :=$(PROJECT_PATH)                                                                                                                                                                                                                                                                                       
+EXTRA_COMPONENT_DIRS :=$(mp)/Config $(mp)/data_handle $(mp)/gatts $(mp)/wifi                                                                       
+COMPONENT_DIRS := $(PROJECT_PATH)/components $(EXTRA_COMPONENT_DIRS) $(IDF_PATH)/components $(PROJECT_PATH)/main                                             
+                                                                                                                                                     
+大约在158行左右，COMPONENT_INCLUDES :=  这一句指定了包含的.h头文件的路径                                                                                
+修改成如下：                                                                                                                                     
+mp :=$(PROJECT_PATH)                                                                                                                               
+my_inc_DIRS :=$(mp)/Config $(mp)/data_handle $(mp)/gatts $(mp)/wifi                                                                                      
+COMPONENT_INCLUDES :=$(my_inc_DIRS)                                                                                                            
+                                                                                                                                                     
+好了就可以像在keil下那样建立工程树结构了。                                                                                                            
+第七步:在main文件夹下新建main.c文件，将前面的gatts_server.h文件加进来，编写main.c文件                                                                       
+第八步:make                                                                                                                                           
+在这个工程中我不仅仅设定了蓝牙gatts模块，还设定了WiFi配置模块                                                                         
                                                                                                                                                      
