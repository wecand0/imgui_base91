#ifndef IMGUI_BASE91_LOGGER_H
#define IMGUI_BASE91_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/async_logger.h"

class Logger {
public:
    Logger() = default;
    void init();
    [[nodiscard]] auto getInstance() const;

private:
    std::shared_ptr<spdlog::async_logger> loggerInstance_;
};


#endif //IMGUI_BASE91_LOGGER_H
