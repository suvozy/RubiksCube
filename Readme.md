![](https://github.com/suvozit/RubiksCube-Assets/raw/master/rubiks%20cube%20solver%20-%20small.PNG)

Rubik’s Cube
===
I am trying to make a Rubik’s Cube solver. 
I have finished working on Windows MFC programing now working on 3D modeling of the cube using OpenGL. 
After the modelling I have to work on the solver algorithm. 
Its going to be a a very interesting project. 
Later on I’ll try to scan the cube through the camera and process it using OpenCV to solve the customized cubes.

[Blogger](https://bitsits.blogspot.com/2011/06/rubiks-cube.html)
[Tumblr](https://suvozit.tumblr.com/post/96210256543/rubiks-cube)

Rubik's Cube Solver
===
Okay, its done. This Rubik's Cube solver uses [Korf's Algorithm][korf] also called [IDA*][ida]. 
The solver have two views 3D and flat. 
The implementation is similar to [Jaap Scherphuis' Cubie][cubie], solving the cube with a maximum depth of 25. 
The cube can be scrambled and solved but its not configurable. 
The 3D cube is drawn using OpenGL. 

![](https://github.com/suvozit/RubiksCube-Assets/raw/master/rubiks%20cube%20solver.PNG)

Download: [RubiksCube.zip][zip] (253 KB)

[korf]: http://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Korf.27s_Algorithm
[ida]: http://en.wikipedia.org/wiki/IDA*
[cubie]: http://www.jaapsch.net/puzzles/cubie.htm
[zip]: https://github.com/suvozit/RubiksCube-Assets/raw/master/RubiksCube.zip

[Blogger](https://bitsits.blogspot.com/2011/06/rubiks-cube-solver.html)
[Tumblr](https://suvozit.tumblr.com/post/96211887608/rubiks-cube-solver-okay-its-done-this-rubiks)
