Introduction and Requirements
=============================
This is a small application for simulating MicroMouse algorithms. Note that
freeglut *must* be installed for this application to work. To install it on
Ubuntu systems, simply enter the following into the terminal:

    sudo apt-get install freeglut3-dev


Software Components
===================
As it stands now, the simulation consists of two components. The first is the
sim component, which is responsible for carrying out all of the backend
operations necessary for a smooth and useful simulation. The second component
is the algo component, which is responsible for specifying the algorithm that
the mouse should use to actually solve the maze. Thus, any algorithm code should
be placed within the algo directory, preferably in its own directory (so that
things stay organized). See "src/algo/floodFill" for an example of this. 


Running the Simulation
======================
The simulation can be run by executing the binary in the "bin" directory, as in:
    
    ./<path-to-bin>/MMSim

For example, if you are in the "src" directory, enter the following to run the
simulation:

    ../bin/MMSim

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
robot to move froward, turn left, or turn right. No other functions are
provided to the user and all other functionality must be supplied else-where, 
as is the case with the actual MicroMouse robot.


Building the Project
====================
After any changes have been made to your code, you must remake the project from
the root directory (the project folder that contains "src", "bin", etc.) as
follows:

    make

If your code doesn't seem to be working properly, try entering the following,
again from the root directory:

    make clean
    make

The use of "make clean" simply requires the computer to recompile your files, 
despite it potentially thinking that changes have not been made.


Contact
=======
Any questions can be directed to mward4@buffalo.edu
