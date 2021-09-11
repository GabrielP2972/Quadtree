#include <string>


class Data{
    public:
        double latitude;
        double longitude;
        char *info;

        Data(){
            latitude = 0;
            longitude = 0;
            info = NULL;
        }

        Data(double latitude, double longitude, char *info){
            this->latitude = latitude;
            this->longitude = longitude;
            this->info = info;
        }
};

class BoundingBox {
    public:
        int xt; int yt;
        int xb; int yb;

        BoundingBox(){
            xt = 0; yt = 0;
            xb = 0; yb = 0;
        }

        BoundingBox(int xt, int yt, int xb, int yb){
            this->xt = xt;
            this->yt = yt;
            this->xb = xb;
            this->yb = yb;
        }

        bool Intersects(BoundingBox test) {
            return (this->xt <= test.xb && this->xb >= test.xt && this->yt <= test.yb && this->yb >= test.yt);
        }

        bool ContainsData(Data data) {
            bool containsX = this->xt <= data.latitude && data.latitude <= this->xb;
            bool containsY = this->yt <= data.longitude && data.longitude <= this->yb;
            
            return containsX && containsY; 
        }
};

class Node {
    protected:
        Node *northWest;
        Node *northEast;
        Node *southWest;
        Node *southEast;
        Data *points;
        BoundingBox boundary;
        int count;
    public:
        Node(){
            northWest = NULL;
            northEast = NULL;
            southWest = NULL;
            southEast = NULL;
            boundary = BoundingBox();
            points = NULL;
        }
        Node(BoundingBox boundary) {
            northWest = NULL;
            northEast = NULL;
            southWest = NULL;
            southEast = NULL;
            this->boundary = boundary;
            points = NULL; 
        }
        ~Node(){
            if (northWest != NULL) delete northWest;
            if (northEast != NULL) delete northEast;
            if (southWest != NULL) delete southWest;
            if (southEast != NULL) delete southEast;
            if (points != NULL) delete points;
        }

        bool insert(Data data){
        
            if (!boundary.ContainsData(data))
                return false;
            
            if (this->points != NULL) {
                this->points = *(data);
                return true;
            }
            
            if (this->northWest == NULL)
                this->subdivide();
            
            if (this->northWest->insert(data)) return true;
            if (this->northEast->insert(data)) return true;
            if (this->southWest->insert(data)) return true;
            if (this->southEast->insert(data)) return true;
            
            return false;
        }

        void subdivide(){
        
            BoundingBox box = this->boundary;
            
            int xMid = (box.xb + box.xt)/2;
            int yMid = (box.yb + box.yt)/2;
            
            BoundingBox northWest = BoundingBox(box.xt, box.yt, xMid-1, yMid-1);
            this->northWest = new Node(northWest, this->capacity);
            
            BoundingBox northEast = BoundingBox(xMid+1, box.yt, box.xb, yMid-1);
            this->northEast = new Node(northEast, this->capacity);
            
            BoundingBox southWest = BoundingBox(box.xt, yMid+1, xMid-1, box.yb);
            this->southWest = new Node(southWest, this->capacity);
            
            BoundingBox southEast = BoundingBox(xMid+1, yMid+1, box.xb, box.yb);
            this->southEast = new Node(southEast, this->capacity);
            
        }
    
        Data* getPoints(){
            return this->points; 
        }

        BoundingBox getBoundary(){
            return this->boundary;  
        }

        int getCapacity(){
            return this->capacity;  
        }
};

class QuadTree : public Node {
public:
    QuadTree(Data *data, BoundingBox bourdary, int capacity){
        this->boundary = boundary;
        this->capacity = capacity;
        if(data != NULL)
            for (int i = 0; i < sizeof(data) / sizeof(Data); i++)
                this->insert (data[i]);
    }
    QuadTree(){
        Node();
    }   
};