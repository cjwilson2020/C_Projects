
#include <stdio.h>

int main(){
	int n = 10;
	int exN = 0;
	for (int i = 0; i  < n+1; i++){
		printf(example(i));
	}

}

int example(int n){
	if (n = 0){
		return 0;
	}
	if (n >= 1){
		return example(n/2)+example(n-1);
	}

}
