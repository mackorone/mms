While the simulator is running, different keys may be pressed to control
different bits of functionality.  All of the logic for handling key-press
events is located in the `sim::View::keyPressed()` method. Below is a summary
of the keys and their effect. We'll do our best to update this list whenever
the `sim::View::keyPressed()` method changes, but if there seems to be a
mismatch, feel free to update (or request updates to) this page.

| Key   | Effect                                     | Restrictions               |
|-------|--------------------------------------------|----------------------------|
| p     | (P)ause the simulation                     | `DISCRETE` mode only       |
| f     | (F)aster                                   | `DISCRETE` mode only       |
| s     | (S)lower                                   | `DISCRETE` mode only       |
| l     | Change the (l)ayout                        | None                       |
| r     | Toggle zoomed map (r)otation               | None                       |
| i     | Zoom (i)n                                  | None                       |
| o     | Zoom (o)ut                                 | None                       |
| t     | Toggle wall (t)ruth                        | None                       |
| c     | Toggle tile (c)olor visibility             | None                       |
| g     | Toggle tile fo(g) visibility               | None                       |
| x     | Toggle tile te(x)t visibility              | None                       |
| d     | Toggle tile (d)istance visibility          | None                       |
| h     | Toggle (h)eader visibility                 | None                       |
| w     | Toggle (w)ireframe mode                    | None                       |
| q     | (Q)uit the simulation                      | None                       |
| [0-9] | Sets the `wasPressed` flag for that number | None                       |
