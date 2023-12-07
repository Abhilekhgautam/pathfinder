#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


struct MazeBlock{
    int startX;
    int startY;
    int height;
    int width;
    olc::Pixel background_color;

    bool operator != (MazeBlock b) const{
        if(startX == b.startX && startY == b.startY) return false;
        else return true;
    }

//    std::vector<MazeBlock> getNeighbors(MazeBlock){
//        std::vector<MazeBlock> neighbors;
//
//        neighbors.emplace_back(MazeBlock{startX + width, startY, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX - width, startY, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX , startY - height, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX, startY + height, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX - width, startY - height, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX + width, startY - height, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX - width, startY + height, height, width, olc::WHITE});
//        neighbors.emplace_back(MazeBlock{startX + width, startY + height, height, width, olc::WHITE});
//
//    }
};

class Maze: public olc::PixelGameEngine{
private:
    std::vector<MazeBlock> blocks;
    olc::Pixel fill_color = olc::RED; // color to be filled with
    MazeBlock start_block = {0};
    MazeBlock end_block = {0};
public:
    void DrawBlocks(){
        for (auto block: blocks){
            // kinda bad: It's okay for now
            FillRect(block.startX, block.startY, block.width, block.height, block.background_color);
            DrawRect(block.startX, block.startY, block.width, block.height, olc::BLACK);
        }
    }
    // initialize your app
    bool OnUserCreate() override{
        for(int x = 0 ; x < ScreenWidth() ; x+=10){
            for(int y = 0 ; y < ScreenHeight() ; y+=10){
                MazeBlock my_block{x,y,10,10, olc::WHITE};
                blocks.emplace_back(my_block);
            }
        }
        return true;
    }
    // this fn is called everytime the frame updates
    bool OnUserUpdate(float fElapsedTime) override{
        Clear(olc::WHITE);
        DrawBlocks();
        if (GetMouse(0).bHeld){
            for(auto& b : blocks){
                if(GetMouseX() > b.startX  &&  GetMouseY() >  b.startY &&  GetMouseX() < b.startX + b.width &&  GetMouseY() <  b.startY + b.height){
                    if(start_block.height != 0 and end_block.height != 0) fill_color = olc::YELLOW;

                    if(start_block.height == 0)
                        start_block = b;
                    if(b != start_block && end_block.height == 0) end_block = b;
                    if(b.background_color != olc::RED)
                     b.background_color = fill_color;
                }
            }
        }
        if(GetKey(olc::Y).bPressed){
            fill_color = olc::YELLOW;
        }
        if(GetKey(olc::S).bPressed){
            searchNearestNeighbor(start_block);
        }

        return true;
    }

    void searchNearestNeighbor(MazeBlock maze){
        int distanceFromEndX = abs(end_block.startX - maze.startX);
        int distanceFromEndY = abs(end_block.startY - maze.startY);

        // startX + width, startY
        // startX - width, startY
        // startX, startY - height
        // startX, startY + height
        // startX - width, startX - height
        // startX + width , startX - height
        // startX - width, startY + height
        // startX + width, startY + height

        int closestDistance = 0;
    }
};

int main() {
    Maze myMaze;
    if(myMaze.Construct(400, 400, 4, 4))
        myMaze.Start();
}
