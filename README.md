# BinInCpp
Generate any binary file's .h&.cpp.  

## Usage
example:  
 ```./BinInCpp.exe 7z.exe```  
This will read ```7z.exe``` and create both 7z.h and 7z.cpp.  
Include ```7z.h``` and compile ```7z.cpp``` in your project.  
Call  ```release_7z()``` to release origin 7z.exe to you word folder.  
It is highly command to lib the cpp file to invoid IDE being slow.  

## Warning 
**Due to stack limit,os might raise stack overflow when the embeded file is too large.**  
**Better not to embed file that is larger than 10M.**  
**Also if the size of embeded file is large than the main executable looks strange**  
**and always not a good idea.**  



# 说明
为二进制文件生成 .h和.cpp 文件，用于集成到c++程序中。  
**由于操作系统有栈限制，需要集成的文件大小不应该大于10M。**  