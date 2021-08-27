#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <string>
#include <vector>

/* ---------- combo_arch ---------- */

// combo_arch options: {<name>, <noc_width>}
const std::vector<std::vector<std::string>> combo_arch_to_nocw = {
    {"leon3",  "32"},
    {"ariane", "64"},
    {"ibex",   "32"}
};



/* ---------- Cache Configuration ---------- */

/* ----- l2 ----- */
// l2_ways
const std::vector<std::string> l2_ways = {
    "2 ways",
    "4 ways",
    "8 ways"
};

// l2_ways_i
const unsigned l2_ways_i = 1;

// l2_sets
const std::vector<std::string> l2_sets = {
    "32 sets",
    "64 sets",
    "128 sets",
    "256 sets",
    "512 sets",
    "1024 sets",
    "2048 sets",
    "4096 sets",
    "8192 sets"
};

// l2_sets_i
const unsigned l2_sets_i = 4;

/* ----- llc ----- */
// llc_ways
const std::vector<std::string> llc_ways = {
    "4 ways",
    "8 ways",
    "16 ways"
};

// llc_ways_i
const unsigned llc_ways_i = 2;

// llc_sets
const std::vector<std::string> llc_sets = {
    "32 sets",
    "64 sets",
    "128 sets",
    "256 sets",
    "512 sets",
    "1024 sets",
    "2048 sets",
    "4096 sets",
    "8192 sets"
};

// llc_sets_i
const unsigned llc_sets_i = 5;

/* ----- al2 ----- */
// al2_ways
const std::vector<std::string> al2_ways = {
    "2 ways",
    "4 ways",
    "8 ways"
};

// al2_ways_i
const unsigned al2_ways_i = 1;

// al2 sets
const std::vector<std::string> al2_sets = {
    "32 sets",
    "64 sets",
    "128 sets",
    "256 sets",
    "512 sets",
    "1024 sets",
    "2048 sets",
    "4096 sets",
    "8192 sets"
};

// al2_sets_i
const unsigned al2_sets_i = 4;

/* ----- implem ----- */
// implem
const std::vector<std::string> implem = {
    "ESP RTL",
    "SPANDEX HLS",
    "ESP HLS"
};
/*
    "SystemVerilog",
    "SystemC (HLS)",
*/

/* ----- RTL ----- */
// implem_to_RTL
const std::vector<std::vector<std::string>> implem_to_RTL = {
    {"ESP RTL", "y"},
    {"SPANDEX HLS", "n"},
    {"ESP HLS", "n"}
};

/* ----- combo_slm ----- */
const std::vector<std::string> combo_slm = {
    "64",
    "128",
    "256",
    "512",
    "1024",
    "2048",
    "4096"
};

// combo_slm_i
const unsigned combo_slm_i = 2;

/* ----- combo_data ----- */
const std::vector<std::string> combo_data = {
    "Bigphysical area",
    "Scatter/Gather"
};

// combo_data_i
const unsigned combo_data_i = 0;

// tile_type
const std::vector<std::vector<std::string>> tile_type = {
    {"Accelerator", "acc"},
    {"Processor", "cpu"},
    {"Memory", "mem"},
    {"Miscellaneous", "misc"},
    {"SLM", "slm"},
    {"Empty", "empty"}
};

// ip_acc
const std::vector<std::vector<std::string>> ip_acc = {
    {"dummy", "DUMMY_STRATUS"},
    {"adder", "ADDER_VIVADO"}
};

// impl_acc
const std::vector<std::vector<std::string>> impl_acc = {
    {"dummy_basic_dma64", "basic_dma64"},
    {"adder_dma32_w8", "dma32_w8"},
    {"adder_dma32_w16", "dma32_w16"},
    {"adder_dma32_w32", "dma32_w32"},
    {"adder_dma64_w8", "dma64_w8"},
    {"adder_dma64_w16", "dma64_w16"},
    {"adder_dma64_w32", "dma64_w32"},
};

// NAPBS
const int NAPBS = 128;

// NCPU_MAX
const int NCPU_MAX = 4;

// NMEM_MAX
const int NMEM_MAX = 4;

// NSLM_MAX
const int NSLM_MAX = 16;

// NTILE_MAX
const int NTILE_MAX = 64;

// NFULL_COHERENT_MAX
const int NFULL_COHERENT_MAX = 16;

// NLLC_COHERENT_MAX
const int NLLC_COHERENT_MAX = 64;

// NACC_MAX
const int NACC_MAX = NAPBS - 2 * NCPU_MAX - NMEM_MAX - NTILE_MAX - 8;


#endif // CONSTANTS_H
