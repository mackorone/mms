# Known Issues

This is a working list of things that aren't known to be broken/imperfect, but
that I haven't had the time to fix (yet). If you're having difficulty using the
simulator, and your issue isn't listed here, please let me know and I'll
investigate/triage it.

## Slow File Dialog
- **Problem**: The file dialog is extremely slow if the mouse algorithm is running.
- **Workaround (You)**: Stop the mouse algorithm before trying to use the file dialog.
- **Solution (Me)**: Refactor the multithreaded code to avoid the implicit sharding
problem, and thus eliminate the need to lock on every update.
