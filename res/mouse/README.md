# res/mouse

This directory contains mouse specification files. We don't have much
documentation on the file format yet, but that will be coming soon. That being
said, the format is XML and should be pretty readable. If you're interested in
writing your own mouse file, here's a few things to note:

- The file is used to simultaneously specify both the mouse dimensions *and*
starting position of the mouse.

- The format implicitly assumes that *forward* means north. If you break this
assumption, weird things will happen, and chances are that your mouse won't
be discrete interface compatible.

- If you want the mouse to be discrete interface compatible, it has to be
differential drive
    - It should only have two wheels
    - The right wheel should be at 90 degrees, the left at 270 degrees
    - The wheels should have the same `y` value

- You may have multiple wheels and sensors, but only one body.

- Sensors aren't used in discrete interface mode.

- We use the centroid of the body polygon as the center of mass for the mouse.
