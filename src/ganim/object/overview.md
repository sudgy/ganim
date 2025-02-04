@dir src/ganim/object

This directory contains objects and anything related to them.  There are two
main kinds of objects: The bases and the leaves.  The bases are:

 - @ref ganim::Updatable "Updatable", for anything that can be updated.
 - @ref ganim::Animatable "Animatable", for anything that should be animated.
 - @ref ganim::Transformable "Transformable", for anything that can be moved
   around on the screen.
 - @ref ganim::Object "Object", for anything that has some kind of extent/size
   and can be drawn on the screen.
 - @ref ganim::SingleObject "SingleObject", for Objects that are more concrete
   and represent single unit.
 - @ref ganim::Group "Group", for Objects that are a collection of other
   objects.

Each of these is a subclass of the previous one, except that both SingleObject
and Group are a subclass of Object.  The reason for separating this much is that
certain objects only need certain functionality.  There are leaf classes that
derive from each of these.  For example, the @ref ganim::Camera "Camera" class
derives from @ref ganim::Transformable "Transformable".

The leaf classes are:

 - @ref ganim::Shape "Shape", an object that represents shapes of any kind.
 - @ref ganim::TextureShape "TextureShape", an extension of Shape to support
   textures.
 - @ref ganim::PolygonShape "PolygonShape", a Shape that represents a polygon.
 - @ref ganim::Path "Path", an object that represents a 2D curve.
 - @ref ganim::ArrowPath "ArrowPath", a curve that has an arrow at the end.
 - @ref ganim::Vector "Vector", an object that represents an arrow in space.
 - @ref ganim::Bivector "Bivector", an object that represents an oriented plane
   segment.
 - @ref ganim::Value "Value", which represents a value and can't be drawn.  It's
   made to be able to animate other objects that are all based on a changing
   number.
 - @ref ganim::Text "Text", which represents simple text.
 - @ref ganim::Tex "Tex", which represents mathematical expressions.

To use these classes, you almost always need to use an @ref ganim::ObjectPtr
"ObjectPtr".

There's also some text stuff but it's very unpolished and most of it has no
documentation since it will all get scrapped anyway (famous last words).
