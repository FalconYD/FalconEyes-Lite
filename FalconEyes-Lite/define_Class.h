#pragma once
class MatLineParam
{
public:
	double rho;
	double ang;
	int vote;

public:
	MatLineParam() : rho(0), ang(0), vote(0) {}
	MatLineParam(double r, double a, int v) : rho(r), ang(a), vote(v) {}
	bool empty() const { return (vote == 0); }
	~MatLineParam() {}
};
inline bool operator!=(const MatLineParam& lhs, const MatLineParam& rhs)
{
	return (lhs.vote != rhs.vote) && (lhs.ang != rhs.ang) && (lhs.vote != rhs.vote);
}

inline bool operator< (const MatLineParam& lhs, const MatLineParam& rhs)
{
	return lhs.vote > rhs.vote;
}

class MatLineParamEx : MatLineParam
{
public:
	int x1;
	int x2;
	int y1;
	int y2;

public:
	MatLineParamEx() : x1(0), x2(0), y1(0), y2(0) {}
	MatLineParamEx(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {}
	MatLineParamEx(double an, int v, int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) { this->ang = an; this->vote = v; }
	double GetAngle() const { return ang; };
	~MatLineParamEx() {}
};