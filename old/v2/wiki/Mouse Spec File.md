The simulator allows for arbitrary mouse configurations. That is, it assumes
nothing about the physical construction/layout of the mouse. Instead, it reads
all of the information from mouse spec files. All mouse spec files are located
in `res/mouse/`. Some mouse files in that directory are:

###### default.xml

A simple, default mouse layout. Nothing fancy - just two wheels and a body.

###### omniMouse.xml

A three-wheeled, triangle-shaped robot that emulates a robot with omniwheels.
This mouse is a solid example of the flexibility of the simulator to work with
all kinds of different mice.

###### megaMouse.xml

The layout for the micromouse that members of the University at Buffalo's IEEE
branch took to competition in 2015. It has four wheels and six sensors. It has
a very high max wheel speed, so if you want to use this mouse with a `DISCRETE`
algorithm, you should definitely override the `wheelSpeedFraction` option (see
[Mouse Algorithm Options](Mouse Algorithm Options)).

### Compatibility

Mouse configurations work best when they satisfy the following two properties:
- It can rotate in place
- It can move forward in a straight line.

Basically, mice work best when they have both rotational and planar symmetry. 

### Mouse Spec File Components

There are currently five different components to a mouse spec file. They are as
follows.

###### `Forward-Direction`

The intended direction of forward travel of the mouse, in degrees, based on the
layout of the body and wheels of the mouse. That is, if the body and wheels of
the mouse are specified such that the mouse travels east when it moves forward,
then this value should be 0 degrees. Note that 90 degrees means north, 180
degrees means west, and 270 degrees means south. This value affects the
starting rotation of the mouse. That is, if the mouse algorithm specifies that
the starting direction should be north, then the mouse will be rotated such
that the forward direction is facing north at the beginning of the simulation. 

Example:

```xml
<Forward-Direction>90</Forward-Direction> <!-- Degrees -->
```

###### `Center-Of-Mass`

This specifies the location, relative to the positions of the other components,
other center of mass of the mouse. The center of mass is the point about which
the mouse rotates, and the point that is used to determine which tile the mouse
is located in. It is also used to position the mouse at the start of the
simulation. That is, the center of mass is always positioned over the top of
the center of the starting tile.

Example:

```xml
<Center-Of-Mass>
    <X>.09</X> <!-- Meters -->
    <Y>.09</Y> <!-- Meters -->
</Center-Of-Mass>
```

###### `Body`

The body of the mouse, consisting of three or more vertices. These vertices
must be specified in either clockwise or coutnerclockwise order. And the values
are relative to all of the other components.

Example:

```xml
<Body>
    <Vertex>
        <X>.065</X> 
        <Y>.0604</Y>
    </Vertex>
    <Vertex>
        <X>.065</X> 
        <Y>.1204</Y>
    </Vertex>
    <Vertex>
        <X>.0535</X> 
        <Y>.1204</Y>
    </Vertex>
    <Vertex>
        <X>.065</X> 
        <Y>.150</Y>
    </Vertex>
    <Vertex>
        <X>.09</X> 
        <Y>.1574</Y>
    </Vertex>
    <Vertex>
        <X>.115</X> 
        <Y>.150</Y>
    </Vertex>
    <Vertex>
        <X>.1265</X> 
        <Y>.1204</Y>
    </Vertex>
    <Vertex>
        <X>.115</X> 
        <Y>.1204</Y>
    </Vertex>
    <Vertex>
        <X>.115</X> 
        <Y>.0604</Y>
    </Vertex>
</Body>
```

###### `Wheel`

The wheels of the robot. Each wheel consists of seven mandatory pieces of information:
- Name of the wheel, used by `CONTINUOUS` algorithms to set the individual wheel speeds
- Diameter of the wheel, in meters
- Width of the wheel, in meters
- Position of the wheel, relative to the other components
- Direction of the wheel, in degrees, such that a positive angular velocity causes the wheel to travel in that direction
- Max speed of the wheel, in rotations per minute
- Encode type of the wheel, only used by `CONTINUOUS` algorithms, can be `ABSOLUTE` or `RELATIVE`
- Encoder ticks per revolution, i.e., the resolution of the encoder

For more information about encoders, see [CONTINUOUS Mode](CONTINUOUS Mode).

Example: 

```xml
<Wheel>
    <Name>left-lower</Name>
    <Diameter>.0245</Diameter>
    <Width>.0077</Width>
    <Position>
        <X>.0574</X>
        <Y>.07615</Y>
    </Position>
    <Direction>270</Direction>
    <Max-Speed>6000</Max-Speed>
    <Encoder-Type>RELATIVE</Encoder-Type>
    <Encoder-Ticks-Per-Revolution>360</Encoder-Ticks-Per-Revolution>
</Wheel>
```

###### `Sensor`

The sensors of the robot. Each sensor consists of six mandatory pieces of information:
- Name of the sensor, used by `CONTINUOUS` algorithms to read the individual sensors
- Radius of the circular body of the sensor, in meters
- Range of the sensor's view, i.e., how far the sensor can see, in meters
- Half width of the sensor's view, i.e., how wide the sensor can see to the left and right, in degrees
- Position of the sensor relative to the other components, in meters
- Direction that the sensor is facing, where 0 degrees is east and 90 degrees is north, etc.

For more information about sensors, see [CONTINUOUS Mode](CONTINUOUS Mode).

Example:

```xml
<Sensor>
    <Name>left-front</Name>
    <Radius>.005</Radius>
    <Range>.20</Range>
    <Half-Width>5</Half-Width>
    <Position>
        <X>.0631</X>
        <Y>.1334</Y>
    </Position>
    <Direction>90</Direction>
</Sensor>
```

### Putting It All Together

Example:

```xml
<?xml version="1.0" encoding="utf-8"?>
<Mouse>
    <Forward-Direction>90</Forward-Direction>
    <Center-Of-Mass>
        <X>.09</X> 
        <Y>.09</Y>
    </Center-Of-Mass>
    <Body>
        <Vertex>
            <X>.065</X> 
            <Y>.0604</Y>
        </Vertex>
        <Vertex>
            <X>.065</X> 
            <Y>.1204</Y>
        </Vertex>
        <Vertex>
            <X>.0535</X> 
            <Y>.1204</Y>
        </Vertex>
        <Vertex>
            <X>.065</X> 
            <Y>.150</Y>
        </Vertex>
        <Vertex>
            <X>.09</X> 
            <Y>.1574</Y>
        </Vertex>
        <Vertex>
            <X>.115</X> 
            <Y>.150</Y>
        </Vertex>
        <Vertex>
            <X>.1265</X> 
            <Y>.1204</Y>
        </Vertex>
        <Vertex>
            <X>.115</X> 
            <Y>.1204</Y>
        </Vertex>
        <Vertex>
            <X>.115</X> 
            <Y>.0604</Y>
        </Vertex>
    </Body>
    <Wheel>
        <Name>left-lower</Name>
        <Diameter>.0245</Diameter>
        <Width>.0077</Width>
        <Position>
            <X>.0574</X>
            <Y>.07615</Y>
        </Position>
        <Direction>270</Direction>
        <Max-Speed>6000</Max-Speed>
        <Encoder-Type>RELATIVE</Encoder-Type>
        <Encoder-Ticks-Per-Revolution>360</Encoder-Ticks-Per-Revolution>
    </Wheel>
    <Wheel>
        <Name>left-upper</Name>
        <Diameter>.0245</Diameter>
        <Width>.0077</Width>
        <Position>
            <X>.0574</X>
            <Y>.1044</Y>
        </Position>
        <Direction>270</Direction>
        <Max-Speed>6000</Max-Speed>
        <Encoder-Type>RELATIVE</Encoder-Type>
        <Encoder-Ticks-Per-Revolution>360</Encoder-Ticks-Per-Revolution>
    </Wheel>
    <Wheel>
        <Name>right-lower</Name>
        <Diameter>.0245</Diameter>
        <Width>.0077</Width>
        <Position>
            <X>.1226</X>
            <Y>.07615</Y>
        </Position>
        <Direction>90</Direction>
        <Max-Speed>6000</Max-Speed>
        <Encoder-Type>RELATIVE</Encoder-Type>
        <Encoder-Ticks-Per-Revolution>360</Encoder-Ticks-Per-Revolution>
    </Wheel>
    <Wheel>
        <Name>right-upper</Name>
        <Diameter>.0245</Diameter>
        <Width>.0077</Width>
        <Position>
            <X>.1226</X>
            <Y>.1044</Y>
        </Position>
        <Direction>90</Direction>
        <Max-Speed>6000</Max-Speed>
        <Encoder-Type>RELATIVE</Encoder-Type>
        <Encoder-Ticks-Per-Revolution>360</Encoder-Ticks-Per-Revolution>
    </Wheel>
    <Sensor>
        <Name>left-front</Name>
        <Radius>.005</Radius>
        <Range>.20</Range>
        <Half-Width>5</Half-Width>
        <Position>
            <X>.0631</X>
            <Y>.1334</Y>
        </Position>
        <Direction>90</Direction>
    </Sensor>
    <Sensor>
        <Name>left-middle</Name>
        <Radius>.005</Radius>
        <Range>.20</Range>
        <Half-Width>5</Half-Width>
        <Position>
            <X>.0685</X>
            <Y>.1484</Y>
        </Position>
        <Direction>135</Direction>
    </Sensor>
    <Sensor>
        <Name>left-side</Name>
        <Radius>.005</Radius>
        <Range>.20</Range>
        <Half-Width>5</Half-Width>
        <Position>
            <X>.0895</X>
            <Y>.1534</Y>
        </Position>
        <Direction>180</Direction>
    </Sensor>
    <Sensor>
        <Name>right-front</Name>
        <Radius>.005</Radius>
        <Range>.20</Range>
        <Half-Width>5</Half-Width>
        <Position>
            <X>.1169</X>
            <Y>.1334</Y>
        </Position>
        <Direction>90</Direction>
    </Sensor>
    <Sensor>
        <Name>right-middle</Name>
        <Radius>.005</Radius>
        <Range>.20</Range>
        <Half-Width>5</Half-Width>
        <Position>
            <X>.1115</X>
            <Y>.1484</Y>
        </Position>
        <Direction>45</Direction>
    </Sensor>
    <Sensor>
        <Name>right-side</Name>
        <Radius>.005</Radius>
        <Range>.20</Range>
        <Half-Width>5</Half-Width>
        <Position>
            <X>.0905</X>
            <Y>.1534</Y>
        </Position>
        <Direction>0</Direction>
    </Sensor>
</Mouse>
```

Here's a picture of the mouse that is specified by the above, i.e., `megaMouse.xml`:

![](https://github.com/mackorone/mms/wiki/imgs/megaMouse.png)
