#ifndef _Timer_hpp_
#define _Timer_hpp_

class Timer {
    unsigned long _start;
public:
    Timer(void) {
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
        return Elapsed() / 1000;
    }
};

#endif