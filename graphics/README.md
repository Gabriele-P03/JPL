# **Utils (CM)**
<h2><i>Utils Function</i></h2>
<br>
This module provides you several useful function which are used
through all JPL's module.
It is splitted into headers file which can be considered as 
sub-modules
<br><br>
This module may be seen even as a SDM (Self-Dependency Module) since some of its headers include some others of this same module.
<br><br>
Since all its headers can be considered as sub-modules, you can download those ones you need and discard all other ones (unless they are not included by those ones you need). 
<br>
<br>

## List of Sub-Modules:
> <span style="color:yellow">BitsUtils</span> 
>> To operate on bits (boolean array)

> <span style="color:yellow">CharUtils</span> 
>> To manage chars array

> <span style="color:yellow">ConversionUtils</span> 
>> To perform conversion between data-type

> <span style="color:yellow">FilesUtils</span> 
>> To reading/getting file and path<br><br>

> <span style="color:yellow">DebugUtils</span> 
>> To provides some function whilst debugger is attached or about stacktrace and errors occurred<br>
>> In order to get a more readable stacktrace, everytime that either a shared or static object is made up by yourself, you should always build them with debug symbols<br><br>
>> e.g.<br>
>> g++ -g <--debug symbol -fPIC -c -o test.out test.cpp<br>
>> g++ -shared -o libtest.so test.out<br>
>> <h2>For windows machine only:</h2>
>> <h5>In order to print a more detailed stacktrace, you should extract .pdb (which contains symbols, also used by gdb) file from the compiled exe. I use <a href="https://github.com/rainers/cv2pdb">cv2pdb</a> which generate a file named as the executable one. Put it in mind that, once invoked cv2pdb (via CLI is "cv2pdb64.exe your_executable_file.exe"), all symbols are removed from the .exe and then, if you invoke gdb on it, it will not encounter any breakpoints.</h5><br><br>

> <span style="color:yellow">StructureUtils</span> 
>> To provide several Data-Structure<br><br>

> <span style="color:yellow">FunctionalUtils</span> 
>> To provide funtional interfaces which can be used through StructureUtils