/**
 * As you sure already know, data passed via either glBufferData or glBufferSubData by a separated thread (maybe your communication client-server one) cannot
 * be seen early by main thread (maybe your rendering one); therefore it is needed to notify, and in a certain way to synchronize, these threads.
 * Of course, they have to share a context.
 *
 * In this implementation, there are N thread-safe queue of GLsync, one per thread.
 * Every thread has its own queue which can be read only by it, but it cannot write into it.
 *
 * Each set of shared contexts have its own ContextSync instance
 */

#ifndef GRAPHICS_CONTEXTSYNC_HPP
#define GRAPHICS_CONTEXTSYNC_HPP
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace jpl::_graphics::_engine::_sync{

    struct Context {
        const std::thread::id threadId;
        GLFWwindow* window;

        Context(const std::thread::id &threadId, GLFWwindow* window);
        Context(GLFWwindow* window);
        bool operator==(const Context& rhs) const;

        ~Context() = default;
    };

    struct ContextHash {
        size_t operator()(const Context& ctx) const {
            return std::hash<std::thread::id>()(ctx.threadId) ^ (std::hash<std::size_t>()(reinterpret_cast<uintptr_t>(ctx.window)) << 1);
        }
    };

    struct ContextQueue {
        std::mutex mutex;
        std::queue<GLsync> queue;
        std::condition_variable cond;

        ContextQueue();
        ContextQueue(const ContextQueue& ctx);
        ContextQueue& operator=(const ContextQueue& ctx);
        ~ContextQueue() = default;

        void pushNewSync(const GLsync &sync);


        std::optional<GLsync> popLastSync();
        /**
         *  This function does destroy GLsync automatically
         * @param wait calling glClientWaitSync on the popped GLsync
         * @return GLsync or std::nullopt if no one found
         */
        std::optional<GLenum> popLastSyncAndWait(GLbitfield flag, GLuint64 timeout);
    };

    class ContextSync {
        protected:
            std::mutex mutex;
            std::unordered_map<Context, ContextQueue, ContextHash> map;

        public:
            ContextSync();

            /**
             * @throw NotFoundException if there's no ctx in the current map
             * @param ctx
             * @return ctx's queue
             */
            ContextQueue* getContextQueue(const Context &ctx);

            /**
             * Create a new Context based on the given threadId (owner of the window) and window.
             * You should always pass std::current_thread::get_id()
             * @param threadId
             * @param window
             * @return the new context
             */
            void addContext(const std::thread::id &theradId, GLFWwindow* window);

            const Context* getContext(GLFWwindow* window);
            const Context* getContext(std::thread::id threadId);

            /**
             * A thread, which is not the owner of the given dst but means to pass the given sync
             * to the owner, has to call this method
             * @param dst
             * @param sync
             */
            void pushNewSync(const Context* &dst, const GLsync &sync);

            ~ContextSync() = default;
    };

}


#endif