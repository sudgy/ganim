#ifndef GANIM_OBJECT_BIVECTOR_HPP
#define GANIM_OBJECT_BIVECTOR_HPP

namespace ganim {
    class Object;
    class Bivector {
        public:
            virtual ~Bivector()=default;
            virtual Object& get_outside()=0;
            virtual Object& get_inside()=0;
    };
}

#endif
