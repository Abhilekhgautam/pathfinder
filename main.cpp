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
    // hard coded, pretty bad :(
    int nRows = 40;
    int nCols = 40;
public:
    Maze(){
        sAppName = "Path Finder";
    }
    void DrawBlocks(){
        for (auto block: blocks){
            // kinda bad: It's okay for now
            FillRect(block.startX, block.startY, block.width, block.height, block.background_color);
            DrawRect(block.startX, block.startY, block.width, block.height, olc::BLACK);
        }
    }
    // initialize your app
    bool OnUserCreate() override{
        for(int x = 0, addToX = 0 ; x < nCols ; x++, addToX+=10){
            for(int y = 0, addToY = 0 ; y < nRows ; y++, addToY += 10){
                MazeBlock my_block{y + (x*nRows),addToX,addToY,10,10, olc::WHITE};
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
            // just check if start and end blocks are selected
            if(start_block.height != 0 && end_block.height != 0)
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

        if(bottomRight < 0 || bottomRight >= nRows * nCols || (b.index % nRows == nRows - 1 || b.index > ((nRows - 1) + ((nCols - 2)* nRows)))){}
        else neighborIndex.emplace_back(bottomRight);

        if(bottomLeft < 0 || bottomLeft >= nRows * nCols || (b.index % nRows == nRows - 1 || b.index <= nCols - 1)){}
        else neighborIndex.emplace_back(bottomLeft);

        // probable bug:
        if(left < 0 || left >= nRows * nCols || b.index <= nCols - 1){}
        else neighborIndex.emplace_back(left);

        if(right < 0 || right >= nRows * nCols || b.index > ((nRows - 1) + ((nCols - 2)* nRows))){}
        else neighborIndex.emplace_back(right);

        if(top < 0 || top >= nRows * nCols || b.index % nRows == 0){}
        else neighborIndex.emplace_back(top);

        if(bottom < 0 || bottom >= nRows * nCols || b.index % nRows == nRows - 1){}
        else neighborIndex.emplace_back(bottom);

        if(topRight < 0 || topRight >= nRows * nCols || b.index % nRows == 0 || b.index > ((nRows - 1) + ((nCols - 2)* nRows))){}
        else neighborIndex.emplace_back(topRight);

        // probable bug
        if(topLeft < 0 || topLeft >= nRows * nCols || b.index % nRows == 0 || b.index % nRows == 0){}
        else neighborIndex.emplace_back(topLeft);


        return neighborIndex;
    }

    MazeBlock searchNearestNeighbor(MazeBlock maze){
        if(maze == end_block) return end_block;
        else{

            std::vector<int> neighbors = getNeighbor(maze);
            MazeBlock nearestBlock = blocks[neighbors[0]];

            int distanceFromEndX = abs(end_block.startX - nearestBlock.startX);
            int distanceFromEndY = abs(end_block.startY - nearestBlock.startY);
            int totalCost = distanceFromEndX + distanceFromEndY;

            int nearestBlockIndex = neighbors[0];

            for(int index: neighbors){
                int cost = abs(blocks[index].startX - end_block.startX) + abs(blocks[index].startY - end_block.startY);
                if(cost <= totalCost && blocks[index].background_color != olc::GREEN && blocks[index].background_color != olc::YELLOW){
                    nearestBlock = blocks[index];
                    nearestBlockIndex = index;
                }
                if(blocks[index].background_color != olc::RED && blocks[index].background_color != olc::YELLOW){
                    blocks[index].background_color = olc::GREEN;
                }
            }
            blocks[start_block.index].background_color = olc::RED;
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
