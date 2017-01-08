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
#ifndef COBALT_BASE_C_VAL_COLLECTION_ENTRY_STATS_H_
#define COBALT_BASE_C_VAL_COLLECTION_ENTRY_STATS_H_

#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>
#include <vector>

#include "base/logging.h"
#include "base/stringprintf.h"
#include "base/time.h"
#include "cobalt/base/c_val.h"

namespace base {

// This class tracks a collection of entries, which it retains in memory. When
// either the max size of the collection is reached or Flush() is manually
// called, the count, average, minimum, maximum, 25th, 50th, 75th and 95th
// percentiles, and standard deviation of the collection are recorded with
// CVals, and the tracking resets in preparation for the next collection of
// entries.
// NOTE1: By default there is no max size and the collection will continue to
//        grow indefinitely until Flush() is called.
// NOTE2: This class keeps all of the entries in memory until flush is called so
//        that percentiles can be determined. In cases where the number of
//        entries is extremely large, |CValTimeIntervalEntryStats| is more
//        appropriate, as it does its tracking without keeping entries in memory
//        (at the cost of not being able to provide percentiles);
template <typename EntryType, typename Visibility = CValDebug>
class CValCollectionEntryStats {
 public:
  static const size_t kNoMaxSize = 0;

  CValCollectionEntryStats(const std::string& name,
                           size_t max_size = kNoMaxSize);

  // Add an entry to the collection. This may trigger a Flush() if adding the
  // entry causes the max size to be reached.
  void AddEntry(const EntryType& value);
  // Manually flush the collection's entries. This updates the stat cvals and
  // clears the entries.
  void Flush();

 private:
  typedef std::vector<EntryType> CollectionType;

  static EntryType CalculatePercentile(const CollectionType& sorted_collection,
                                       int percentile);
  static double CalculateStandardDeviation(const CollectionType& collection,
                                           double mean);

  // The maximum size of the collection before Flush() is automatically called.
  const size_t max_size_;
  // The current collection of entries. These will be used to generate the cval
  // stats during the next call of Flush().
  CollectionType collection_;

  // CVals of the stats for the previous collection.
  base::CVal<size_t, Visibility> count_;
  base::CVal<EntryType, Visibility> average_;
  base::CVal<EntryType, Visibility> minimum_;
  base::CVal<EntryType, Visibility> maximum_;
  base::CVal<EntryType, Visibility> percentile_25th_;
  base::CVal<EntryType, Visibility> percentile_50th_;
  base::CVal<EntryType, Visibility> percentile_75th_;
  base::CVal<EntryType, Visibility> percentile_95th_;
  base::CVal<EntryType, Visibility> standard_deviation_;
};

template <typename EntryType, typename Visibility>
CValCollectionEntryStats<EntryType, Visibility>::CValCollectionEntryStats(
    const std::string& name, size_t max_size /*=kNoMaxSize*/)
    : max_size_(max_size),
      count_(StringPrintf("%s.Cnt", name.c_str()), 0, "Total entries."),
      average_(StringPrintf("%s.Avg", name.c_str()), EntryType(),
               "Average value."),
      minimum_(StringPrintf("%s.Min", name.c_str()), EntryType(),
               "Minimum value."),
      maximum_(StringPrintf("%s.Max", name.c_str()), EntryType(),
               "Maximum value."),
      percentile_25th_(StringPrintf("%s.Pct.25th", name.c_str()), EntryType(),
                       "25th percentile value."),
      percentile_50th_(StringPrintf("%s.Pct.50th", name.c_str()), EntryType(),
                       "50th percentile value."),
      percentile_75th_(StringPrintf("%s.Pct.75th", name.c_str()), EntryType(),
                       "75th percentile value."),
      percentile_95th_(StringPrintf("%s.Pct.95th", name.c_str()), EntryType(),
                       "95th percentile value."),
      standard_deviation_(StringPrintf("%s.Std", name.c_str()), EntryType(),
                          "Standard deviation of values.") {}

template <typename EntryType, typename Visibility>
void CValCollectionEntryStats<EntryType, Visibility>::AddEntry(
    const EntryType& value) {
  collection_.push_back(value);
  if (collection_.size() == max_size_) {
    Flush();
  }
}

template <typename EntryType, typename Visibility>
void CValCollectionEntryStats<EntryType, Visibility>::Flush() {
  if (collection_.size() == 0) {
    return;
  }

  // Sort the collection. This allows min, max, and percentiles to be easily
  // determined.
  std::sort(collection_.begin(), collection_.end());

  // Calculate the mean of the collection.
  EntryType sum = std::accumulate(collection_.begin(), collection_.end(),
                                  CValDetail::FromDouble<EntryType>(0));
  double mean = CValDetail::ToDouble<EntryType>(sum) / collection_.size();

  // Update the collection stat cvals.
  count_ = collection_.size();
  average_ = CValDetail::FromDouble<EntryType>(mean);
  minimum_ = collection_.front();
  maximum_ = collection_.back();
  percentile_25th_ = CalculatePercentile(collection_, 25);
  percentile_50th_ = CalculatePercentile(collection_, 50);
  percentile_75th_ = CalculatePercentile(collection_, 75);
  percentile_95th_ = CalculatePercentile(collection_, 95);
  standard_deviation_ = CValDetail::FromDouble<EntryType>(
      CalculateStandardDeviation(collection_, mean));

  collection_.clear();
}

template <typename EntryType, typename Visibility>
EntryType CValCollectionEntryStats<EntryType, Visibility>::CalculatePercentile(
    const CollectionType& sorted_collection, int percentile) {
  DCHECK_GT(sorted_collection.size(), 0);
  DCHECK(percentile >= 0 && percentile <= 100);

  // Determine the position of the percentile within the collection.
  double percentile_position =
      (sorted_collection.size() - 1) * static_cast<double>(percentile) * 0.01;

  // Split out the integral and fractional parts of the percentile position.
  double percentile_integral_position, percentile_fractional_position;
  percentile_fractional_position =
      std::modf(percentile_position, &percentile_integral_position);

  int percentile_first_index = static_cast<int>(percentile_integral_position);

  // If |percentile_first_index| maps to the last entry, then there is no
  // second entry and there's nothing to interpolate; simply use the last entry.
  if (sorted_collection.size() == percentile_first_index + 1) {
    return sorted_collection.back();
  }

  // Interpolate between the two entries that the percentile falls between.
  double first_data_point = CValDetail::ToDouble<EntryType>(
                                sorted_collection[percentile_first_index]) *
                            (1.0 - percentile_fractional_position);
  double second_data_point =
      CValDetail::ToDouble<EntryType>(
          sorted_collection[percentile_first_index + 1]) *
      percentile_fractional_position;

  return CValDetail::FromDouble<EntryType>(first_data_point +
                                           second_data_point);
}

template <typename EntryType, typename Visibility>
double
CValCollectionEntryStats<EntryType, Visibility>::CalculateStandardDeviation(
    const CollectionType& collection, double mean) {
  if (collection.size() <= 1) {
    return 0;
  }

  double dif_squared_sum = 0;
  for (size_t i = 0; i < collection.size(); ++i) {
    double dif = CValDetail::ToDouble<EntryType>(collection[i]) - mean;
    dif_squared_sum += dif * dif;
  }

  double variance =
      dif_squared_sum / static_cast<double>(collection.size() - 1);
  variance = std::max(variance, 0.0);

  return std::sqrt(variance);
}

}  // namespace base

#endif  // COBALT_BASE_C_VAL_COLLECTION_ENTRY_STATS_H_
