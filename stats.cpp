
#include "stats.h"
	sumRed.clear();
	sumRed.resize(im.height(), vector<long> (im.width(),0));
	sumGreen.clear();
	sumGreen.resize(im.height(), vector<long> (im.width(),0));
	sumBlue.clear();
	sumBlue.resize(im.height(), vector<long> (im.width(),0));
	sumsqRed.clear();
	sumsqRed.resize(im.height(), vector<long> (im.width(),0));
	sumsqGreen.clear();
	sumsqGreen.resize(im.height(), vector<long> (im.width(),0));
	sumsqBlue.clear();
	sumsqBlue.resize(im.height(), vector<long> (im.width(),0));


	for (unsigned int y = 0; y < im.height(); y++) {
		for (unsigned int x = 0; x < im.width(); x++) {

			RGBAPixel * pix = im.getPixel(x,y);

			if ((y == 0) && (x == 0)) {
				sumRed[y][x] = pix->r;
				sumGreen[y][x] = pix->g;
				sumBlue[y][x] = pix->b;
				sumsqRed[y][x] = pix->r * pix->r;
				sumsqGreen[y][x] = pix->g * pix->g;
				sumsqBlue[y][x] = pix->b * pix->b;
			}
			else if (y==0) {
				sumRed[y][x] = sumRed[y][x-1] + pix->r;
				sumGreen[y][x] = sumGreen[y][x-1] + pix->g;
				sumBlue[y][x] = sumBlue[y][x-1] + pix->b;
				sumsqRed[y][x] = sumsqRed[y][x-1] + pix->r * pix->r;
				sumsqGreen[y][x] = sumsqGreen[y][x-1] + pix->g * pix->g;
				sumsqBlue[y][x] = sumsqBlue[y][x-1] + pix->b * pix->b;
			}
			else if (x==0) {
				sumRed[y][x] = sumRed[y-1][x] + pix->r;
				sumGreen[y][x] = sumGreen[y-1][x] + pix->g;
				sumBlue[y][x] = sumBlue[y-1][x] + pix->b;
				sumsqRed[y][x] = sumsqRed[y-1][x] + pix->r * pix->r;
				sumsqGreen[y][x] = sumsqGreen[y-1][x] + pix->g * pix->g;
				sumsqBlue[y][x] = sumsqBlue[y-1][x] + pix->b * pix->b;
			}
			else {
				sumRed[y][x] = sumRed[y][x-1] + sumRed[y-1][x] - sumRed[y-1][x-1] + pix->r;
				sumGreen[y][x] = sumGreen[y][x-1] + sumGreen[y-1][x] - sumGreen[y-1][x-1] + pix->g;
				sumBlue[y][x] = sumBlue[y][x-1] + sumBlue[y-1][x] - sumBlue[y-1][x-1] + pix->b;
				sumsqRed[y][x] = sumsqRed[y][x-1] + sumsqRed[y-1][x] - sumsqRed[y-1][x-1] + pix->r * pix->r;
				sumsqGreen[y][x] = sumsqGreen[y][x-1] + sumsqGreen[y-1][x] - sumsqGreen[y-1][x-1] + pix->g * pix->g;
				sumsqBlue[y][x] = sumsqBlue[y][x-1] + sumsqBlue[y-1][x] - sumsqBlue[y-1][x-1] + pix->b * pix->b;
			}
		}
	}
}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
	if (ul.first==0 && ul.second==0) {
		if (channel=='r') return sumRed[lr.second][lr.first];
		if (channel=='g') return sumGreen[lr.second][lr.first];
		if (channel=='b') return sumBlue[lr.second][lr.first];
	} else if (ul.first==0) {
		if (channel=='r') return sumRed[lr.second][lr.first]-sumRed[ul.second-1][lr.first];
		if (channel=='g') return sumGreen[lr.second][lr.first]-sumGreen[ul.second-1][lr.first];
		if (channel=='b') return sumBlue[lr.second][lr.first]-sumBlue[ul.second-1][lr.first];
	} else if (ul.second==0) {
		if (channel=='r') return sumRed[lr.second][lr.first]-sumRed[lr.second][ul.first-1];
		if (channel=='g') return sumGreen[lr.second][lr.first]-sumGreen[lr.second][ul.first-1];
		if (channel=='b') return sumBlue[lr.second][lr.first]-sumBlue[lr.second][ul.first-1];
	} else {
		if (channel=='r') return sumRed[lr.second][lr.first]-sumRed[lr.second][ul.first-1]-sumRed[ul.second-1][lr.first]+sumRed[ul.second-1][ul.first-1];
		if (channel=='g') return sumGreen[lr.second][lr.first]-sumGreen[lr.second][ul.first-1]-sumGreen[ul.second-1][lr.first]+sumGreen[ul.second-1][ul.first-1];
		if (channel=='b') return sumBlue[lr.second][lr.first]-sumBlue[lr.second][ul.first-1]-sumBlue[ul.second-1][lr.first]+sumBlue[ul.second-1][ul.first-1];
	}
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
	if (ul.first==0 && ul.second==0) {
		if (channel=='r') return sumsqRed[lr.second][lr.first];
		if (channel=='g') return sumsqGreen[lr.second][lr.first];
		if (channel=='b') return sumsqBlue[lr.second][lr.first];
	} else if (ul.first==0) {
		if (channel=='r') return sumsqRed[lr.second][lr.first]-sumsqRed[ul.second-1][lr.first];
		if (channel=='g') return sumsqGreen[lr.second][lr.first]-sumsqGreen[ul.second-1][lr.first];
		if (channel=='b') return sumsqBlue[lr.second][lr.first]-sumsqBlue[ul.second-1][lr.first];
	} else if (ul.second==0) {
		if (channel=='r') return sumsqRed[lr.second][lr.first]-sumsqRed[lr.second][ul.first-1];
		if (channel=='g') return sumsqGreen[lr.second][lr.first]-sumsqGreen[lr.second][ul.first-1];
		if (channel=='b') return sumsqBlue[lr.second][lr.first]-sumsqBlue[lr.second][ul.first-1];
	} else {
		if (channel=='r') return sumsqRed[lr.second][lr.first]-sumsqRed[lr.second][ul.first-1]-sumsqRed[ul.second-1][lr.first]+sumsqRed[ul.second-1][ul.first-1];
		if (channel=='g') return sumsqGreen[lr.second][lr.first]-sumsqGreen[lr.second][ul.first-1]-sumsqGreen[ul.second-1][lr.first]+sumsqGreen[ul.second-1][ul.first-1];
		if (channel=='b') return sumsqBlue[lr.second][lr.first]-sumsqBlue[lr.second][ul.first-1]-sumsqBlue[ul.second-1][lr.first]+sumsqBlue[ul.second-1][ul.first-1];
	}
}

long stats::getScore(pair<int,int> ul, pair<int,int> lr){
	long Sqs = getSumSq('r', ul, lr) + getSumSq('b', ul, lr) + getSumSq('g', ul, lr);
  	long sumSq = getSum('r', ul, lr) * getSum('r', ul, lr) + getSum('b', ul, lr) * getSum('b', ul, lr) + getSum('g', ul, lr) * getSum('g', ul, lr);
	return Sqs - sumSq / rectArea(ul, lr);
}

RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	return RGBAPixel(getSum('r', ul, lr)/rectArea(ul, lr), 
				     getSum('g', ul, lr)/rectArea(ul, lr), 
				     getSum('b', ul, lr)/rectArea(ul, lr));
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}
