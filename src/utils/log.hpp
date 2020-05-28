
namespace logging {

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#define LOG_INFO(msg)                    \
  (Log(logging::LogData<logging::None>() \
       << BOLDWHITE << "[INFO] " << msg << RESET))

#define LOG_INFO_WITH_CONTEXT(msg)                      \
  (Log(logging::LogData<logging::None>()                \
       << BOLDWHITE << "[INFO] " << msg << RESET << " " \
       << __PRETTY_FUNCTION__))

#define LOG_WARNING(msg)                                                       \
  (Log(logging::LogData<logging::None>()                                       \
       << BOLDYELLOW << "[WARNING] " << msg << RESET << " " << __FILE__ << ":" \
       << __LINE__))

#define LOG_ERROR(msg)                                                    \
  (Log(logging::LogData<logging::None>()                                  \
       << BOLDRED << "[ERROR] " << msg << RESET << " " << __FILE__ << ":" \
       << __LINE__))

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
#ifdef __ICC
#define NOINLINE_ATTRIBUTE __attribute__((noinline))
#else
#define NOINLINE_ATTRIBUTE
#endif  // __ICC
#endif  // NOINLINE_ATTRIBUTE

struct None {};

template <typename List>
struct LogData {
  List list;
};

template <typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(
    LogData<Begin>&& begin, Value&& value) noexcept {
  return {{std::forward<Begin>(begin.list), std::forward<Value>(value)}};
}

template <typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(
    LogData<Begin>&& begin, const char (&value)[n]) noexcept {
  return {{std::forward<Begin>(begin.list), value}};
}

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template <typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(
    LogData<Begin>&& begin, PfnManipulator value) noexcept {
  return {{std::forward<Begin>(begin.list), value}};
}

template <typename Begin, typename Last>
void output(std::ostream& os, std::pair<Begin, Last>&& data) {
  output(os, std::move(data.first));
  os << data.second;
}

inline void output(std::ostream& os, None) {}

template <typename List>
void Log(LogData<List>&& data) NOINLINE_ATTRIBUTE {
  output(std::cout, std::move(data.list));
  std::cout << std::endl;
}

}  // namespace logging