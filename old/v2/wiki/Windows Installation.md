### Windows Installation, Building, and Running Instructions

#### Download freeglut and GLEW

See http://www.cs.uregina.ca/Links/class-info/390AN/WWW/Lab1/GLUT/windows.html

Unzip them and place them wherever you desire.

#### Setting up Visual Studio Project

NOTE: These instructions were written for VS 2015

Once freeglut and GLEW are downloaded as per the above instruction you will want to create
a project in Visual Studio. To do this automatically:

1.  Open VS and select: File -> New -> Project From Existing Code

2.  Select Visual C++ from the dropdown and hit Next

3.  Enter the following options on the subsequent screen
    * Project file location - The path to INSIDE the /src folder
        (ex. D:\Users\Tomasz\Documents\GitHub\mms\src)

    Project name - The name you wish the project to have (ex. MMS)

    Add files to the project from these folder - Leave Default

    File types to add to the project - Leave Default

    Show all files in Solution Explorer - Checked
   
    Hit Next

4.  Make sure 'Use Visual Studio' is checked and under 'Project Type' it says
'Console Application Project'.  Nothing else should be checked

5.  Hit Finish and Visual Studio will assemble the project for you

6.  *Encouraged* - Check 'Show all Files' under the 'Project' menu
    this will make the 'Solution Explorer' pane mirror the directory structure
   
7.  In the 'Solution Explorer' right click the project file 
    (should be the second from the top) and select 'Properties'.

8.  Under C/C++ - General - Additional Include Directories: Add the include 
    folders within the freeglut and GLEW folders you downloaded earlier, along
    with the GL folder within the include folder for GLEW, and the lib folder within /src.
    For me it looks like this:
	
    C:\Users\Kyle\Desktop\openGL\glew-1.11.0\include\GL
	
    C:\Users\Kyle\Documents\GitHub\mms\src\lib
	
    C:\Users\Kyle\Desktop\openGL\glew-1.11.0\include
	
    C:\Users\Kyle\Desktop\openGL\freeglut\include
	
	Click "Apply".
   
9.  Under C/C++ - Output Files - Object File Name: Enter '$(IntDir)/%(RelativeDir)/'. Click "Apply".

10. Under Linker - General - Additional Library Directories: Add the lib folders
    within the freeglut and GLEW folders.  
	For me it looks like this:
	
	C:\Users\Kyle\Desktop\openGL\glew-1.11.0\lib
	
	C:\Users\Kyle\Desktop\openGL\freeglut\lib
	
	Click "Apply".
	
11. Under Linker - Input - Additional Dependencies: Add the following:

	freeglut.lib
	
	glew32.lib
	
	Click "Apply".
	
12. Go to the folder where the glew files are held, glew-1.1x.0 (there are different versions: 1.11 and 1.13 are proven to work).
	Go the lib directory and the following files may or may not be there:
	
	glew32.lib
	
	glew32s.lib
	
	If they are not already in lib, go to Release\Win32 and copy them to the lib directory.
	
13. Copy the freeglut.dll and glew32.dll files to the src folder, they can be found here:

	...\freeglut\bin

	...\glew-1.1x.0\bin\Release\Win32
	
14. In the 'Solution Explorer' right click the project and select 
	Add -> Existing Item.  Select mms\res\parameters.xml.  This allows
	for easy access to the parameters file.

15. To build the project select Build -> Build Solution or just run it and you will
   be prompted if you want to build the changed project

16. To run the program select Debug -> Start Without Debugging

#### Building the Project on Windows
    
* To build the project select Build -> Build Solution or just run it and you will
  be prompted if you want to build the changed project

* To run the program select Debug -> Run Without Debugging. Hit Yes if you want to
  rebuild

* If you need to clean for some reason you can go to Build -> Clean Solution
