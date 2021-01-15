int main(){
	int arr2D[3000][500];
	for(int col = 0; col < 500; col++){
		for(int row = 0; row < 3000; row++){
			arr2D[row][col] = row + col;
		}
	}


}
