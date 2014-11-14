Introduction and Requirements
=============================
This is a small application for simulating MicroMouse algorithms.

The only current way to compile the application on Windows is through
Visual Studio.  You can get a 3 year license for professional (recommended)
through the Microsoft Dreamspark program.

MinGW will not work becuase currently it lacks support for Windows threading.
This option is being explored and will be updated as options become available.

Note that freeglut *must* be installed for this application to work. To install
it on Windows systems, follow the instructions outlined in the document:

    http://www.cs.uregina.ca/Links/class-info/390AN/WWW/Lab1/GLUT/windows.html

    *Note: You do not need to install glew.  You only need to follow the first
           section


Setting up Visual Studio Project (VS)
=====================================
Once freeglut is installed as per the above instruction, create a project in
Visual Studio:

(Note: these instructions will work on VS 2013 Profesional, and likely be simlar to other
versions)

1) Open VS and select: File -> New -> Project From Existing Code

2) Select Visual C++ from the dropdown and hit Next

3) Enter the following options on the subsequent screen

   Project file location - The path to INSIDE the /src folder
        (ex. D:\Users\Tomasz\Documents\GitHub\mms\src)

   Project name - The name you wish the project to have (ex. MMS)

   Add files to the project from these folder - Unimportant

   File types to add to the project - Leave Default

   Show all files in Solution Explorer - Checked

4) In the subsequent screen make sure 'Use Visual Studio' is check
   and under 'Project Type' it says 'Console Application Proejct'.
   Nothing else should be checked

5) Hit Finish and Visual Studio will assemble the project for you

6) *Encouraged* - Check 'Show all Files' under the 'Project' menu
   this will make the 'Solution Explorer' pane mirror the directory structure

7) To build the project select Build -> Build Solution or just run it and you will
   be prompted if you want to build the changed project

8) To run the program select Debug -> Run Without Debugging


Software Components
===================
As it stands now, the simulation consists of two components. The first is the
sim component, which is responsible for carrying out all of the backend
operations necessary for a smooth and useful simulation. The second component
is the algo component, which is responsible for specifying the algorithm that
the mouse should use to actually solve the maze. Thus, any algorithm code should
be placed within the algo directory, preferably in its own directory (so that
things stay organized). See "src/algo/Mack_Ward/FloodFill" for an example of this. 


Running the Simulation
======================
    
- To build the project select Build -> Build Solution or just run it and you will
  be prompted if you want to build the changed project

- To run the program select Debug -> Run Without Debugging. Hit Yes if you want to
  rebuild

During the simulation, the user may either pause/resume or speed-up/slow-down
the mouse speed, as well as quit the application, as follows:

    <space>  ->   pause/resume
     <f/F>   ->   faster
     <s/S>   ->   slower
     <q/Q>   ->   quit


Writing Your Own Algorithms
===========================
As mentioned before, all user-defined algorithms should be placed within the
algo directory, preferably within their own directories. While it is possible
to define a single function for the algorithm, a good strategy is to define a
class that contains the appropriate functions for executing the algorithm. This
will make the algorithm easier to understand, easier to debug, and much more
portable. AlgorithmTemplate ".h" and ".cpp" files have been provided to help
those who may not know the syntax for defining classes in C++. You can simply
copy these, change the approprite identifiers, and provide definitions for the
given solve function, as well as you own helper functions.

Additionally, the algorithm class (that you define) should implement the
IAlgorithm interface (and thus must include a solve() method). This ensures
that the algorithm is compatible with the simulation utilities already defined.

Lastly, in order to actually execute a user-defined algorithm, the
"src/algo/Solver.cpp" must be modified to include your code, and you must call
the appropriate function (most likely the solve function of your own class)
from within the Solver's solve method (just FYI, this is a technique known as
delegation). You can look at the Solver class for some examples of how to do
this.


Available Mouse Commands
========================
For users writing their own algorithms, they should call any one of the
following functions in their code to receive input and generate output (Note
that the commands below assume that the user has a pointer to a MouseInterface
object, and the the variable name for the pointer is "m_mouse"):

    1) m_mouse->wallFront()
    2) m_mouse->wallRight()
    3) m_mouse->wallLeft()
    4) m_mouse->moveForward()
    5) m_mouse->turnRight()
    6) m_mouse->turnLeft()

Functions 1-3 return true or false values correpsonding to whether or not there
is a wall to the front, right, or left of the robot. Functions 4-6 instruct the
robot to move forward, turn left, or turn right. No other functions are
provided to the user and all other functionality must be supplied else-where, 
as is the case with the actual MicroMouse robot.


Building the Project
====================
    
- To build the project select Build -> Build Solution or just run it and you will
  be prompted if you want to build the changed project

- To run the program select Debug -> Run Without Debugging. Hit Yes if you want to
  rebuild

If you need to clean for some reason you can go to Build -> Clean Solution

Contact
=======
Any pertaining to the program or Linux can be directed to mward4@buffalo.edu
Questions pertaining to Windows development can be directed to tomaszpi@buffalo.edu
