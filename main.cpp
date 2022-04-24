#include <stdio.h>
#include<stdlib.h>
#define LEFT 1
#define MID 2 
#define RIGHT 3

typedef struct node
{
	int dataLeft;
	int dataRight;
	struct node *left,*mid,*right;
}node;

node * alloc(int left, int right);

node * alloc(int left, int right){
	node *T=(node*)malloc(sizeof(node));
	T->dataLeft=left;
	T->dataRight=right;
	T->left=NULL;
	T->mid=NULL;
	T->right=NULL;
}


int determinePosition(node *T, int num){
	int leftExisted = T->dataLeft != NULL;
	int rightExisted = T->dataRight != NULL;
	if(!leftExisted){
		return NULL;
	}
	if(T->dataLeft == num){
		return NULL;
	}
	if(rightExisted && T->dataRight == num){
		return NULL;
	}
	
	if(num < T->dataLeft){
		return LEFT;
	}else if(rightExisted){
		if(num>T->dataRight){
			return RIGHT;
		}
		return MID;
	}else{
		return MID;
	}
}

node * findLeaf(node *head, int num){
	
	if(head == NULL){
		return NULL;
	}
	int pos = determinePosition(head,num);
	
	if(pos==LEFT){
		if(head->left == NULL){
			return head;
		}
		return findLeaf(head->left, num);
	}
	if(pos==MID){
		if(head->mid == NULL){
			return head;
		}
		return findLeaf(head->mid, num);
	}
	if(pos==RIGHT){
		if(head->right == NULL){
			return head;
		}
		return findLeaf(head->right, num);
	}
	return NULL;
}

node * findLeafParent(node *head, int num, node* parent){
	
	if(head == NULL){
		return NULL;
	}
	int pos = determinePosition(head,num);
	
	if(pos==LEFT){
		if(head->left == NULL){
			return parent;
		}
		return findLeafParent(head->left, num, head);
	}
	if(pos==MID){
		if(head->mid == NULL){
			return parent;
		}
		return findLeafParent(head->mid, num, head);
	}
	if(pos==RIGHT){
		if(head->right == NULL){
			return parent;
		}
		return findLeafParent(head->right, num, head);
	}
	return parent;
}

void split(node **head, node *pos, node *replacer){
	
	int num = replacer->dataLeft;
	int mid,left,right;
	if(replacer->dataLeft < pos->dataLeft){
		mid = pos->dataLeft;
		left = replacer->dataLeft;
		right = pos->dataRight;
	}else if(replacer->dataLeft < pos->dataRight){
		mid = replacer->dataLeft;
		left = pos->dataLeft;
		right = pos->dataRight;
	}else{
		mid = pos->dataRight;
		right = replacer->dataLeft;
		left = pos->dataLeft;
	}
	
	node *splitMid= alloc(mid, NULL);
	node *splitLeft = alloc(left, NULL);
	node *splitRight = alloc(right, NULL);
	
	if(replacer->dataLeft == mid){
		splitLeft->left = pos->left;
		splitLeft->mid = replacer->left;
		
		splitRight->left = replacer->mid;
		splitRight->mid = pos->right;
		
	}else if(replacer->dataLeft == left){
		splitLeft->left = replacer->left;
		splitLeft->mid = replacer->mid;
		
		splitRight->left = pos->mid;
		splitRight->mid = pos->right;
	}else if(replacer->dataLeft == right){
		splitLeft->left = pos->left;
		splitLeft->mid = pos->mid;
		
		splitRight->left = replacer->left;
		splitRight->mid = replacer->mid;
	}
	
	splitMid->left = splitLeft;
	splitMid->mid = splitRight;
	
	node *temp = *head;
	node *parent = findLeafParent(temp, mid, NULL);
	
	if(parent != NULL && parent->dataRight == NULL){
		int isLeft = parent->dataLeft > num;
		if(isLeft){
			parent->left = splitLeft;
			parent->right = parent->mid;
			parent->mid = splitRight;
			parent->dataRight = parent->dataLeft;
			parent->dataLeft = mid;
		}else{
			parent->mid = splitLeft;
			parent->right = splitRight;
			parent->dataRight = mid;
		}
		
		free(pos);
		return;
	}else if(parent == NULL){
		*head = splitMid;
		free(pos);
		return;
	}
	
	free(pos);
	split(head,parent,splitMid);
}

void insert(node **head, int num){
	if(*head == NULL){
		*head = alloc(num,NULL);
		return;
	}
	node *pos = findLeaf(*head, num);
	if(pos!=NULL){
		//case 1: jika leaf baru diisi satu data
		if(pos->dataRight==NULL){
			
			int isLeft = pos->dataLeft > num;
			if(isLeft){
				pos->dataRight = pos->dataLeft;
				pos->dataLeft = num;
			}else{
				pos->dataRight = num;
			}
			
			return;
		}
		node *replacer= alloc(num, NULL);
		
		//case 2 & 3: jika leaf sudah diisi 2 data
		split(head,pos,replacer);
	}
}

void preorder(node *tree, char label){
	if(tree!=NULL){
		printf("\n%c: ",label);
		printf("[%d",tree->dataLeft);
		if(tree->dataRight != NULL){
			printf(",%d",tree->dataRight);
		}
		printf("]");
		preorder(tree->left, 'L');
		preorder(tree->mid, 'M');
		preorder(tree->right, 'R');
	}
}

int main(void){
	node *tree = NULL;
	insert(&tree, 23);
	insert(&tree, 36);
	insert(&tree, 45);
	insert(&tree, 98);
	insert(&tree, 12);
	insert(&tree, 10);
	insert(&tree, 8);
	insert(&tree, 40);
	insert(&tree, 55);
	insert(&tree, 60);
	insert(&tree, 74);
	insert(&tree, 80);
	
	printf("\nPREORDER: ");
	preorder(tree,'R');
	
	return 0;
}

