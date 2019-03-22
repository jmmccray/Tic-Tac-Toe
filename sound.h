#ifndef SOUND_H_
#define SOUND_H_

typedef enum {note_silent,
              note_c0,
              note_db0,
              note_d0,
              note_b0,
              note_c1,
              note_db1,
              note_d1,
              note_a1,
              note_b1,
              note_c2,
              note_d3, //Here and above I added
              note_c4,
              note_d4,
              note_e4,
              note_f4,
              note_g4,
              note_a4,
              note_b4,
              note_c5,
              note_d5,
              note_e5,
              note_f5,
              note_fs5,
              note_g5,
              note_a5,
              note_b5,
              note_c6} tnote;

void InitSound();
void PlaySound(tnote n, unsigned ms);
void loseSound();
void winSound();
void tieSound();

#endif /* SOUND_H_ */
