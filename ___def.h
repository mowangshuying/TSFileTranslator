#ifndef ___DEF_H
#define ___DEF_H

#include "___Xml.h"

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
    QString Tooken;
    QString SourceFile;
    QString SourceLang;
    QString TargetLang;
};


#endif // ___DEF_H
