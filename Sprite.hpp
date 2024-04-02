#ifndef _Sprite_hpp_
#define _Sprite_hpp_
#include <M5Unified.h> 

const uint32_t DEFAULT_COLOR_DEPTH = 16;

class Sprite {
    int _x = 0;
    int _y = 0;
    M5Canvas* parentCanvas;
    bool _backup = false;
public:
    int _depth = DEFAULT_COLOR_DEPTH;
    bool _psram = false;
    M5Canvas  canvas;                   // スプライトのキャンバス
    M5Canvas  backgroundCanvas;         // 画面書き戻し用のキャンバス
    Sprite(M5Canvas* parent)  : parentCanvas(parent) {
        parentCanvas = parent;
     
    }
    Sprite(M5Canvas* parent,int width,int height,int x=0,int y=0,int depth=DEFAULT_COLOR_DEPTH,bool psram = false)  : parentCanvas(parent) {
        parentCanvas = parent;
        if(create(width,height,x,y,depth,psram) == NULL){
            Serial.println("Sprite create error");
        }
    }

    ~Sprite(){
        deinit();
    };
    void deinit(){
        canvas.deleteSprite();
    }
    void pushBackground(void){
        uint8_t* buffer = new uint8_t[width()*height()*_depth/8];
        parentCanvas->readRect(_x,_y,width(),height(),buffer);
        backgroundCanvas.pushImage(0,0,width(),height(),buffer);
        delete[] buffer;
        _backup = true;
    }
    void popBackground(void){
        if(_backup){
            uint8_t* buffer = new uint8_t[width()*height()*_depth/8];
            backgroundCanvas.readRect(0,0,width(),height(),buffer); 
            parentCanvas->pushImage(_x,_y,width(),height(),buffer);
            delete[] buffer;
            _backup = false;
        }
    }

    uint32_t convertColor(uint32_t color){
        if(_depth == 8)
            return M5.Lcd.color16to8(color);
        return color;
    }

    void clear(uint16_t color = TFT_BLACK){
        canvas.setCursor(0,0);
        if(_depth == 8){
            canvas.fillSprite(M5.Lcd.color16to8(color));
            return;
        }
        canvas.fillSprite(color);
    }

    /// @brief          スプライトを作成する
    /// @param width    作成するスプライトの幅
    /// @param height   作成するスプライトの高さ
    /// @param x        作成するスプライトのX座標
    /// @param y        作成するスプライトのY座標
    /// @param depth    作成するスプライトの色深度(8bitでないとメモリ不足になる可能性がある)
    /// @param psram    PSRAMを使用するかどうか
    /// @return         true:成功 false:失敗
    bool create(int width,int height,int x = 0,int y = 0,int depth = 8,bool psram = false){
        _x = x;
        _y = y;
        _depth = depth;
        _psram = psram;

        canvas.setPsram(psram);
        canvas.setColorDepth(depth);
        if(canvas.createSprite(width,height) == NULL){
            Serial.println("Sprite create error");
            Sound::errorBeep();
            return false;
        }

        backgroundCanvas.setPsram(psram);
        backgroundCanvas.setColorDepth(depth);
        if(backgroundCanvas.createSprite(width,height) == NULL){
            Serial.println("Back Sprite create error");
            canvas.deleteSprite();
            Sound::errorBeep();
            return false;
        }

        return true;
    }

#pragma region Property
    int x(){
        return _x;
    }
    void x(int value){
        _x = value;
    }
    int y(){
        return _y;
    }
    void y(int value){
        _y = value;
    }
    int width(){
        return canvas.width();
    }
    int height(){
        return canvas.height();
    }
#pragma endregion

#pragma region Push
    void push(void){
        pushBackground();
        canvas.pushSprite(parentCanvas,_x,_y);
    }
    void push(int x,int y){
        _x = x;
        _y = y;        
        canvas.pushSprite(parentCanvas,x,y);
    }
#pragma endregion

#pragma region Move
    void moveTo(int x,int y){
        _x = x;
        _y = y;
    }

    void moveToTopLeft(void){
        _x = 0;
        _y = 0;
    }
    void moveToTopRight(void){
        _x = M5.Display.width() - width();
        _y = 0;
    }
    void moveTopCenter(void){
        _x = (M5.Display.width() - width()) / 2;
        _y = 0;
    }
    void moveToBottomLeft(void){
        _x = 0;
        _y = M5.Display.height() - height();
    }
    void moveToBottomRight(void){
        _x = M5.Display.width() - width();
        _y = M5.Display.height() - height();
    }
    void moveToBottomCenter(void){
        _x = (M5.Display.width() - width()) / 2;
        _y = M5.Display.height() - height();
    }
    void moveToCenter(void){
        _x = (M5.Display.width() - width()) / 2;
        _y = (M5.Display.height() - height()) / 2;
    }
#pragma endregion
};
#endif