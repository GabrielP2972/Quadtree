#include <iostream>
#include <stdlib.h>
#include <string>
#include "QuadTree.hpp"
using namespace std;

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

bool checkMap(string test, int n){
	bool keep;
	for (int i = 0; i < n; i++){	
		switch (test[i]){
			case '.':
				keep = true;
				break;
			case 'h':
				keep = true;
				break;
			case 'm':
				keep = true;
				break;
			default:
				keep = false;
				break;
		}
		if(!keep)return false;
	}
	return true;
}

/*
i
M N
x_fb y_fb
x_m y_m
<mapa>
*/

int main (){

	string map;
	unsigned long long i;
	int N, M, x_fb, y_fb, x_m, y_m;
	bool good = true;
	cin>>i;
	cin>>M>>N;	
	BoundingBox limites = BoundingBox(0, 0, N-1, M-1);
	QuadTree tree = QuadTree(NULL, limites, 1) ;
	cin>>x_fb>>y_fb;
	cin>>x_m>>y_m;
	good = checkS(x_fb) && checkS(y_fb) && checkS(x_m) && checkS(y_m);
	map = new string[M];
	for (int j = 0; j < M; j++){
		cin>>map;
		if(good){
			good = checkMap(map, N);
		}else
			break;
		for (int k = 0; k < N; k++){
			if(map[k] == 'h' || map[k] == 'm'){
				Data data = Data(j, k, map[k]);
				tree.insert(data);
			}
		}		
	}

	if(good && i>0 && M>0 && N>0){
		for (int j=0; j<i ; j++){
			/* code */
		}
	}else
		cout<<"NO VALIDO";
	return 0;
}
