There are two different types of mouse algorithms that you can write using
*mms*: `DISCRETE` and `CONTINUOUS`. Here's a brief outline of each algorithm
type, and why you might choose one mode of operation over the other:

### `DISCRETE`
- Used for writing high-level maze-solving code, without having to worry about
  not crashing into walls
- Provides an API that allows the user to control the mouse's movement in
  discrete chunks, such as moving forward one tile, or turning to the right by
  ninty degrees
- Provides an API for reading wall values based on current position and current
  rotation
- Allows you to speed up, slow down, and pause the simulation

### `CONTINUOUS`
- Used for writing low-level motor-solving code
- Can use the high-level logic developed in `DISCRETE` mode to very
  realistically simulate Micromouse performance (basically an end-to-end test)
- Provides an API that allows the user to control the mouse's movement in very
  realistic, fine-grained ways, such as setting the motor speeds
- Provides an API for reading sensor and wheel encoder values
- Does not allow the algorithm to obtain wall information directly - it must be
  inferred based on sensor values (just like the real mouse)
- Does not allow you to speed up, slow down, and pause the simulation (for
  performance reasons)

Most users should write a `DISCRETE` algorithm first, and then write a
`CONTINUOUS` algorithm, once the former is working well. In fact, `DISCRETE` is
the default algorithm type, so you don't even need to do anything extra if you
wish to use it.

Note that if you try to call a `DISCRETE` mouse API method from within a
`CONTINUOUS` algorithm (or vice versa), and error will be logged and the
simulator will exit. We "fail fast" here to ensure that algorithm writers don't
easily (or ever) confuse which API they're using.
