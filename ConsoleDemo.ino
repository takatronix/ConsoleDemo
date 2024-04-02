#include <M5Unified.h>

#include "Device.hpp"
#include "StopWatch.hpp"
#include "Sound.hpp"

#include "Sprite.hpp"
#include "TextSprite.hpp"

// 画像データ
#include "image_wifi.hpp"
// 音声データ
#include "sound_button1.hpp"
Sound soundButtonPush(sound_button1);


M5Canvas console(&M5.Display);
TextSprite batterySprite(&console,25,8);
TextSprite statusSprite(&console,180,8);

StopWatch startTimer;


//M5Canvas canvas(&M5.Display);
// 画面全体にランダムカラーで点を描画(debug用)
void drawRandomPoint(M5Canvas& canvas){
  for(int i = 0; i < 500; ++i){
    int x = random(M5.Display.width());
    int y = random(M5.Display.height());
    canvas.drawPixel(x,y,random(0xFFFF));
  }
}
void setup() {
  auto cfg = M5.config();
  //cfg.external_speaker.hat_spk2 = true;
  M5.begin(cfg);

  M5.Speaker.begin();

  // 起動音  
  Sound::setVolume(255);
  Sound::beep(2000);
  Sound::beep(1000);

  // 回転に対応するため、画面サイズの大きい方に合わせる
  int size = max(M5.Display.width(),M5.Display.height());
  console.setFont(&fonts::lgfxJapanGothic_16);
 // M5Stackの場合8bitカラーにしないとメモリ不足になる
  console.setColorDepth(8);
  if(console.createSprite(size,size) == NULL){
    M5.Display.fillScreen(TFT_RED);
    M5.Display.setTextSize(2);
    Serial.println("createSprite error");
    return;
  }

  console.setTextSize(1);
  console.setTextScroll(true);
  console.setTextColor(GREEN);

  // デバイス情報表示
  printDeviceInformation();
}

void printDeviceInformation(){
  console.setTextColor(TFT_YELLOW);
  console.printf("Board:%s\n",Device::getBoardName());
  console.printf("Screen:%dx%d\n",M5.Display.width(),M5.Display.height());
  console.printf("Battery:%d\n",Device::getBatteryLevel());
  console.printf("Orientation:%s\n",Device::getOrientationName());
  console.printf("FreeMemory:%d\n", Device::getFreeDmaSize());
  console.printf("FreeBlock:%d\n", Device::getLargestFreeBlock());
  console.setTextColor(TFT_GREEN);
}

void loop(void)
{
  StopWatch loopTimer;
  static int processTime = 0;
  static unsigned loopCount = 0;
  int consoleY = 0;
  M5.update();
  
  // 前回の描画を消す
  batterySprite.popBackground();
  statusSprite.popBackground();

  if(Device::wasOrientationChanged()){
    int rotation = Device::getRotation();
    if(rotation != -1){
      console.printf("%s\n",Device::getOrientationName());
      M5.Display.setRotation(rotation);
      // 左下基準に表示
        consoleY = console.height() - M5.Display.height();
      if(console.height() > M5.Display.height()){
        //consoleY *= -1;
      }
      console.setTextColor(CYAN);
      console.printf("display:%dx%d\n",M5.Display.width(),M5.Display.height());
      console.printf("console:%dx%d\n",console.width(),console.height());
      console.printf("consoleY:%d\n",consoleY);
      console.setTextColor(GREEN);
    }
  }

  // ボタンの状態をチェック
  button_handler();
  // タッチの状態をチェック
  touch_handler();
  
  // バッテリー残量描写
  String batteryLevel = String(Device::getBatteryLevel()) + "%";
  batterySprite.update(batteryLevel);
  batterySprite.moveToTopRight();
  batterySprite.push();

  // ステータス表示
  float fps = 0.0f;
  int sec = startTimer.Second();
  if(sec != 0){
    fps = (float)loopCount / (float)sec;
  }
  int mem = Device::getFreeDmaSize() / 1024;
  int freeBlock = Device::getLargestFreeBlock() / 1024;
  int psram = Device::getFreePsramSize() / 1024;

  char buf[256];
  sprintf(buf,"%.1fFPS / %dKB / %dKB / %dms",fps,mem,freeBlock,processTime);
  statusSprite.update(buf);
  statusSprite.moveToTopLeft();
  statusSprite.push();

  // オフスクリーン->LCDへ描画
  console.pushSprite(0,consoleY);
  processTime = loopTimer.Elapsed();
  ++loopCount;
}

void touch_handler(){
  if(!M5.Touch.isEnabled()){
    return;
  }

  auto t = M5.Touch.getDetail();
  M5.Display.setCursor(0, 0);
  if(t.wasPressed()){
    console.printf("Pressed(%d,%d)\n", t.x, t.y);
  }
  if (t.wasReleased()) {
    console.printf("Released(%d,%d)\n", t.x, t.y);
  }
}

// 各ボタンの状態チェック
void button_handler(){

  // ButtonA
  if(M5.BtnA.wasPressed()){
    console.printf("ButtonA Pressed\n");
    Sound::playNote(Note::C4,50);
    M5.Power.setLed(255);
  }
  if(M5.BtnA.wasReleased()){
    Sound::playNote(Note::C5,50);
    M5.Power.setLed(0);
  }
  if(M5.BtnA.wasHold()){
    Sound::playNote(Note::C6,300);
  }

  // ButtonB
  if(M5.BtnB.wasPressed()){
    console.printf("ButtonB Pressed\n");
    Sound::playNote(Note::D4,50);
    printDeviceInformation();
  }
  if(M5.BtnB.wasReleased()){
    Sound::playNote(Note::D5,50);
  }

  // ButtonC
  if(M5.BtnC.wasPressed()){
    console.printf("ButtonC Pressed\n");
    Sound::playNote(Note::E4,50);
  }
  if(M5.BtnC.wasReleased()){
    Sound::playNote(Note::E5,50);
  }

  // PWR Button (M5StickCのみ)
  if(M5.BtnPWR.wasPressed()){
    console.printf("ButtonPWR Pressed\n");
    Sound::playNote(Note::F5,50);
  }
  if(M5.BtnPWR.wasReleased()){
    Sound::playNote(Note::F4,50);
  }

}

