
#include "gbafe.h"

extern u8 Enable1RNFlagLink;
extern u8 EnableFatesRNFlagLink;
extern u8 EnableEvilRNFlagLink;
extern u8 EnableNiceRNGFlagLink;
extern u8 EnableCoinTossRNGFlagLink;
extern u8 EnablePerfectHitFlagLink;

extern bool CheckEventId(int flagID);

int NewRoll2RN(int threshold);
int RollFatesRN(int threshold);
int RollEvilRN(int threshold);

int NewRoll2RN(int threshold) {

	if (CheckEventId(Enable1RNFlagLink)) return Roll1RN(threshold);
	if (CheckEventId(EnableFatesRNFlagLink)) return RollFatesRN(threshold);
	if (CheckEventId(EnableEvilRNFlagLink)) return RollEvilRN(threshold);
	if (CheckEventId(EnableNiceRNGFlagLink) && threshold == 69) return true;
	if (CheckEventId(EnableCoinTossRNGFlagLink)) threshold = 50;
	if (CheckEventId(EnablePerfectHitFlagLink)) { if (threshold != 0) return true; return false; }
		
	int average = (NextRN_100() + NextRN_100()) / 2;

    return (threshold > average);
};

int RollFatesRN(int threshold) {
	
	if (threshold < 50) return (threshold > NextRN_100());
	
	//using the simplified "close enough" formula for performance reasons
	
	int average = (3*NextRN_100() + NextRN_100())/4;
	
	return (threshold > average);
	
}

int RollEvilRN(int threshold) {

	int a = NextRN_100();
	int average = ((3*a*a) - (2*a*a*a) /100 );
	return (threshold*100 > average);
	
}



