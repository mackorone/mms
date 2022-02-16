## Creating New Releases

Notes for myself so I don't forget how to create releases.

### Windows
1. Install Qt and open the project in Qt Creator
1. Remove `CONFIG += debug` from `mms.pro` to reduce binary size
1. Change the build mode from "debug" to "release"
1. Build the binary from Qt Creator, run it to verify it works
1. Open a Qt command prompt from the start menu
1. Run: `bin\windeployqt.exe PATH\TO\BINARY`
1. Run the binary from file manager to verify it works
1. Zip up the folder and upload it

#### Links
1. https://doc.qt.io/qt-5/windows-deployment.html#the-windows-deployment-tool
1. https://forum.qt.io/topic/29079/solved-view-qt-command-prompt-in-windows/5
1. https://stackoverflow.com/questions/36630830/windeployqt-misses-some-of-the-libraries-and-gets-others-that-are-not-required


### macOS

TODO
