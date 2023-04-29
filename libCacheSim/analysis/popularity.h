#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <vector>

#include "utils/include/linReg.h"
#include "struct.h"

using namespace std;

namespace analysis {
class PopularityUtils {
 public:
  PopularityUtils() = delete;
  ~PopularityUtils() = delete;
  static double slope(const std::vector<double> &x,
                      const std::vector<double> &y) {
    const auto n = x.size();
    const auto s_x = std::accumulate(x.begin(), x.end(), 0.0);
    const auto s_y = std::accumulate(y.begin(), y.end(), 0.0);
    const auto s_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
    const auto s_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    const auto a = (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
    return a;
  }
};

class Popularity {
 public:
  Popularity() { has_run = false; };
  ~Popularity() = default;

  explicit Popularity(obj_info_map_type &obj_map) { run(obj_map); };

  friend ostream &operator<<(ostream &os, const Popularity &popularity) {
    if (popularity.freq_vec_.empty()) {
      ERROR("popularity has not been computed\n");
      return os;
    }

    if (popularity.fit_fail_reason_.size() > 0)
      os << popularity.fit_fail_reason_ << "\n";
    else
      os << setprecision(4)
         << "popularity: Zipf linear fitting slope=" << popularity.slope_
         << ", intercept=" << popularity.intercept_ << ", R2=" << popularity.r2_
         << "\n";

    return os;
  }

  vector<uint32_t> &get_sorted_freq() { return freq_vec_; }

  void dump(string &path_base) {
    if (freq_vec_.empty()) {
      assert(!has_run);
      ERROR("popularity has not been computed\n");
      return;
    }

    string ofile_path = path_base + ".popularity";
    ofstream ofs(ofile_path, ios::out | ios::trunc);
    ofs << "# " << path_base << "\n";
    ofs << "# freq (sorted):cnt - for Zipf plot\n";

    /* convert sorted freq list to freq:cnt list sorted by freq (to save some
     * space) */
    uint32_t last_freq = freq_vec_[0];
    uint32_t freq_cnt = 0;
    for (auto &cnt : freq_vec_) {
      if (cnt == last_freq) {
        freq_cnt += 1;
      } else {
        ofs << last_freq << ":" << freq_cnt << "\n";
        freq_cnt = 1;
        last_freq = cnt;
      }
    }
    ofs << last_freq << ":" << freq_cnt << "\n";
    ofs.close();
  }

  string fit_fail_reason_ = "";

 private:
  void run(obj_info_map_type &obj_map) {
    /* freq_vec_ is a sorted vec of obj frequency */
    freq_vec_.reserve(obj_map.size());
    for (const auto &p : obj_map) {
      freq_vec_.push_back(p.second.freq);
    }
    sort(freq_vec_.begin(), freq_vec_.end(), greater<>());

    if (obj_map.size() < 200) {
      fit_fail_reason_ = "popularity: too few objects (" +
                         to_string(obj_map.size()) +
                         "), skip the popularity computation";
      WARN("%s\n", fit_fail_reason_.c_str());
      return;
    }

    if (freq_vec_[0] < 200) {
      fit_fail_reason_ = "popularity: the most popular object has " +
                         to_string(freq_vec_[0]) + " requests ";
      WARN("%s\n", fit_fail_reason_.c_str());
    }

    /* calculate Zipf alpha using linear regression */
    vector<double> log_freq(obj_map.size());
    vector<double> log_rank(obj_map.size());

    int i = 0;
    for_each(log_freq.begin(), log_freq.end(),
             [&](double &item) { item = log(freq_vec_[i++]); });
    i = 0;
    for_each(log_rank.begin(), log_rank.end(), [&](double &item) {
      ++i;
      item = log(i);
    });

    /* TODO: a better linear regression with intercept and R2 */
    slope_ = -PopularityUtils::slope(log_rank, log_freq);

    has_run = true;
  }

  vector<uint32_t> freq_vec_{};
  double slope_, intercept_, r2_;
  bool has_run = false;
};
}  // namespace analysis