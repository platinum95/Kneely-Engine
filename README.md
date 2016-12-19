# Kneely-Engine
OpenGL based graphics/physics engine

No comments; project on hard mode

##Layout
Repository contains a VS2015 solution which contains two projects: the actual engine, under the name Boilerplate, and a working implementation of the engine, under the name OpenGL Testing. (Eventually I'll change these to more suitable names)

The BoilerPlate project contains the source for the engine itself. This is all the "inner" workings of the engine; management of openGL, maths, cameras, 3d objects, loading, framebuffers etc all go in here. The original idea was that it would be "Boilerplate" code (hence the name), containing code that would otherwise often be copied and pasted. It has since evolved to a bit beyond that, but still functions in the same basic principal. There's a class hierarchy int here which I should probably outline at some point. With this, you can truncate the higher end of the hierarchy to shrink the library down to fit smaller applications/purposes.

The OpenGL Testing project is a working example of an implementation of the BoilerPlate library. It uses pretty much all of the functionaility built into the BoilerPlate end. Look here for example initialisation/usage of the engine.

##Compilation
Needs a few libraries (x86 versions included in repo [probably shouldnt do this]). The VS solution should have them all set up as relative paths anyway, so should work out of the box. Linux compilation is definitely possible, theres a Makefile floating around somewhere which compiles an earlier version of the engine. I'll update it intermittently since cross-compatitibilty is an important part of the project for me.

##Usage
I dont want to write this right now. Soon. For now, look at the lovely uncommented spaghetti code in the implementation project.




