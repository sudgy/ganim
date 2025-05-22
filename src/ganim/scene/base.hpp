#ifndef GANIM_SCENE_BASE_HPP
#define GANIM_SCENE_BASE_HPP

/** @file
 * @brief The @ref ganim::SceneBase "SceneBase" class
 */

#include <vector>

#include "ganim/color.hpp"
#include "ganim/gl/framebuffer.hpp"
#include "ganim/gl/texture.hpp"
#include "ganim/gl/buffer.hpp"
#include "ganim/gl/vertex_array.hpp"

#include "camera.hpp"

#include "ganim/object/bases/group.hpp"

namespace ganim {
    /** @brief The base class for scenes, which contains most of the scene logic
     *
     * This class has all of the scene logic except for what to actually do with
     * the final result.  Look at the subclasses to see how to actually make a
     * scene.
     *
     * This class also represents a collection of objects, with it supporting
     * iterators and having a size function.  However, this will only iterate
     * over all visible objects, and not all objects.
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
            virtual ~SceneBase();
            SceneBase(SceneBase&&) noexcept=default;
            SceneBase& operator=(SceneBase&&) noexcept=default;
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
             * This is used to save on rendering time when you only want to see
             * certain parts of a scene.  All objects will still be updated and
             * the scene will run like normal, but the scene won't be rendered.
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
            /** @brief Set how many layers of transparency to draw.
             *
             * By default, ganim won't do anything to try to draw transparent
             * objects in the correct order, which can often produce incorrect
             * results.  If you want to draw multiple overlapping transparent
             * objects, call this function with the maximum number of
             * overlapping transparent objects.  Note that the use of this
             * feature is pretty expensive, so it should be avoided when
             * possible.
             */
            void set_transparency_layers(int layers);


            /** @brief Set an image to be drawn in the background of the scene
             */
            void set_background_image(const std::string& filename);
            /** @brief Add an object to a scene
             *
             * Note that unlike in manim, this does not make the object visible!
             * You must set it to be visible manually, or use an animation that
             * does that for you.
             */
            template <typename T>
            void add(ObjectPtr<T>& object)
            {
                if constexpr (std::convertible_to<T&, Group&>) {
                    add_group(object);
                }
                else if constexpr (std::convertible_to<T&, Object&>) {
                    add_object(object);
                }
                else {
                    add_animatable(object);
                }
            }
            /** @brief Add a range of objects to the scene */
            void add(normal_input_range auto& object)
            {
                for (auto& obj : object) {
                    add(obj);
                }
            }
            /** @brief Add several objects to the scene */
            template <typename... Ts> requires(sizeof...(Ts) > 1)
            void add(Ts&... objects)
            {
                (add(objects), ...);
            }

            /** @brief Get the camera used for drawing */
            ObjectPtr<Camera> get_camera() {return M_camera;}

            auto begin() {return M_objects.begin();}
            auto end() {return M_objects.end();}
            auto begin() const {return M_objects.begin();}
            auto end() const {return M_objects.end();}
            auto cbegin() const {return M_objects.cbegin();}
            auto cend() const {return M_objects.cend();}
            int size() const {return static_cast<int>(M_objects.size());}

        private:
            /** @brief Used for subclasses to process the frames.
             *
             * When this function is called, the scene's framebuffer will be
             * active.
             */
            virtual void process_frame()=0;
            void draw_objects();

            void add_animatable_base(ObjectPtr<Animatable> object);
            void add_object_base(ObjectPtr<Object> object);
            void add_group_base(ObjectPtr<Group> object, bool together);
            void add_group_subobjects(ObjectPtr<Group>& object, bool together);
            void add_animatable(ObjectPtr<Animatable> object);
            void add_object(ObjectPtr<Object> object);
            void add_group(ObjectPtr<Group> object);
            void clean_up();

            gl::Framebuffer M_framebuffer;
            gl::Framebuffer M_downsampled_framebuffer;
            gl::Texture M_framebuffer_texture;
            gl::Texture M_downsampled_framebuffer_texture;
            gl::Texture M_depth_buffer;
            int M_pixel_width = 0;
            int M_pixel_height = 0;
            int M_fps;
            int M_frame_count = 0;
            Color M_background_color;
            ObjectPtr<Camera> M_camera;
            Camera M_static_camera;
            std::vector<ObjectPtr<Animatable>> M_animatables;
            std::vector<ObjectPtr<Object>> M_objects;
            std::vector<std::pair<ObjectPtr<Group>, bool>> M_groups;
            std::unique_ptr<Object> M_background_object;
            gl::Texture M_background_texture = 0;
            bool M_animating = true;

            struct DepthLayer {
                gl::Framebuffer framebuffer;
                gl::Texture texture;
                gl::Texture depth_buffer;
                gl::VertexArray vertex_array;
                gl::Buffer vertex_buffer;
                gl::Buffer element_buffer;
            };
            std::vector<DepthLayer> M_depth_layers;
    };
}

#endif
