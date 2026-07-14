#ifndef ___DEF_H
#define ___DEF_H

#include "___Xml.h"

enum class LogLevel {
    Trace = 0,
    Info = 1,    // 普通信息 (黑色)
    Warring = 2, // 警告 (橙色)
    Error = 3    // 错误 (红色)
};

// 开始
// 停止
// 继续
// 暂停
enum class TaskButtonType {
    Start,      // 开始
    Stop,       // 完全停止
    Continue,   // 继续
    pause,      // 中途停止
};

class TaskData {
public:
    QString HtttpUrl;
    QString Token;
    QString SourceFile;
    QString SourceLang;
    QString TargetLang;
};


#endif // ___DEF_H
