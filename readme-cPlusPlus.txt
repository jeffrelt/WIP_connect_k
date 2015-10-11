===ConnectK C++ shell===

The c++ shell runs as a subprocess of the java shell. The java shell will make calls to the C++ shell using std I/O and the C++ responds similarly. This communication is handled in the connectK.cpp file and does not require any additional work. 

To run your C++ AI, compile the source into an executable (.exe) and name it "connectK.exe" (capital K) and place it in the same folder as connectKShell.jar (note that this is the .jar file and not the .java file).
In Visual Studio, add all .cpp files to your project, and then choose the "Build" command. This will create an .exe file in the "Debug" subdirectory of your project.
On Unix, you can compile all .cpp files into a single executable (e.g., one called myAI) by typing the following command into a terminal:
$ g++ ConnectK.cpp Move.cpp AIShell.cpp -o myAI

Launch ConnectKShell.jar (by double-clicking or by java command). To play against your AI select file->new and choose "class connectK.ProcessPlayer" from the drop down menu next to "Player 2 AI". Click "New Game" and the game will start against your C++ AI. 

Alternatively, you may launch the Java shell from the command line and provide it with the name of the executable you would like it to use for the AI. This is useful if you would rather name your executable something other than "connecktK.exe"
To do this, run the following command: 
java -jar ConnectKShell.jar processAI AIExecutableName.exe

The source files for the C++ program are contained in the src folder. 
The Java source files are also included in jsrc though this is provided as a convenience for those interested. Changing the jsrc files is unnecessary.

A batch file is also included as a demonstration of how to run the java shell from the command line. Please note that the java directory must be added to your PATH variable to function correctly. Otherwise you may see "Command not found" or similar errors.  (This file must be changed to .bat)
