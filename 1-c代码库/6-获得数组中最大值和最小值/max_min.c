
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
using namespace std;
 
int arr[10];
void getMax_Min(int left,int right,int &maxNum,int &minNum){
	if(left==right){
		maxNum=arr[left];
		minNum=arr[left];
		return ;
	}else if(left+1==right){
		maxNum=arr[left]>arr[right]?arr[left]:arr[right];
		minNum=arr[left]>arr[right]?arr[right]:arr[left];
		return ;
	}else{
		int mid=(left+right)/2;
		int leftMax,leftMin,rightMax,rightMin;
		getMax_Min(left,mid,leftMax,leftMin);//递归找出左边的最大最小值 
		getMax_Min(mid,right,rightMax,rightMin);//递归找出右边的最大最小值 
		maxNum=max(leftMax,rightMax);//左右两边最大值相比较，取最大的 
		minNum=min(leftMin,rightMin);//左右两边最小值相比较，取最小的 
	}
}
 
int main(){
	for(int i=0;i<10;i++){
		arr[i]=rand();//生成随机数 
		cout<<arr[i]<<" ";	
	}
	cout<<endl;
	int maxNum,minNum;
	getMax_Min(0,9,maxNum,minNum);
	cout<<"最大值是："<<maxNum<<" 最小值是："<<minNum<<endl;
	return 0;
}