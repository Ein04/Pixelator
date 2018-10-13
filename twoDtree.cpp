#include "twoDtree.h"


twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}


twoDtree::~twoDtree(){
	clear();
}


twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ 
	stats st = stats(imIn); 
	height = imIn.height();
	width = imIn.width();
	root = buildTree(st,pair<int,int>(0,0),pair<int,int>(width-1,height-1));
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	Node * newNode = new Node(ul, lr, s.getAvg(ul, lr));

	if (ul != lr) {
		pair<int,int> bestLR;
		pair<int,int> bestUL;
		//pair<pair<int,int>, pair<int,int>> split;		
		long bestScore = LONG_MAX;

		// traverse through vertical split
		for(int i = ul.first; i < lr.first; i++) {
			long currScore = s.getScore(ul, pair<int,int>(i,lr.second)) + 
							 s.getScore(pair<int,int>(i+1,ul.second),lr);
			if (currScore <= bestScore) {
				bestScore = currScore;
				bestUL = pair<int,int>(i+1,ul.second);
				bestLR = pair<int,int>(i,lr.second);
			}
		}
		// traverse through horizontal split
		for(int i = ul.second; i < lr.second; i++) {
			long currScore = s.getScore(pair<int,int>(ul.first,i+1), lr) + 
							 s.getScore(ul, pair<int,int>(lr.first,i));
			if (currScore <= bestScore) {
				bestScore = currScore;
				bestUL = pair<int,int>(ul.first,i+1);
				bestLR = pair<int,int>(lr.first,i);
			}
		}
		// split by the best
		newNode->left = buildTree(s, ul, bestLR);
		newNode->right = buildTree(s, bestUL, lr);
	}
	return newNode;
}


PNG twoDtree::render(){
	PNG newPic(width,height);
	renderNode(newPic,root);
	return newPic;
}

void twoDtree::renderNode(PNG & pic, Node* n) {
	if (n == NULL) return;
	if (!n->left && !n->right) {  
		//leaf node
		for (int i = n->upLeft.first; i <= n->lowRight.first; i++) {
			for (int j = n->upLeft.second; j <= n->lowRight.second; j++) {
				*pic.getPixel(i,j) = n->avg;  
			}
		}
	} else {
		//non-leaf node
		if (n->left) renderNode(pic,n->left);
		if (n->right) renderNode(pic,n->right);
	}
}


void twoDtree::prune(double pct, int tol){
	pruneNode(pct,tol,root);
}

void twoDtree::pruneNode(double pct, int tol, Node* n){
	if (!n->left && !n->right) {
		return;
	} else {
		pair<int,int> leavesAcc = leavesWithinTol(tol,n,n);
		double pctInTol = leavesAcc.first/double(leavesAcc.second);
		if (pctInTol >= pct) {
			clearNode(n->left);
			clearNode(n->right);
		} else {
			pruneNode(pct,tol,n->left);
			pruneNode(pct,tol,n->right);
		}
	}
}

pair<int,int> twoDtree::leavesWithinTol(int tol, Node* rootN, Node* node){
	if (!node->left && !node->right) {
		if (withinTol(tol,rootN->avg,node->avg)) {
			return pair<int,int>(1,1);
		} else {
			return pair<int,int>(0,1);
		}
	} else {
		pair<int,int> leftP = leavesWithinTol(tol,rootN,node->left);
		pair<int,int> rightP = leavesWithinTol(tol,rootN,node->right);
		return pair<int,int>((leftP.first + rightP.first),
			(leftP.second + rightP.second));
	}
}

bool twoDtree::withinTol(int tol, RGBAPixel rootAvg, RGBAPixel leafAvg) {
	long distance = (rootAvg.r-leafAvg.r)*(rootAvg.r-leafAvg.r) + (rootAvg.g-leafAvg.g)*(rootAvg.g-leafAvg.g) + (rootAvg.b-leafAvg.b)*(rootAvg.b-leafAvg.b);
	return distance <= tol; 
}

void twoDtree::clear() {
	clearNode(root);
}

void twoDtree::clearNode(Node*& n) {
	if (n) {
		clearNode(n->left);
		clearNode(n->right);
		delete n;
		n = NULL;
	}
}

void twoDtree::copy(const twoDtree & orig){
	root = copyNode(orig.root);
	width = orig.width;
	height = orig.height;
}

twoDtree::Node * twoDtree::copyNode(Node* n) {
	if (n) {
		Node* newNode = new Node(n->upLeft,n->lowRight,n->avg);
		newNode->left = copyNode(n->left);
		newNode->right = copyNode(n->right);
		return newNode;
	} else {
		return NULL;
	}
}



