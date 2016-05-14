Input buttons are a mechanism for communicating with the mouse algorithm while
it's executing. They can be used to simulate pressing a button on the actual
robot (after a crash, for instance).

The available input buttons are simply 0 though 9. They can be used as follows:

First, a user of the simulator presses an input button. Then, the simulator
prints out a statement that the input button was pressed, and puts that
particular input button into the "pressed" state. It is then the mouse
algorithm's responsibility to periodically check the input buttons to see if any
of them are in the "pressed" state. If so, the algorithm can acknowledge the
button press (i.e., put them back into the "ready" state) and take action a
particular action if warranted.

The API for interacting with the input buttons is as follows:

```c++
// Returns true if the given input button is in the "pressed" state
bool inputButtonPressed(int inputButton);

// Sets the input to the "ready" state
void acknowledgeInputButtonPressed(int inputButton);
```

And this API might be used as follows:

```c++
while (true) {

    // We assume that this is defined by the algorithm
    doSomeMovement();

    // Every so often, check the input button 0 
    if (mouse->inputButtonPressed(0)) {

        // If it was pressed, acknowledge that it was pressed and set the
        // button back to the "ready" state
        mouse->acknowledgeInputButtonPressed(0);

        // Additionally, take some special, algorithm-specific action, like
        // sending the mouse to the beginning of the maze to simulate a reset
        mouse->resetPosition();
    }
}
```

If you're looking for an example of an algorithm that uses input buttons, check
out `FloodFill`. Right now, it's using input buttons 0 and 1 to simulate a soft
reset (retain memory) and hard reset (don't retain memory), respectively.
