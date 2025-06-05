#ifndef GANIM_OBJECT_ARROW_PATH_HPP
#define GANIM_OBJECT_ARROW_PATH_HPP

#include "ganim/object/path.hpp"
#include "ganim/object/bases/static_group.hpp"

namespace ganim {
    /** @brief A struct for the arguments to construct an @ref ArrowPath. */
    struct ArrowPathArgs {
        /** @brief The tip size, that is, the height of the triangle used for
         * the tip.
         */
        double tip_size = 0.3;
        /** @brief The thickness of the main part of the path. */
        double thickness = 0.06;
    };
    /** @brief An object representing a 2D curve with an arrow on the end.
     *
     * It's basically just a path going almost all the way to the end, and then
     * a small triangle representing the tip of the arrow.  You must have enough
     * space for the tip to fit.
     *
     * @see make_arrow_path
     */
    class ArrowPath : public StaticGroup<Path, Shape> {
        public:
            /** @brief Construct an arrow path given a list of points using 2D
             * PGA.
             */
            explicit ArrowPath(
                const std::vector<pga2::Bivec>& points,
                ArrowPathArgs args = {}
            );
            /** @brief Construct an arrow path given a list of points using 2D
             * VGA.
             */
            explicit ArrowPath(
                const std::vector<vga2::Vec>& points,
                ArrowPathArgs args = {}
            );
            /** @brief Basically the same as "recalling" the constructor.
             *
             * @see ArrowPath(const std::vector<pga2::Bivec>&, ArrowPathArgs)
             */
            void recreate(
                std::vector<pga2::Bivec> points,
                ArrowPathArgs args = {}
            );
            /** @brief Basically the same as "recalling" the constructor.
             *
             * @see ArrowPath(const std::vector<vga2::Vec>&, ArrowPathArgs)
             */
            void recreate(
                const std::vector<vga2::Vec>& points,
                ArrowPathArgs args = {}
            );

            ObjectPtr<ArrowPath> polymorphic_copy() const;

        private:
            virtual ArrowPath* polymorphic_copy_impl() const;
    };
    /** @brief Make an ArrowPath in an ObjectPtr.
     *
     * @see ArrowPath::ArrowPath(const std::vector<pga2::Bivec>&, ArrowPathArgs)
     */
    inline ObjectPtr<ArrowPath> make_arrow_path(
        const std::vector<pga2::Bivec>& points,
        ArrowPathArgs args = {}
    )
    {
        return ObjectPtr<ArrowPath>(points, args);
    }
    /** @brief Make an ArrowPath in an ObjectPtr.
     *
     * @see ArrowPath::ArrowPath(const std::vector<vga2::Vec>&, ArrowPathArgs)
     */
    inline ObjectPtr<ArrowPath> make_arrow_path(
        const std::vector<vga2::Vec>& points,
        ArrowPathArgs args = {}
    )
    {
        return ObjectPtr<ArrowPath>(points, args);
    }
}

#endif
