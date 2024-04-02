#ifndef _StopWatch_hpp_
#define _StopWatch_hpp_

class StopWatch {
    unsigned long _start;
public:
    StopWatch(void) {
        Reset();
    }
    void Reset() {
        _start = millis();
    }
    unsigned long Elapsed() {
        return millis() - _start;
    }
    operator unsigned long() {
        return Elapsed();
    }
    bool IsTimeup(unsigned long time) {
        return Elapsed() >= time;
    }
    // 秒を取得
    unsigned long Second() {
        return Elapsed() / 1000L;
    }
};

#endif