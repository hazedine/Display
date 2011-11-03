#ifndef  DEF_RANDOM_ORDER
#define  DEF_RANDOM_ORDER

typedef  unsigned long  Random_mask_type;

#define  INCREMENT_RANDOM_ORDER( index, mask ) \
         { \
             if( (index) & 1 ) \
                 (index) = ((index) >> 1) ^ (mask); \
             else \
                 (index) = ((index) >> 1); \
         }


#endif
