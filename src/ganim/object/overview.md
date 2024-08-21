@dir src/ganim/object

This directory contains objects and anything related to them.  There are two
main kinds of objects: The bases and the leaves.  The bases are:

 - @ref ganim::Updatable "Updatable", for anything that can be updated.
 - @ref ganim::Animatable "Animatable", for anything that should be animated.
 - @ref ganim::Transformable "Transformable", for anything that can be moved
   around on the screen.
 - @ref ganim::Object "Object", for anything that has some kind of extent/size
   and can be drawn on the screen.

Each of these is a subclass of the previous one.  The reason for separating this
much is that certain objects only need certain functionality.  There are leaf
classes that derive from each of these.  For example, the @ref ganim::Camera
"Camera" class derives from @ref ganim::Transformable "Transformable".

The leaf classes are:

 - @ref ganim::Shape "Shape", a Drawable that represents shapes of any kind.
