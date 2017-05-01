/*
 * Copyright 2017 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/browser/memory_settings/pretty_print.h"

#include <algorithm>
#include <string>
#include <vector>

#include "cobalt/browser/memory_settings/memory_settings.h"
#include "cobalt/browser/memory_settings/table_printer.h"
#include "starboard/log.h"
#include "starboard/string.h"

namespace cobalt {
namespace browser {
namespace memory_settings {
namespace {

std::string StringifySourceType(const MemorySetting* setting) {
  if (!setting->valid()) {
    return "N/A";
  }

  switch (setting->source_type()) {
    case MemorySetting::kUnset: {
      return "Unset";
    }
    case MemorySetting::kCmdLine: {
      return "CmdLine";
    }
    case MemorySetting::kBuildSetting: {
      return "Build";
    }
    case MemorySetting::kAutoSet: {
      return "AutoSet";
    }
  }

  SB_NOTIMPLEMENTED() << "Unimplemented string for type: "
                      << setting->source_type();
  return "UNKNOWN";
}

std::string StringifyMemoryType(const MemorySetting* setting) {
  switch (setting->memory_type()) {
    case MemorySetting::kCPU: {
      return "CPU";
    }
    case MemorySetting::kGPU: {
      return "GPU";
    }
    case MemorySetting::kNotApplicable: {
      return "N/A";
    }
  }

  SB_NOTIMPLEMENTED() << "Unimplemented string for memory type: "
                      << setting->memory_type();
  return "UNKNOWN";
}

std::string StringifyValue(const MemorySetting* setting) {
  if (!setting->valid()) {
    return "N/A";
  }
  return setting->ValueToString();
}

std::string ToMegabyteString(int64_t bytes) {
  float megabytes = bytes / (1024.0f * 1024.0f);

  char buff[128];
  SbStringFormatF(buff, sizeof(buff), "%.1f MB", megabytes);
  // Use 16
  return std::string(buff);
}

}  // namespace

std::string GeneratePrettyPrintTable(
    const std::vector<const MemorySetting*>& settings) {
  TablePrinter printer;

  std::vector<std::string> header;
  header.push_back("NAME");
  header.push_back("VALUE");
  header.push_back("");
  header.push_back("TYPE");
  header.push_back("SOURCE");
  printer.AddRow(header);

  for (size_t i = 0; i < settings.size(); ++i) {
    const MemorySetting* setting = settings[i];

    std::vector<std::string> row;
    row.push_back(setting->name());
    row.push_back(StringifyValue(setting));
    if (setting->valid()) {
      row.push_back(ToMegabyteString(setting->MemoryConsumption()));
    } else {
      row.push_back("N/A");
    }
    row.push_back(StringifyMemoryType(setting));
    row.push_back(StringifySourceType(setting));
    printer.AddRow(row);
  }

  std::string table_string = printer.ToString();
  return table_string;
}

std::string GenerateMemoryTable(int64_t total_cpu_memory,
                                base::optional<int64_t> total_gpu_memory,
                                int64_t settings_cpu_consumption,
                                int64_t settings_gpu_consumption) {
  TablePrinter printer;
  std::vector<std::string> header;
  header.push_back("TYPE");
  header.push_back("TOTAL");
  header.push_back("SETTINGS");
  printer.AddRow(header);

  std::vector<std::string> data_row;
  data_row.push_back("CPU");
  data_row.push_back(ToMegabyteString(total_cpu_memory));
  data_row.push_back(ToMegabyteString(settings_cpu_consumption));
  printer.AddRow(data_row);
  data_row.clear();

  data_row.push_back("GPU");

  std::string total_gpu_consumption_str;
  if (!total_gpu_memory) {
    total_gpu_consumption_str = "<UNKNOWN>";
  } else {
    total_gpu_consumption_str = ToMegabyteString(*total_gpu_memory);
  }

  data_row.push_back(total_gpu_consumption_str);
  data_row.push_back(ToMegabyteString(settings_gpu_consumption));
  printer.AddRow(data_row);
  data_row.clear();

  return printer.ToString();
}

}  // namespace memory_settings
}  // namespace browser
}  // namespace cobalt
