#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/cfg/env.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void cds_test_01();
void cds_test_02();
void cds_test_03();
void cds_test_04();