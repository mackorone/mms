This is a small application for simulatiing MicroMouse algorithms. Note that
freeglut must be installed for this application to work. As it stands now,
all solution code is to be placed in the "solve" method of the Solution class
(preferably between the lines that indication where the code should go).
Additionlly, the simulation (for now) must be run from the "src" folder, as in:

    * From the "src" directory *
    
        ../bin/MMSim

Right now, there are two algorithms defined in the Solution class. The first is
a hard-coded solution for maze3 and the second is a simple wall-following 
algorithm. Note that neither of these solutions is sufficient.

For users writing their own algorithms, they should call any one of the
following functions in their code to receive input and generate output:

    1) m_mouse->wallFront()
    2) m_mouse->wallRight()
    3) m_mouse->wallLeft()
    4) m_mouse->moveForward()
    5) m_mouse->turnRight()
    6) m_mouse->turnLeft()

Note that after any changes have been made, you must remake the project from the
root directory (the project folder that contains "src", "bin", etc.) as follows:

    make

Any questions can be directed to mward4@buffalo.edu
