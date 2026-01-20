
#include "loggingUtils.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <iostream>

bool LoggingUtils::initLogger(const std::string &loggerPath) {

  auto maxSize = 1048576 * 5;
  auto maxFiles = 3;
  try {
    auto logger =
        spdlog::rotating_logger_mt("baseLogger", loggerPath, maxSize, maxFiles);
    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::flush_on(spdlog::level::err);
  } catch (const spdlog::spdlog_ex &ex) {
    std::cout << "Log init falied: " << ex.what() << std::endl;
    return false;
  }

  return true;
}
