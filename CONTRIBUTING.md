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
1. Rename `bin` to `mms`, zip up the folder, and upload it
1. Rename the artifact to "windows.zip" in the GitHub UI

### macOS

Same as Windows, except:
1. You have to install XCode first
2. You have to install and configure XCode command line tools
```
sudo xcode-select --install
sudo xcode-select -s /Library/Developer/CommandLineTools/
```
3. You can run `macdeployqt` from any directory
4. You can just zip up `mms.app` rather than `bin`
5. Rename the artifact to "macos.zip" in the GitHub UI

#### Links
1. https://doc.qt.io/qt-6/windows-deployment.html#the-windows-deployment-tool
2. https://doc.qt.io/qt-6/macos-deployment.html#the-mac-deployment-tool
3. https://forum.qt.io/topic/29079/solved-view-qt-command-prompt-in-windows/5
