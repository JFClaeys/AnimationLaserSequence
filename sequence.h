
/*
 
typedef struct {
  byte Data;
  byte Delay;   // ms
  SeqType seqtype;
} TSeqElem;

 */
#define sqDATA 0
#define sqCHAR 1

const TSeqElem sequences[] PROGMEM  = {
  { 0b11111111,  20, sqDATA },  // full open
  { 0b00000000,  20, sqDATA },  // full close
  { 0b11111111,  20, sqDATA },  // full open
  { 0b00000000,  20, sqDATA },  // full close
  { 0b11111111,  20, sqDATA },  // full open
  { 0b00000000,  20, sqDATA },  // full close
  { 0b11111111,  20, sqDATA },  // full open
  { 0b00000000,  20, sqDATA },  // full close
  { 0b11111111,  20, sqDATA },  // full open
  { 0b00000000, 100, sqCHAR }   // full close
};

/*
const TSeqElem sequences[] PROGMEM  = {
  { 0b11111111,  200,sqDATA},  // full open
  { 0b00000000,  200,sqDATA},  // full close
  { 0b10101010,  20, sqDATA },  // 1 closed, one opened
  { 0b01010101,  20, sqDATA },  // one opened , one closed
  { 0b10101010,  20, sqDATA },  // 1 closed, one opened
  { 0b01010101,  20, sqDATA },  // one opened , one closed
  { 0b10101010,  20, sqDATA },  // 1 closed, one opened
  { 0b01010101,  20, sqDATA },   // one opened , one closed
  { 0b11111111,  200,sqDATA},  // full open
  { 0,           50, sqCHAR }
};*/
