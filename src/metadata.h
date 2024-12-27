#ifndef METADATA_H_
#define METADATA_H_

#ifdef __cplusplus
extern "C" {
#endif
// Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20, Width, Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status, T1, T2

typedef struct __attribute__((packed)) metadata_struct
{
	int power;
	int M00;
	int M01;
	int M10;
	int M11;
	int M02;
	int M20;
	int width;
	int track_number;
	int frame_max;
	int frame_number;
	int timestamp;
	int io_status;
	int t1;
	int t2;
} metadata_t;

double metadata_get_width(metadata_t* metadata);

#ifdef __cplusplus
}
#endif

#endif