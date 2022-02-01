#pragma once

#include "../../include/libCacheSim/evictionAlgo/L2Cache.h"
#include "bucket.h"
#include "const.h"
#include "obj.h"
#include "segment.h"

/************* feature *****************/
void seg_feature_shift(L2Cache_params_t *params, segment_t *seg);

void seg_hit(L2Cache_params_t *params, cache_obj_t *cache_obj);

void update_train_y(L2Cache_params_t *params, cache_obj_t *cache_obj);

/************* training *****************/
#if TRAINING_DATA_SOURCE == TRAINING_X_FROM_EVICTION
void transform_seg_to_training(cache_t *cache, bucket_t *bucket, segment_t *segment);
#endif

/************* inference *****************/

// void create_data_holder(cache_t *cache);

// void create_data_holder2(cache_t *cache);

void snapshot_segs_to_training_data(cache_t *cache);

void train(cache_t *cache);

void inference(cache_t *cache);

static inline void resize_matrix(L2Cache_params_t *params, feature_t **x_p, pred_t **y_p,
                                 int32_t *size_p, int64_t new_size) {
  learner_t *learner = &params->learner;
  if ((*size_p) != 0) {
    // free previously allocated memory
    DEBUG_ASSERT(*x_p != NULL);
    my_free(sizeof(feature_t) * (*size_p) * learner->n_feature, *x_p);
    my_free(sizeof(pred_t) * (*size_p), *y_p);
  }

  *x_p = my_malloc_n(feature_t, new_size * learner->n_feature);
  *y_p = my_malloc_n(pred_t, new_size);
  *size_p = new_size;
}