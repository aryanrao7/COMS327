#ifndef XY_H
#define XY_H

// defines an enumeration and a data structure for handling 2D coordinates.]

typedef enum dim {
  dim_x,       
  dim_y,       
  num_dims     
} dim_t;

// A data structure to hold a pair of 2D coordinates]
typedef int16_t pair_t[num_dims];

#endif
