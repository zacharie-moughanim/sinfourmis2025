#pragma once

// TODO: Equilibrate the game constants

// =======================
// Edge constants
// =======================
#define EDGE_LIFE 10U       // edge base life
#define EDGE_MIN_LIFE 3U    // edge minimum life to be crossed
#define EDGE_CROSS_SPEED 5U // edge crossing speed in unit/turn

// =======================
// Node constants
// =======================
#define NODE_MAX_FOOD 100U // node maximum food quantity
#define NODE_FOOD_REGEN 5U // node food regeneration quantity in unit/turn

// =======================
// Ant constants
// =======================
#define DEFAULT_MAX_LIFE 10U  // ant default maximum life
#define DEFAULT_MAX_WATER 10U // ant default maximum water
#define DEFAULT_MAX_FOOD 10U  // ant default maximum food
#define DEFAULT_ATTACK 1U     // ant default attack power

// =======================
// Queen constants
// =======================
#define QUEEN_DEFAULT_MAX_STORED_ANT 10U   // Queen default maximum stored ant
#define QUEEN_DEFAULT_PRODUCED_ANTS 1U     // Number of ant produced by the queen in one turn
#define QUEEN_DEFAULT_UPGRADE_DURATION 10U // Number of turn to upgrade something
#define QUEEN_REDUCE_UPGRADE_TIME_COST 10U // Cost to reduce the upgrade time in wining points
#define QUEEN_DEFAULT_SENT_ANTS 1U         // Maximum number of ant sent by the queen in one turn
#define QUEEN_UPGRADE_NB_ANT_COST 10U // Cost to upgrade the number of produced ant in wining points
#define QUEEN_UPGRADE_PRODUCED_ANTS_COST                                                           \
    10U // Cost to upgrade the number of produced ant in wining points
#define QUEEN_UPGRADE_NB_ANT_SENDING_COST                                                          \
    10U                               // Cost to upgrade the number of ant sent in wining points
#define QUEEN_UPGRADE_LIFE_COST 10U   // Cost to upgrade the ant life in wining points
#define QUEEN_UPGRADE_WATER_COST 10U  // Cost to upgrade the ant water reserve in wining points
#define QUEEN_UPGRADE_FOOD_COST 10U   // Cost to upgrade the ant food reserve in wining points
#define QUEEN_UPGRADE_ATTACK_COST 10U // Cost to upgrade the ant attack power in wining points
