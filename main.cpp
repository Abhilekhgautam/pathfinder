#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


struct MazeBlock{
    int index;
    int startX;
    int startY;
    int height;
    int width;
    olc::Pixel background_color;

    bool operator != (MazeBlock b) const{
        if(startX == b.startX && startY == b.startY) return false;
        else return true;
    }

    bool operator == (MazeBlock b) const{
        if(startX == b.startX && startY == b.startY) return true;
        else return false;
    }

    MazeBlock& operator = (MazeBlock b){
        startX = b.startX;
        startY = b.startY;
        height = b.height;
        width = b.width;
        index = b.index;
        background_color = b.background_color;

        return *this;
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
    int nRows = ScreenHeight() / 2;
    int nCols = ScreenWidth() / 2;
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
                MazeBlock my_block{y + (x*nRows)/10,x,y,10,10, olc::WHITE};
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
    std::vector<int> getNeighbor(MazeBlock b) const{
       std::vector<int> neighborIndex;
        int bottomRight = b.index + nRows + 1;
        int bottomLeft  = b.index - nRows + 1;
        int left = b.index - nRows;
        int right = b.index + nRows;
        int bottom = b.index + 1;
        int top = b.index - 1;
        int topRight = b.index + nRows - 1;
        int topLeft = b.index - nRows - 1;

        if(bottomRight < 0 || bottomRight > nRows * nCols){}
        else neighborIndex.emplace_back(bottomRight);

        if(bottomLeft < 0 || bottomLeft > nRows * nCols){}
        else neighborIndex.emplace_back(bottomLeft);

        if(left < 0 || left > nRows * nCols){}
        else neighborIndex.emplace_back(left);

        if(right < 0 || right > nRows * nCols){}
        else neighborIndex.emplace_back(right);

        if(top < 0 || top > nRows * nCols){}
        else neighborIndex.emplace_back(top);

        if(bottom < 0 || bottom > nRows * nCols){}
        else neighborIndex.emplace_back(bottom);

        if(topRight < 0 || topRight > nRows * nCols){}
        else neighborIndex.emplace_back(topRight);

        if(topLeft < 0 || topLeft > nRows * nCols){}
        else neighborIndex.emplace_back(topLeft);


        return neighborIndex;
    }

    MazeBlock searchNearestNeighbor(MazeBlock maze){
        if(maze == end_block) return end_block;
        else{
            int distanceFromEndX = abs(end_block.startX - maze.startX);
            int distanceFromEndY = abs(end_block.startY - maze.startY);

            std::vector<int> neighbors = getNeighbor(maze);
            MazeBlock nearestBlock = blocks[neighbors[0]];
            int nearestBlockIndex = neighbors[0];

            for(int index: neighbors){
                if(abs(blocks[index].startX - distanceFromEndX) < abs(nearestBlock.startX - distanceFromEndX)
                   and abs(blocks[index].startY - distanceFromEndY) < abs(nearestBlock.startY - distanceFromEndY)){
                    blocks[index].background_color = olc::BLACK;
                    nearestBlock = blocks[index];
                    nearestBlockIndex = index;
                }
            }
            blocks[nearestBlockIndex].background_color = olc::YELLOW;
            return searchNearestNeighbor(nearestBlock);
        }


        // startX + width, startY
        // startX - width, startY
        // startX, startY - height
        // startX, startY + height
        // startX - width, startX - height
        // startX + width , startX - height
        // startX - width, startY + height
        // startX + width, startY + height

    }
};

int main() {
    Maze myMaze;
    if(myMaze.Construct(400, 400, 4, 4))
        myMaze.Start();
}
