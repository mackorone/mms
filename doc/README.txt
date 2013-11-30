This is a small application for simulating MicroMouse algorithms. Note that
freeglut *must* be installed for this application to work. As it stands now,
all solution code is to be placed in the "solve" method of the Solution class.
The simulation can be run by executing the binary in the "bin" directory, as
in:
    
        ./<path-to-bin>/MMSim

For example, if you are in the "src" directory, enter the following to run the
simulation:

    ../bin/MMSim

If you already in the "bin" directory, simply enter

    ./MMsim

Right now, there are two algorithms defined in the Solution class. The first is
a simple right-wall-following algorithm and the second, a simple left-wall
-following algorithm. Neither of these solutions is satisfactory.

For users writing their own algorithms, they should call any one of the
following functions in their code to receive input and generate output:

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

Any questions can be directed to mward4@buffalo.edu
