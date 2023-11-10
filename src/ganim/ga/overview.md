@dir src/ganim/ga

By design, Ganim is heavily based on geometric algebra.  I couldn't find any
geometric algebra libraries that I liked so I rolled my own.  The results are in
this directory.  For most uses, you should use the definitions in @ref vga2.hpp,
@ref vga3.hpp, @ref pga2.hpp, or @ref pga3.hpp to create multivectors, and then
use the operations defined in @ref ganim::Multivector "Multivector" to use them.
If you want to do something different, look closer at @ref sga.hpp.
