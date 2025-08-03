#ifndef CONFIG_H
#define CONFIG_H

#define REGEX_IP QRegExp("\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b")
#define REGEX_EMAIL QRegExp("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}")
// #define REGEX_URL
// QRegExp("(?:http|https):\/\/(?:www\\.)?[\\w\\d-]+(?:\\.[\\w\\d-]+)*(?:(?::\\d{1,5})?\/\\S*)?")
#define REGEX_URL                                                                                  \
    QRegExp("(?:http|https)://"                                                                    \
            "(?:www\\.)?[\\w\\d-]+(?:\\.[\\w\\d-]+)*(?:(?::\\d{1,5})?/\\S*)?")
// #define REGEX_URL QRegExp("http")
#define REGEX_REGISTRY QRegExp("HKEY_[A-Z_\\\\]+[A-Z]")
#define REGEX_UNIX_PATH QRegExp("/([a-zA-Z0-9_]+/?)+")
#define REGEX_WIN_PATH                                                                             \
    QRegExp("[a-zA-Z]:\\\\(?:[^\\\\/:*?\"<>|\\r\\n]+\\\\)*[^\\\\/:*?\"<>|\\r\\n]*")
#define REGEX_BASE64 QRegExp("^[A-Za-z0-9+/]{4}*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=)?$")

#define DEFAULT_FONT_SIZE 13
#define THREAD_NUM 14

#endif // CONFIG_H
