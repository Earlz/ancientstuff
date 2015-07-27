//See COPYRIGHT.txt for info on copyright
#ifndef _K_MISC_H
//#define _K_MISC_H
#ifdef __cplusplus
extern("C"){
#endif

inline void outportb(unsigned int port,unsigned char value);
inline unsigned char inportb(unsigned int port);
inline void stopints();
void wait(unsigned long ms);









#ifdef __cplusplus
}
#endif

#endif