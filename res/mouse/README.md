# res/mouse

This directory contains mouse specification files. We don't have much
documentation on the file format yet, but that will be coming soon. That being
said, the format is XML and should be pretty readable. If you're interested in
writing your own mouse file, here's a few things to note:

- If you want the mouse to be discrete interface compatible, it should be as symmetrical as possible.

- You may have multiple wheels and sensors, but only one body.

- Sensors aren't used in discrete interface mode.
