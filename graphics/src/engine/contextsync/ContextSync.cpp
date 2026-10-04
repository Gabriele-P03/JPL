#include "ContextSync.hpp"

#include <jpl/exception/runtime/NotFoundException.hpp>
#include <jpl/logger/Logger.hpp>

jpl::_graphics::_engine::_sync::Context::Context(const std::thread::id &threadId, GLFWwindow *window) : threadId(threadId), window(window){
    if (window == nullptr)
        throw jpl::_exception::IllegalArgumentException("GLFWwindow cannot be nullptr");
}
jpl::_graphics::_engine::_sync::Context::Context(GLFWwindow *window) : Context(std::this_thread::get_id(), window){}
bool jpl::_graphics::_engine::_sync::Context::operator==(const Context &rhs) const {
    return this->window == rhs.window;
}


jpl::_graphics::_engine::_sync::ContextQueue::ContextQueue() : mutex(std::mutex()), queue(std::queue<GLsync>()), cond(std::condition_variable()){}
jpl::_graphics::_engine::_sync::ContextQueue::ContextQueue(const ContextQueue &ctx) : mutex(std::mutex()), queue(ctx.queue), cond(std::condition_variable()) {}
jpl::_graphics::_engine::_sync::ContextQueue &jpl::_graphics::_engine::_sync::ContextQueue::operator=(const ContextQueue &ctx) {
    this->queue = ctx.queue;
    return *this;
}

void jpl::_graphics::_engine::_sync::ContextQueue::pushNewSync(const GLsync &sync) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->queue.push(sync);
}

std::optional<GLsync> jpl::_graphics::_engine::_sync::ContextQueue::popLastSync() {
    std::lock_guard<std::mutex> lock(this->mutex);
    if (this->queue.empty())
        return std::nullopt;
    GLsync result = this->queue.front();
    this->queue.pop();
    return result;
}
std::optional<GLenum> jpl::_graphics::_engine::_sync::ContextQueue::popLastSyncAndWait(GLbitfield flag, GLuint64 timeout) {
    std::optional<GLsync> result = this->popLastSync();
    if (result == std::nullopt) {
        return std::nullopt;
    }
    GLenum waitReturn = glClientWaitSync(result.value(), flag, timeout);
    glDeleteSync(result.value());
    return waitReturn;
}

jpl::_graphics::_engine::_sync::ContextSync::ContextSync() : map(std::unordered_map<Context, ContextQueue, ContextHash>()){}

jpl::_graphics::_engine::_sync::ContextQueue *jpl::_graphics::_engine::_sync::ContextSync::getContextQueue(const Context &ctx) {
    if (this->map.contains(ctx))
        return &this->map.at(ctx);
    throw jpl::_exception::NotFoundException("Context for thread not found");
}

void jpl::_graphics::_engine::_sync::ContextSync::addContext(const std::thread::id &threadId, GLFWwindow *window){
    std::lock_guard<std::mutex> lock(this->mutex);
    this->map.insert_or_assign(Context(threadId, window), ContextQueue());
}

void jpl::_graphics::_engine::_sync::ContextSync::pushNewSync(const Context *&dst, const GLsync &sync) {
    std::unique_lock<std::mutex> lock(this->mutex);
    ContextQueue* cq = this->getContextQueue(*dst);
    cq->pushNewSync(sync);
}

const jpl::_graphics::_engine::_sync::Context *jpl::_graphics::_engine::_sync::ContextSync::getContext(std::thread::id threadId) {
    std::unique_lock<std::mutex> lock(this->mutex);
    for (auto & iter : this->map) {
        if (iter.first.threadId == threadId) {
            return &iter.first;
        }
    }
    return nullptr;
}

const jpl::_graphics::_engine::_sync::Context *jpl::_graphics::_engine::_sync::ContextSync::getContext(GLFWwindow *window) {
    std::unique_lock<std::mutex> lock(this->mutex);
    for (auto & iter : this->map) {
        if (iter.first.window == window) {
            return &iter.first;
        }
    }
    return nullptr;
}
