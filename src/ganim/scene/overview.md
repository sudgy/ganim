@dir src/ganim/scene

This directory contains things related to scenes.  There are actually several
classes for scenes themselves.  There is @ref ganim::SceneBase "SceneBase",
which contains all of the logic except for what exactly to do with the final
image.  For that reason, it is abstract.  To actually use it, you need to use
one of its subclasses.  Currently there's only @ref ganim::Scene "Scene", which
writes the output of the scene to a file.

Because it doesn't really fit anywhere else, this directory also has the @ref
ganim::Camera "Camera" class.
