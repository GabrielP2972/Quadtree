#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class Info{
    public:
        int y;
        int x;
        char info;
        bool empty;

        Info(){
            y = 0;
            x = 0;
            info = 0;
            empty= true;
        }

        Info(int y, int x, char info){
            this->y = y;
            this->x = x;
            this->info = info;
            empty=false;
        }

        bool compare(Info test){
            return y == test.y && x == test.x;
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

        void redimencionar(int xt, int yt, int xb, int yb){
            this->xt = xt;
            this->yt = yt;
            this->xb = xb;
            this->yb = yb;
        }

        bool Intersects(BoundingBox test) {
            return (this->xt <= test.xb && this->xb >= test.xt && this->yt <= test.yb && this->yb >= test.yt);
        }

        bool ContainsInfo(Info info) {
            bool containsX = xt <= info.x && info.x <= xb;
            bool containsY = yt <= info.y && info.y <= yb;
            
            return containsX && containsY; 
        }
};

class Quad {
    private:
        Quad *northWest;
        Quad *northEast;
        Quad *southWest;
        Quad *southEast;
        Info info;
        BoundingBox boundary;
        BoundingBox MaxBoundary;
        bool in;
        bool cap;
    public:
        Quad(){
            northWest = NULL;
            northEast = NULL;
            southWest = NULL;
            southEast = NULL;
            boundary = BoundingBox();
            info = Info();
            in = true;
            cap = false;
        }
        Quad(BoundingBox boundary, BoundingBox MaxBoundary) {
            northWest = NULL;
            northEast = NULL;
            southWest = NULL;
            southEast = NULL;
            this->boundary = boundary;
            this->MaxBoundary = MaxBoundary;
            info = Info(); 
            in = true;
            cap = false;
        }

        bool isleaf(){
            return (northWest==NULL && northEast==NULL && southWest==NULL && southEast==NULL);
        }

        void eraseQuad(){
            if(northWest!=NULL){
                info = northWest->info;
                if(northWest->isleaf())
                    northWest = NULL;
                else
                    northWest->eraseQuad();
            }
            if(northEast!=NULL){
                info = northEast->info;
                if (northEast->isleaf())
                    northEast = NULL;
                else
                    northEast->eraseQuad();
            }
            if(southWest!=NULL){
                info = southWest->info;
                if (southWest->isleaf())
                    southWest = NULL;
                else
                    southWest->eraseQuad();
            }
            if(southEast!=NULL){
                info = southEast->info;
                if (southEast->isleaf())
                    southEast = NULL;
                else
                    southEast->eraseQuad();
            }
        }

        bool insert(Info info){
            if (this->info.empty){
                this->info.info = info.info;
                this->info.x = info.x;
                this->info.y = info.y;
                this->info.empty = false;
                return true;
            }

            int mid_x = (boundary.xt + boundary.xb)/2, mid_y = (boundary.yt + boundary.yb)/2;

            BoundingBox cuadrante = BoundingBox(boundary.xt, boundary.yt, mid_x, mid_y);//Northwest
            if(cuadrante.ContainsInfo(info)){
                if(northWest == NULL)
                    northWest = new Quad(cuadrante, MaxBoundary);
                if(northWest->insert(info)){
                    return true;
                }

            }

            cuadrante.redimencionar(mid_x+1, boundary.yt, boundary.xb, mid_y);//Northeast
            if(cuadrante.ContainsInfo(info)){
                if(northEast == NULL)
                    northEast = new Quad(cuadrante, MaxBoundary);
                if(northEast->insert(info)){
                    return true;
                }

            }

            cuadrante.redimencionar(boundary.xt, mid_y+1, mid_x, boundary.yb);//Southwest
            if(cuadrante.ContainsInfo(info)){
                if(southWest == NULL)
                    southWest = new Quad(cuadrante, MaxBoundary);
                if(southWest->insert(info)){
                    return true;
                }

            }

            cuadrante.redimencionar(mid_x+1, mid_y+1, boundary.xb, boundary.yb);//Southeast
            if(cuadrante.ContainsInfo(info)){
                if(southEast == NULL)
                    southEast = new Quad(cuadrante, MaxBoundary);
                if(southEast->insert(info)){
                    return true;
                }

            }

            return false;
        }
    
        void move(int neo_x, int neo_y){
            if(in && !cap){
                info.y += neo_y;
                info.x += neo_x;
            }
            
            if(!MaxBoundary.ContainsInfo(info))
                in = false;

            if(isleaf())
                return;

            if(northWest != NULL){
                northWest->boundary.xt+=neo_x;
                northWest->boundary.xb+=neo_x;
                northWest->boundary.yb+=neo_y;
                northWest->boundary.yt+=neo_y;
                northWest->move(neo_x, neo_y);
            }
                
            if(northEast != NULL){
                northEast->boundary.xt+=neo_x;
                northEast->boundary.xb+=neo_x;
                northEast->boundary.yb+=neo_y;
                northEast->boundary.yt+=neo_y;
                northEast->move(neo_x, neo_y);
            }
                
            if(southWest != NULL){
                southWest->boundary.xt+=neo_x;
                southWest->boundary.xb+=neo_x;
                southWest->boundary.yb+=neo_y;
                southWest->boundary.yt+=neo_y;
                southWest->move(neo_x, neo_y);
            }
                
            if(southEast != NULL){
                southEast->boundary.xt+=neo_x;
                southEast->boundary.xb+=neo_x;
                southEast->boundary.yb+=neo_y;
                southEast->boundary.yt+=neo_y;
                southEast->move(neo_x, neo_y);
            }
        }

        int colicion(Quad test, int total){
            if(!test.cap){ 
                if(test.in)
                    if(info.compare(test.info)){
                        if(rand()%4 == 0){
                            test.cap=true;
                            total++;
                        }
                    }

                if(test.northWest != NULL)
                    if(test.northWest->boundary.ContainsInfo(info))
                        total+=colicion(*(test.northWest), total);
                    
                if(test.northEast != NULL)
                    if(test.northEast->boundary.ContainsInfo(info))
                        total+=colicion(*(test.northEast), total);
                    
                if(test.southWest != NULL)
                    if(test.southWest->boundary.ContainsInfo(info))
                        total+=colicion(*(test.southWest), total);
                    
                if(test.southEast != NULL)
                    if(test.southEast->boundary.ContainsInfo(info))
                        total+=colicion(*(test.southEast), total);
            } 
            return total;
        } 

        Quad* getNorthw(){
            return northWest;
        }
        
        Quad* getNorthe(){
            return northEast;
        }
        
        Quad* getSouthw(){
            return southWest;
        }
        
        Quad* getSouthe(){
            return southEast;
        }
        
        Info getInfo(){
            return info; 
        }

        BoundingBox getBoundary(){
            return this->boundary;  
        }
};


int totalcatch(Quad H, Quad M){
	int total=0;
	total+=H.colicion(M, total);
	if(H.getNorthw()!=NULL){
        total+=totalcatch(*(H.getNorthw()), M);
    }
		
	if(H.getNorthe()!=NULL){
        total+=totalcatch(*(H.getNorthe()), M);
    }
		
	if(H.getSouthw()!=NULL){
        total+=totalcatch(*(H.getSouthw()), M);
    }
		
	if(H.getSouthe()!=NULL){
        total+=totalcatch(*(H.getSouthe()), M);
    }
		
	return total;
}

bool checkS(int test){
	switch (test){
		case -1:
			return true;
			break;
		case 0:
			return true;
			break;
		case 1:
			return true;
			break;
		default:
			return false;
			break;
	}
}

/*
i
M N
x_fb y_fb
x_m y_m
<mapa>
*/

int main (){
    srand(42);
	string map;
	long long i;
	int N, M, x_fb, y_fb, x_m, y_m;
	bool good = true;
    cin>>i;
    cin>>M>>N;
	BoundingBox limite = BoundingBox(0, 0, N-1, M-1);
	Quad hab = Quad(limite, limite);
	Quad med = Quad(limite, limite);
    cin>>x_fb>>y_fb;
    cin>>x_m>>y_m;
	for (int j = 0; j < M; j++){
        cin>>map;
		for (int k = 0; k < N; k++){
			char aux=map[k];
			Info info = Info(j, k, aux);
			switch (aux){
				case 'h':
					hab.insert(info);
					break;
				case 'm':
					med.insert(info);	
					break;
				case '.':
					/* nada */
					break;
				default:
					good=false;
					break;
			}
		}		
	}

	if(i>0){
        if(M>0 && N>0){
            if(checkS(x_fb) && checkS(y_fb) && checkS(x_m) && checkS(y_m)){
                if(good){
                    int total=0;
                    for (int j=1; j<=i ; j++){
                        int cach=0;
                        hab.move(x_fb, y_fb);
                        med.move(x_m, y_m);
                        cach+=totalcatch(hab, med);
                        cout<<"#"<<j<<" Medusas Capturadas: "<<cach<<endl;
                        total+=cach;
                    }
                    cout<<"Total de Medusas Capturadas: "<<total;
                }else
		            cout<<"NO VALIDO";
            }else
                cout<<"NO VALIDO";
        }else
            cout<<"NO VALIDO";
    }else
        cout<<"NO VALIDO";
        
	return 0;
}
