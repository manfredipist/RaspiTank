#include "pimap.h"
#include "ui_pimap.h"

PiMapCell::PiMapCell(const int &i, const int &j, QObject *parent) : QObject(parent), x(i), y(j){
    qGRectItem = new QGraphicsRectItem(i*CELL_SIZE,j*CELL_SIZE,CELL_SIZE,CELL_SIZE);

    if(i==STARTX && j==STARTY)
        qGRectItem->setBrush(QBrush(Qt::blue));
    else
        qGRectItem->setBrush(QBrush(Qt::gray));

    confidence = 1.0;
}

PiMapCell::~PiMapCell(){
    delete qGRectItem;
}

void PiMapCell::setConfidence(const double &confidence){
    this->confidence = confidence;
    updateQGRectItem();
}

void PiMapCell::decreaseConfidence(){
    if(this->confidence>LASER_CONFIDENCE)
        this->confidence -= LASER_CONFIDENCE;
    else
        this->confidence = 0.0;
    updateQGRectItem();
}

void PiMapCell::updateQGRectItem(){
    if(confidence==1)
        qGRectItem->setBrush(QBrush(Qt::white));
    else if(confidence>0.5)
        qGRectItem->setBrush(QBrush(Qt::lightGray));
    else if(confidence==0)
        qGRectItem->setBrush(QBrush(Qt::red));
    else if(confidence<0.5)
        qGRectItem->setBrush(QBrush(Qt::yellow));
}

PiMap::PiMap(const char &orientation, const int &startX, const int &startY, QWidget *parent) : QWidget(parent), ui(new Ui::PiMap){
    ui->setupUi(this);

    QGraphicsView *view = ui->graphicsView;
    QGraphicsScene *scene = new QGraphicsScene(0, 0, CELL_SIZE*CELLS_COUNT, CELL_SIZE*CELLS_COUNT);

    for(int i=0;i<CELLS_COUNT;i++){
        QHash<int, PiMapCell*> row;
        for(int j=0;j<CELLS_COUNT;j++){
            PiMapCell *cell = new PiMapCell(i,j);
            scene->addItem(cell->getQGRectItem());
            row.insert(j,cell);
        }
        grid.insert(i,row);
    }

    this->currentOrientation = orientation;
    this->currentX = startX;
    this->currentY = startY;

    view->fitInView(0, 0, CELL_SIZE*CELLS_COUNT, CELL_SIZE*CELLS_COUNT,Qt::KeepAspectRatio);

    view->setScene(scene);
    view->scale(1,-1);
}


PiMap::~PiMap()
{
    for(int i=0;i<CELLS_COUNT;i++){
        for(int j=0;j<CELLS_COUNT;j++)
            grid[i][j]->deleteLater();
    }

    delete ui;
}

void PiMap::setNewHopePosition(const int &newX, const int &newY, const char &newOrientation){
    if(newX >=0 && newX < CELLS_COUNT && newY >= 0 && newY < CELLS_COUNT){
        grid[currentX][currentY]->getQGRectItem()->setBrush(QBrush(Qt::white));
        grid[newX][newY]->getQGRectItem()->setBrush(QBrush(Qt::blue));

        currentX = newX;
        currentY = newY;
        currentOrientation = newOrientation;
    }
    else
        qDebug()<<"Hope went out of the map!";
}

void PiMap::changeOrientation(const char &orientation){
    switch(orientation){
        case 'n':
            this->currentOrientation='n';
        break;
        case 'e':
            this->currentOrientation='e';
        break;
        case 's':
            this->currentOrientation='s';
        break;
        case 'w':
            this->currentOrientation='w';
        break;
    }
}

bool PiMap::hasEnoughConfidenceToMove(const double &newX, const double &newY){
    if(newX >= 0 && newX < CELLS_COUNT && newY >= 0 && newY < CELLS_COUNT)
    {
        if(grid[newX][newY]->getConfidence()>SAFETY_CONFIDENCE)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool PiMap::moveIfSafe(const QString &direction){
    if(direction=="forward"){
        switch(currentOrientation){
            case 'n':
                if(hasEnoughConfidenceToMove(currentX, currentY+1))
                    setNewHopePosition(currentX, currentY+1, currentOrientation);
                else
                    return false;
            break;
            case 'e':
                if(hasEnoughConfidenceToMove(currentX+1, currentY))
                    setNewHopePosition(currentX+1, currentY, currentOrientation);
                else
                    return false;
            break;
            case 's':
                if(hasEnoughConfidenceToMove(currentX, currentY-1))
                    setNewHopePosition(currentX, currentY-1, currentOrientation);
                else
                    return false;
            break;
            case 'w':
                if(hasEnoughConfidenceToMove(currentX-1, currentY))
                    setNewHopePosition(currentX-1,currentY, currentOrientation);
                else
                    return false;
            break;
        }
    }
    else if(direction=="backward"){
        switch(currentOrientation){
            case 'n':
                if(hasEnoughConfidenceToMove(currentX, currentY-1))
                    setNewHopePosition(currentX, currentY-1, currentOrientation);
                else
                    return false;
            break;
            case 'e':
                if(hasEnoughConfidenceToMove(currentX-1, currentY))
                    setNewHopePosition(currentX-1, currentY, currentOrientation);
                else
                    return false;
            break;
            case 's':
                if(hasEnoughConfidenceToMove(currentX, currentY+1))
                    setNewHopePosition(currentX, currentY+1, currentOrientation);
                else
                    return false;
            break;
            case 'w':
                if(hasEnoughConfidenceToMove(currentX+1, currentY))
                    setNewHopePosition(currentX+1, currentY, currentOrientation);
                else
                    return false;
            break;
        }
    }
    else if(direction=="left"){
        switch(currentOrientation){
            case 'n':
                if(hasEnoughConfidenceToMove(currentX-1, currentY))
                    setNewHopePosition(currentX-1, currentY, 'w');
                else
                    return false;
            break;
            case 'e':
                if(hasEnoughConfidenceToMove(currentX, currentY+1))
                    setNewHopePosition(currentX, currentY+1, 'n');
                else
                    return false;
            break;
            case 's':
                if(hasEnoughConfidenceToMove(currentX+1, currentY))
                    setNewHopePosition(currentX+1, currentY, 'e');
                else
                    return false;
            break;
            case 'w':
                if(hasEnoughConfidenceToMove(currentX, currentY-1))
                    setNewHopePosition(currentX, currentY-1, 's');
                else
                    return false;
            break;
        }
    }
    else if(direction=="right"){
        switch(currentOrientation){
            case 'n':
                if(hasEnoughConfidenceToMove(currentX+1, currentY))
                    setNewHopePosition(currentX+1, currentY, 'e');
                else
                    return false;
            break;
            case 'e':
                if(hasEnoughConfidenceToMove(currentX, currentY-1))
                    setNewHopePosition(currentX, currentY-1, 's');
                else
                    return false;
            break;
            case 's':
                if(hasEnoughConfidenceToMove(currentX-1, currentY))
                    setNewHopePosition(currentX-1, currentY, 'w');
                else
                    return false;
            break;
            case 'w':
                if(hasEnoughConfidenceToMove(currentX, currentY+1))
                    setNewHopePosition(currentX, currentY+1, 'n');
                else
                    return false;
            break;
        }
    }
    return true;
}


void PiMap::setObstacleOnMap(const double &newX, const double &newY){
    if(newX >= 0 && newX < CELLS_COUNT && newY >= 0 && newY < CELLS_COUNT)
        grid[newX][newY]->decreaseConfidence();
}

void PiMap::updateObstaclesOnMap(const double &front_laser, const double &left_laser, const double &right_laser){
    int front_laser_offset = -1;
    int left_laser_offset = -1 , right_laser_offset = -1;

    if(front_laser > 0 && front_laser < 200)
        front_laser_offset = ceil(front_laser/CELL_SIZE);
    if(left_laser> 0 && left_laser < 140)
        left_laser_offset = ceil(left_laser/CELL_SIZE);
    if(right_laser> 0 && right_laser < 140)
        right_laser_offset = ceil(right_laser/CELL_SIZE);

    switch(currentOrientation){
        case 'n':
            if(front_laser_offset>0)
                setObstacleOnMap(currentX,currentY+front_laser_offset);
            if(left_laser_offset>0)
                setObstacleOnMap(currentX-1,currentY+left_laser_offset);
            if(right_laser_offset>0)
                setObstacleOnMap(currentX+1,currentY+right_laser_offset);
        break;
        case 'e':
            if(front_laser_offset>0)
                setObstacleOnMap(currentX+front_laser_offset,currentY);
            if(left_laser_offset>0)
                setObstacleOnMap(currentX+left_laser_offset,currentY+1);
            if(right_laser_offset>0)
                setObstacleOnMap(currentX+right_laser_offset,currentY-1);
        break;
        case 's':
            if(front_laser_offset>0)
                setObstacleOnMap(currentX,currentY-front_laser_offset);
            if(left_laser_offset>0)
                setObstacleOnMap(currentX+1,currentY-left_laser_offset);
            if(right_laser_offset>0)
                setObstacleOnMap(currentX-1,currentY-right_laser_offset);
        break;
        case 'w':
            if(front_laser_offset>0)
                setObstacleOnMap(currentX-front_laser_offset,currentY);
            if(left_laser_offset>0)
                setObstacleOnMap(currentX-left_laser_offset,currentY-1);
            if(right_laser_offset>0)
                setObstacleOnMap(currentX-right_laser_offset,currentY+1);
        break;
    }
}

