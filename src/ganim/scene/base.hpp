#ifndef GANIM_SCENE_BASE_HPP
#define GANIM_SCENE_BASE_HPP

/** @file
 * @brief The @ref ganim::SceneBase "SceneBase" class
 */

#include <vector>

#include "ganim/color.hpp"
#include "ganim/maybe_owning_ref.hpp"
#include "ganim/gl/framebuffer.hpp"
#include "ganim/gl/texture.hpp"

#include "camera.hpp"

#include "ganim/object/shaders.hpp"
#include "ganim/object/bases/drawable.hpp"
#include "ganim/object/group/cluster.hpp"

namespace ganim {
    /** @brief The base class for scenes, which contains most of the scene logic
     *
     * This class has all of the scene logic except for what to actually do with
     * the final result.  Look at the subclasses to see how to actually make a
     * scene.
     */
    class SceneBase : public Updatable {
        public:
            /** @brief Constructor.
             *
             * Because this class is abstract, you probably won't be calling
             * this yourself.
             *
             * @param pixel_width The width, in pixels, of this scene.
             * @param pixel_height The height, in pixels, of this scene.
             * @param coord_width The width, in coordinate units, of this scene.
             * @param coord_height The height, in coordinate units, of this
             * scene.
             * @param fps The framerate, in frames per second, of this scene.
             */
            SceneBase(
                int pixel_width,
                int pixel_height,
                double coord_width,
                double coord_height,
                int fps
            );
            virtual ~SceneBase()=default;
            /** @brief Process the scene for one frame.
             *
             * This will update all objects, draw them all onto the scene's
             * internal framebuffer, and then call @ref process_frame.
             */
            void frame_advance();
            /** @brief Process the scene for several frames.
             *
             * @param amount The number of frames to process.
             */
            void frame_advance(int amount);
            /** @brief Process the scene for a certain amount of time.
             *
             * @param time The time, in seconds, to process.  This time will
             * actually be converted to the nearest number of frames, so the
             * amount of time that this functions waits may not be exactly what
             * was passed into this function.
             */
            void wait(double time = 1.0);
            /** @brief Get the number of frames that have elapsed since the
             * start of the scene.
             *
             * @return The number of frames that have elapsed since the start of
             * the scene
             */
            int get_frame_count() const;
            /** @brief Get the time that has elapsed since the start of the
             * scene.
             *
             * @return The time, in seconds, that has elapsed since the start of
             * the scene.
             */
            double get_time() const;
            /** @brief Stop animating the scene
             *
             * This is used to save on time when you only want to see certain
             * parts of a scene.  All objects will still be updated and the
             * scene will run like normal, but the scene won't be rendered.
             */
            void stop_animating();
            /** @brief Start animating the scene
             *
             * This is used to reverse a call to @ref stop_animating
             */
            void start_animating();

            /** @brief Get the width, in pixels, of this scene. */
            constexpr int pixel_width() const {return M_pixel_width;}
            /** @brief Get the height, in pixels, of this scene. */
            constexpr int pixel_height() const {return M_pixel_height;}
            /** @brief Set the background color of this scene. */
            constexpr void set_background_color(const Color& color)
                {M_background_color = color;}


            void set_background_image(const std::string& filename);
            /** @brief Add an object to a scene
             *
             * Note that unlike in manim, this does not make the object visible!
             * You must set it to be visible manually, or use an animation that
             * does that for you.
             */
            template <typename T>
            void add(T& object)
            {
                if constexpr (std::convertible_to<T&, Animatable&>) {
                    add_animatable(object);
                }
                if constexpr (std::convertible_to<T&, Drawable&>) {
                    add_drawable(object);
                    return;
                }
                else if constexpr (std::is_polymorphic_v<T>) {
                    if (auto* p = dynamic_cast<Drawable*>(&object)) {
                        add_drawable(*p);
                        return;
                    }
                }
                if constexpr (std::convertible_to<T&, Cluster>) {
                    add_group(object);
                    return;
                }
                else if constexpr (std::is_polymorphic_v<T>) {
                    if (auto* p = dynamic_cast<Cluster*>(&object)) {
                        add_group(*p);
                        return;
                    }
                }
                if constexpr (std::ranges::input_range<T>) {
                    for (auto& obj : object) {
                        add(obj);
                    }
                }
            }
            template <typename... Ts> requires(sizeof...(Ts) > 1)
            void add(Ts&... objects)
            {
                (add(objects), ...);
            }
            /** @brief Remove an object from a scene
             *
             * This will make the object not be updated or drawn anymore, and
             * the object will be non-animatable.  Note that animations that
             * "remove" objects from a scene don't actually call this function!
             * They just set the objects visibility to false, to allow you to
             * use a creating animation on it again later.  However, you need to
             * call this function if an object's lifetime is about to end.
             */
            template <typename T>
            void remove(T& object)
            {
                if constexpr (std::convertible_to<T&, Animatable&>) {
                    remove_animatable(object);
                }
                if constexpr (std::convertible_to<T&, Drawable&>) {
                    remove_drawable(object);
                    return;
                }
                else if constexpr (std::is_polymorphic_v<T>) {
                    if (auto* p = dynamic_cast<Drawable*>(&object)) {
                        remove_drawable(*p);
                        return;
                    }
                }
                if constexpr (std::convertible_to<T&, Cluster>) {
                    remove_group(object);
                    return;
                }
                else if constexpr (std::is_polymorphic_v<T>) {
                    if (auto* p = dynamic_cast<Cluster*>(&object)) {
                        remove(*p);
                        return;
                    }
                }
                if constexpr (std::ranges::input_range<T>) {
                    for (auto& obj : object) {
                        remove(obj);
                    }
                }
            }
            template <typename... Ts> requires(sizeof...(Ts) > 1)
            void remove(Ts&... objects)
            {
                (remove(objects), ...);
            }

            void add_for_animation(MaybeOwningRef<Animatable> object);
            void remove_for_animation(Animatable& object);

            Camera& get_camera() {return M_camera;}

        private:
            /** @brief Used for subclasses to process the frames.
             *
             * When this function is called, the scene's framebuffer will be
             * active.
             */
            virtual void process_frame()=0;

            void add_animatable(Animatable& object);
            void add_drawable(Drawable& object);
            void add_group(GroupBase& object);
            void remove_animatable(Animatable& object);
            void remove_drawable(Drawable& object);
            void remove_group(GroupBase& object);

            gl::Framebuffer M_framebuffer;
            gl::Texture M_framebuffer_texture;
            gl::Texture M_depth_buffer;
            int M_pixel_width = 0;
            int M_pixel_height = 0;
            int M_fps;
            int M_frame_count = 0;
            Color M_background_color;
            Camera M_camera;
            Camera M_static_camera;
            std::vector<Animatable*> M_objects;
            std::vector<Drawable*> M_drawables;
            std::vector<MaybeOwningRef<Animatable>> M_ghost_animating_objects;
            std::unique_ptr<Drawable> M_background_object;
            gl::Texture M_background_texture = 0;
            bool M_animating = true;
    };
}

#endif
