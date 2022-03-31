#include "all.hpp"

class suffix_array {
public:
  suffix_array() {}
  suffix_array(string &text) {
    int n = text.length();
    suffix suffixes[n];
    for (int i = 0; i < n; i++) {
      suffixes[i].index = i;
      suffixes[i].rank[0] = text[i] - 'a';
      suffixes[i].rank[1] = ((i + 1) < n) ? (text[i + 1] - 'a') : -1;
    }
    sort(suffixes, suffixes + n, compare_suffixes);
    int indexes[n];
    for (int k = 4; k < 2 * n; k = k * 2) {
      int rank = 0;
      int prev_rank = suffixes[0].rank[0];
      suffixes[0].rank[0] = rank;
      indexes[suffixes[0].index] = 0;
      for (int i = 1; i < n; i++) {
        if (suffixes[i].rank[0] == prev_rank &&
            suffixes[i].rank[1] == suffixes[i - 1].rank[1]) {
          prev_rank = suffixes[i].rank[0];
          suffixes[i].rank[0] = rank;
        } else {
          prev_rank = suffixes[i].rank[0];
          suffixes[i].rank[0] = ++rank;
        }
        indexes[suffixes[i].index] = i;
      }

      for (int i = 0; i < n; i++) {
        int nextindex = suffixes[i].index + k / 2;
        suffixes[i].rank[1] =
            (nextindex < n) ? suffixes[indexes[nextindex]].rank[0] : -1;
      }
      sort(suffixes, suffixes + n, compare_suffixes);
    }
    vector<int> array;
    for (int i = 0; i < n; i++) {
      array.push_back(suffixes[i].index + 1);
    }
    _array = array;
  }

  vector<int> get_lcp() {
    if (_lcp.size() == _text.size()) {
      return _lcp;
    }
    int n = _array.size();
    vector<int> lcp(n, 0);
    vector<int> inv_suff(n, 0);
    for (int i = 0; i < n; i++) {
      inv_suff[_array[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < n; i++) {
      if (inv_suff[i] == n - 1) {
        k = 0;
        continue;
      }
      int j = _array[inv_suff[i] + 1];
      while (i + k < n && j + k < n && _text[i + k] == _text[j + k]) {
        k++;
      }
      lcp[inv_suff[i]] = k;
      if (k > 0) {
        k--;
      }
    }
    return _lcp = lcp;
  }

  int get_value(int index) { return _array[index]; }

  char get_char(int index) {
    if (index >= _text.lenghth()) {
      return char(0);
    }
    return _text[index];
  }

  int length() { return _text.length(); }

private:
  vector<int> _array;
  vector<int> _lcp;
  string _text;

  struct suffix {
    int index;
    int rank[2];
  };

  static int compare_suffixes(const suffix &a, const suffix &b) {
    return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1] ? 1 : 0)
                                    : (a.rank[0] < b.rank[0] ? 1 : 0);
  }
};
class hasher {
public:
  hasher(std::string &filename) {}

private:
  const int MOD = 1e9 + 7;
  const int p = 1021;
};

class seeker {
public:
  seeker(std::string &filename) {
    _filename = filename;

    std::fstream file(_filename.c_str(), std::fstream::in);
    file.seekp(0, std::ios::end);
    uint64_t filesize = file.tellp();
    file.seekp(0, std::ios::beg);
    _fit_into_memory = (filesize <= SEEKER_BUFFER_SIZE);

    char buffer[SEEKER_BUFFER_SIZE];

    if (_fit_into_memory) {
      file.read(buffer, SEEKER_BUFFER_SIZE);
      string text(buffer);
      suffix_array sa(text);
      _sa = sa;
      _sa_count = 1;

    } else {
      int index = 0;
      while (!file.eof()) {
        file.read(buffer, SEEKER_BUFFER_SIZE);
        string text(buffer);
        suffix_array sa(text);

        index++;
      }
      _sa_count = index;
    }
  }

  int seek(std::string &substring) {
    if (_fit_into_memory) {
      return _sa_binsearch(_sa, substring);
    } else {
    }
    return 0;
  }

private:
  std::string _filename;
  suffix_array _sa;
  int _sa_count;
  bool _fit_into_memory;
  const uint64_t SEEKER_BUFFER_SIZE = 100000;

  void _to_char(char a[], uint64_t number) { memcpy(a, &number, 8); }

  uint64_t _to_uint64_t(char a[]) {
    uint64_t number = 0;
    memcpy(&number, a, 8);
    return number;
  }

  int _sa_binsearch(suffix_array &sa, string &substring) {
    int left = 0;
    int right = substring.length();
    for (int i = 0; i < substring.length(); i++) {

      // lower_bound for left
      int ll = left;
      int rl = right;

      while (ll + 1 < rl) {
        int ml = (ll + rl) >> 1;
        int suffix_index = sa.get_value(ml);
        char symbol = sa.get_char(suffix_index + i);
        if (symbol < substring[i]) {
          ll = ml + 1;
        } else {
          rl = ml;
        }
      }

      // upper_bound for right
      int lr = left;
      int rr = right;

      while (lr + 1 < rr) {
        int mr = (lr + rr) >> 1;
        int suffix_index = sa.get_value(mr);
        char symbol = sa.get_char(suffix_index + i);
        if (symbol <= substring[i]) {
          lr = mr;
        } else {
          rr = mr - 1;
        }
      }

      left = ll;
      right = lr;
    }
    int index = -1;
    for (int i = left; i <= right; i++) {
      index = std::min(index, sa.get_value(i));
    }
    return index;
  }
};