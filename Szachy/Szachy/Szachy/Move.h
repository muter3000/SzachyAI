#pragma once
class Move
{
private:
	/* Bits: 0-5-Start
	*		 6-11-Target
	*		 12
	*		 13-15 SpecialMoveFlag 0-nothing 1-ep 2-castle 3-prom 4-double pawn move
	*		 16-19 Piece
	*		 20 Capture flag
	*		 21-24 PromotionTarget
	*		 25-26 CastleType  0-Q 1-K 2-q 3-k
	*/
	unsigned int data;
public:
	Move() :data(0) {};
	Move(int _data) :data(_data) {}
	unsigned int getData() { return data; }
	void setData(unsigned int _data) { data=_data; }
	void operator=(unsigned int _data) { data = _data; }
};

