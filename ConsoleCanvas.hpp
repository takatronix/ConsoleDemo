
class ConsoleCanvas : public M5Canvas {
public:
    ConsoleCanvas(M5Display* pDisplay):M5Canvas(pDisplay){
    }

    void create(int width,int height,int x = 0,int y = 0,int depth = 8,bool psram = false){
      //  M5Canvas::createSprite(width,height);
    }
};