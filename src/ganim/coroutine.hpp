#ifndef GANIM_COROUTINE_HPP
#define GANIM_COROUTINE_HPP

/** @file
 * @brief Contains several coroutine helpers
 *
 * It contains four coroutine helpers:
 * - @ref ganim::Coroutine, for a coroutine that passes no information around
 * - @ref ganim::ArgsCoroutine, for a coroutine that takes in arguments each
 *   time it is resumed
 * - @ref ganim::Generator, for a coroutine that returns a value each time it is
 *   suspended
 * - @ref ganim::ArgsGenerator, for a coroutine that takes in arguments each
 *   time it is resumed and returns a value each time it is suspended
 */

#include <coroutine>
#include <exception>
#include <tuple>
#include <optional>

namespace ganim {

/** @brief Used for coroutines that take in arguments each time they are resumed
 *
 * To use it, define a coroutine that returns `ArgsCoroutine<Args...>`.  To
 * suspend execution within the coroutine, use
 * `co_await std::suspend_always();`.  Then, to actually use it, call the
 * coroutine, store the result in a variable `co`, and then to run it to the
 * next suspension point, use `co(args...)`.  Within the coroutine, you can
 * access the arguments by using `co_await ArgsCoroutine<Args...>::get_args()`.
 * This returns an std::tuple that contains the arguments passed in.  This only
 * needs to be done once, and the values in the tuple will always be updated.
 * Because this tuple is constantly being assigned to, argument types without
 * assignment operators are not supported (such as reference types).
 *
 * As an example, here is some of the test code used to test this class:
 * ```cpp
 * auto co = []() -> ArgsCoroutine<int>{
 *     auto& args = co_await ArgsCoroutine<int>::get_args();
 *     auto& arg = get<0>(args);
 *     REQUIRE(arg == 1);
 *     co_await std::suspend_always();
 *     REQUIRE(arg == 2);
 *     co_await std::suspend_always();
 *     REQUIRE(arg == 3);
 * }();
 * co(1);
 * co(2);
 * co(3);
 * ```
 */
template<typename... Args>
class ArgsCoroutine {
    public:
        /// @private
        struct Promise {
            std::exception_ptr M_exception;
            std::tuple<Args...> M_args;

            ArgsCoroutine get_return_object()
            {
                return ArgsCoroutine(Handle::from_promise(*this));
            }
            std::suspend_always initial_suspend() {return {};}
            std::suspend_always final_suspend() noexcept {return {};}
            void unhandled_exception()
            {
                M_exception = std::current_exception();
            }

            void return_void() {}
        };
        /// @private
        using promise_type = Promise;
        /// @private
        using Handle = std::coroutine_handle<Promise>;

        ArgsCoroutine(Handle handle) : M_handle(handle) {}
        ~ArgsCoroutine()
        {
            M_handle.destroy();
        }

        struct get_args
        {
            std::tuple<Args...>* M_args;
            bool await_ready() {return false;}
            bool await_suspend(std::coroutine_handle<Promise> h)
            {
                M_args = &h.promise().M_args;
                return false;
            }
            std::tuple<Args...>& await_resume() {return *M_args;}
        };
        void operator()(Args... args)
        {
            if (M_handle.done()) return;
            M_handle.promise().M_args = std::make_tuple(args...);
            M_handle();
            if (M_handle.promise().M_exception) {
                std::rethrow_exception(M_handle.promise().M_exception);
            }
        }

    private:
        Handle M_handle;
};
/** @brief Used for coroutines that pass no data around
 *
 * To use it, define a coroutine that returns `Coroutine`.  To suspend execution
 * within the coroutine, use `co_await std::suspend_always();`.  Then, to
 * actually use it, call the coroutine, store the result in a variable `co`, and
 * then to run it to the next suspension point, use `co()`.
 *
 * As an example, here is some of the test code used to test this class:
 * ```cpp
 * test_value = 0; // A global variable
 * auto co = []() -> Coroutine {
 *     REQUIRE(test_value == 0);
 *     co_await std::suspend_always();
 *     REQUIRE(test_value == 1);
 *     co_await std::suspend_always();
 *     REQUIRE(test_value == 2);
 * }();
 * co();
 * test_value = 1;
 * co();
 * test_value = 2;
 * co();
 * ```
 */
using Coroutine = ArgsCoroutine<>;

/** @brief Used for coroutines that take in arguments each time they are resumed
 * and returns values each time they are suspended
 *
 * To use it, define a coroutine that returns `ArgsGenerator<T, Args...>`.  To
 * suspend execution within the coroutine and return a value, use
 * `co_yield value;`.  Then, to actually use the coroutine, call the coroutine,
 * store the result in a variable `co`, and then to run it to the next
 * suspension point, use `co(args...)`.  It will return an `std::optional<T>`
 * containing the yielded value, or an empty optional if the coroutine has
 * finished.  Within the coroutine, you can access the arguments by using
 * `co_await ArgsGenerator<T, Args...>::get_args()`.  This returns an std::tuple
 * that contains the arguments passed in.  This only needs to be done once, and
 * the values in the tuple will always be updated.  Because this tuple is
 * constantly being assigned to, argument types without assignment operators are
 * not supported (such as reference types).
 *
 * As an example, here is some of the test code used to test this class:
 * ```cpp
 * auto make_gen = []() -> ArgsGenerator<int, int>{
 *     auto& args = co_await ArgsGenerator<int, int>::get_args();
 *     auto& arg = get<0>(args);
 *     while (arg != 0) co_yield arg;
 * };
 * auto gen1 = make_gen();
 * REQUIRE(gen1(1) == 1);
 * REQUIRE(gen1(2) == 2);
 * REQUIRE(gen1(3) == 3);
 * REQUIRE(!gen1(0));
 * REQUIRE(!gen1(1));
 * auto gen2 = make_gen();
 * REQUIRE(!gen2(0));
 * REQUIRE(!gen2(1));
 * ```
 */
template<typename T, typename... Args>
class ArgsGenerator {
    public:
        /// @private
        struct Promise {
            std::optional<T> M_value;
            std::exception_ptr M_exception;
            std::tuple<Args...> M_args;

            ArgsGenerator get_return_object()
            {
                return ArgsGenerator(Handle::from_promise(*this));
            }
            std::suspend_always initial_suspend() {return {};}
            std::suspend_always final_suspend() noexcept {return {};}
            void unhandled_exception()
            {
                M_exception = std::current_exception();
            }

            template<std::convertible_to<T> From>
            std::suspend_always yield_value(From&& from)
            {
                M_value = std::forward<From>(from);
                return {};
            }
            void return_void() {}
        };
        /// @private
        using promise_type = Promise;
        /// @private
        using Handle = std::coroutine_handle<Promise>;

        ArgsGenerator(Handle handle) : M_handle(handle) {}
        ~ArgsGenerator()
        {
            M_handle.destroy();
        }

        struct get_args
        {
            std::tuple<Args...>* M_args;
            bool await_ready() {return false;}
            bool await_suspend(std::coroutine_handle<Promise> h)
            {
                M_args = &h.promise().M_args;
                return false;
            }
            std::tuple<Args...>& await_resume() {return *M_args;}
        };
        std::optional<T> operator()(Args... args)
        {
            if (M_handle.done()) return std::nullopt;
            M_handle.promise().M_args = std::make_tuple(args...);
            M_handle();
            if (M_handle.promise().M_exception) {
                std::rethrow_exception(M_handle.promise().M_exception);
            }
            auto result = std::move(M_handle.promise().M_value);
            M_handle.promise().M_value.reset();
            return result;
        }

    private:
        Handle M_handle;
};

/** @brief Used for coroutines that return values each time they are suspended
 *
 * To use it, define a coroutine that returns `Generator<T>`.  To
 * suspend execution within the coroutine and return a value, use
 * `co_yield value;`.  Then, to actually use the coroutine, call the coroutine,
 * store the result in a variable `co`, and then to run it to the next
 * suspension point, use `co()`.  It will return an `std::optional<T>`
 * containing the yielded value, or an empty optional if the coroutine has
 * finished.
 *
 * As an example, here is some of the test code used to test this class:
 * ```cpp
 * auto gen = []() -> Generator<int>{
 *     co_yield 1;
 *     co_yield 2;
 *     co_yield 3;
 * }();
 * REQUIRE(gen() == 1);
 * REQUIRE(gen() == 2);
 * REQUIRE(gen() == 3);
 * REQUIRE(!gen());
 * ```
 */
template <typename T>
using Generator = ArgsGenerator<T>;

}

#endif
