#include "logger.h"

#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

void Logger::init() {
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("mylog.txt", 1024 * 1024 * 3, 3);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
    loggerInstance_ = std::make_shared<spdlog::async_logger>("imgui_base91", sinks.begin(), sinks.end(),
                                                             spdlog::thread_pool(),
                                                             spdlog::async_overflow_policy::discard_new);
    spdlog::register_logger(loggerInstance_);
}

auto Logger::getInstance() const {
    return loggerInstance_;
}
