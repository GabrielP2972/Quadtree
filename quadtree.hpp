#include <iostream>
using namespace std;

template<class T>
class quadtree{
    private:
        T info;
        quadtree<T>* children;
        size_t actualH;
    public:
        quadtree(T info){
            this->info= info;
            children = new quadtree<T>[4];
            actualH=0;
        }

        quadtree(){
            info=NULL;
            children=NULL;
            actualH=0;
        }

        T getInfo(){
            return info;
        }

        quadtree<T>* getNode(T info){
            if(info==this->info) return this;

            quadtree<T>* ref =NULL;
            for(int i=0; i<actualH; i++){
                ref = children[i].getNode(info);
                if(ref!= NULL)return ref;
            }
            return NULL;
        }
        
        void insert(T info){
            if(actualH>=4) return;

            children[actualH] = quadtree<T>(info);
            actualH++;
        }

        void insert(T info, T dad){
            quadtree<T>* father= getNode(dad);
            father->insert(info);
        }


};