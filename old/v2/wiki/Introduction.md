### Summary

This is an application for developing and simulating Micromouse algorithms. For
information on the Micromouse competition, see the [Micromouse Wikipedia
page](http://en.wikipedia.org/wiki/Micromouse)

### Motivation

Back when I was in IEEE at the University at Buffalo, as I'm sure is the case
now, we procrastinated on nearly all of our projects. This included Micromouse;
if it was more than a week before the competition, you could safely assume our
robot wasn't built yet. This proved to be problematic for the programmers on
the project. They weren't able to start writing code until it was way too late
(i.e., the night before the competition). In an attempt to solve the problem of
not being able to write and test code before the robot was built, *mms* was
born.

### What *mms* Can Do

- Enable you to write Micromouse code without a working Micromouse
- Allow you to visualize how your Micromouse would behave in a real maze
- Let you write both high-level path-finding code and low-level motor-control code
- Let you interact with the algorithm, while executing, via ten different input buttons
- Let you display arbitrary (ascii-ish) text on any/all of the tiles
- Let you color and re-color the base of any maze tile/cell
- Show you the distance of any tile from the center of the maze
- Load mazes from `.maz` files, allowing you to test any arbitrary maze
- Allow you to write and use your own maze generation algorithms
- Work (mostly) well with default values and default configuration
- Probably a ton of other things that I'm forgetting...

### What *mms* (Currently) Can't Do

- Actually be a real-time system (it's running on top of an OS, after all)
- Detect whether or not your algorithm uses the C++ STL (or doing other silly things)
- Behave absolutely deterministically in `CONTINUOUS` mode
- Run more than one mouse at once (though we're hoping to do this in the future)

### Some Concluding Introductory Thoughts

*mms* is a tool for you to use. It's not an end in and of itself. It's only a
means to end. The end is a working, and hopefully winning, Micromouse. Use *mms*
in whatever way best helps you achieve that goal.
