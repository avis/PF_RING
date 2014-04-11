/*
 *
 * Copyright 2012 - ntop
 *
 * Authors: Luca Deri <deri@ntop.org>
 *          Alfredo Cardigliano <cardigliano@ntop.org>
 *
 */

#ifndef _PFRING_ZERO_H_
#define _PFRING_ZERO_H_

/* #define DNA_CLUSTER_MAX_NUM_SLAVES              32 */ /* (Defined in pf_ring.h) Due to the 32-bit slave mask */
#define DNA_CLUSTER_MAX_NUM_SOCKETS                32 /* Due to 32-bit interface mask */

/* flags for the 'flag' bitmap parameter of dna_cluster_create() */
#define DNA_CLUSTER_DIRECT_FORWARDING     1 << 0
#define DNA_CLUSTER_NO_ADDITIONAL_BUFFERS 1 << 1
#define DNA_CLUSTER_HUGEPAGES             1 << 2
#define DNA_CLUSTER_DCA                   1 << 3

/* return values for pfring_dna_cluster_distribution_func()  */
#define DNA_CLUSTER_DROP -1
#define DNA_CLUSTER_PASS  0
#define DNA_CLUSTER_FRWD  1

/* return values for pfring_dna_cluster_distribution_func()  */
#define DNA_BOUNCER_DROP -1
#define DNA_BOUNCER_PASS  0

typedef void pfring_dna_bouncer;

typedef enum {
  one_way_mode = 0,
  two_way_mode
} dna_bouncer_mode;

typedef int (*pfring_dna_bouncer_decision_func)(u_int32_t *pkt_len, u_char *pkt, const u_char *user_bytes, u_int8_t direction);

typedef void pfring_dna_cluster;

typedef struct {
  u_int64_t tot_rx_packets;
  u_int64_t tot_tx_packets;
  u_int64_t tot_rx_processed; 
} pfring_dna_cluster_stat;

typedef struct {
  u_int32_t rx_queued;
} pfring_dna_cluster_slave_stat;

typedef struct {
  u_int32_t num_slaves;
  pfring_dna_cluster_slave_stat approximate_stats[DNA_CLUSTER_MAX_NUM_SLAVES];
} pfring_dna_cluster_slaves_info;

typedef int (*pfring_dna_cluster_distribution_func)(const u_char *buffer, const u_int16_t buffer_len, const pfring_dna_cluster_slaves_info *slaves_info, u_int32_t *id_mask, u_int32_t *hash);

/* --------------------- Public API --------------------- */

/* DNA bouncer */
pfring_dna_bouncer *pfring_dna_bouncer_create(pfring *ingress_ring, pfring *egress_ring);
int  pfring_dna_bouncer_set_mode(pfring_dna_bouncer *handle, dna_bouncer_mode mode);
int  pfring_dna_bouncer_loop(pfring_dna_bouncer *handle, pfring_dna_bouncer_decision_func func, const u_char *user_bytes, u_int8_t wait_for_packet);
void pfring_dna_bouncer_breakloop(pfring_dna_bouncer *handle);
void pfring_dna_bouncer_destroy(pfring_dna_bouncer *handle);

/* DNA cluster master */
pfring_dna_cluster* dna_cluster_create(u_int32_t cluster_id, u_int32_t num_apps, u_int32_t flags);
int  dna_cluster_low_level_settings(pfring_dna_cluster *handle, u_int32_t slave_rx_queue_len, u_int32_t slave_tx_queue_len, u_int32_t slave_additional_buffers);
int  dna_cluster_set_hugepages_mountpoint(pfring_dna_cluster *handle, const char *mountpoint);
int  dna_cluster_register_ring(pfring_dna_cluster *handle, pfring *ring);
void dna_cluster_set_cpu_affinity(pfring_dna_cluster *handle, u_int32_t rx_core_id, u_int32_t tx_core_id);
void dna_cluster_set_thread_name(pfring_dna_cluster *handle, const char *rx_thread_name, const char *tx_thread_name);
int  dna_cluster_set_mode(pfring_dna_cluster *handle, socket_mode mode);
void dna_cluster_set_distribution_function(pfring_dna_cluster *handle, pfring_dna_cluster_distribution_func func);
void dna_cluster_set_wait_mode(pfring_dna_cluster *handle, u_int32_t active_wait);
int  dna_cluster_enable(pfring_dna_cluster *handle);
int  dna_cluster_disable(pfring_dna_cluster *handle);
int  dna_cluster_stats(pfring_dna_cluster *handle, pfring_dna_cluster_stat *stats); 
void dna_cluster_destroy(pfring_dna_cluster *handle);

/* -------------------- Internal API -------------------- */

/* DNA cluster slave */
int   pfring_dna_cluster_open(pfring *ring);

#endif /* _PFRING_ZERO_H_ */

