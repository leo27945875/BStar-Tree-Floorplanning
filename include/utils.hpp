#pragma once

#include <random>

#define DEBUG 0

#if DEBUG == 1
    #define LOG(x)                   std::cout << x << std::endl
    #define LOGKV(x)                 std::cout << #x << " = " << (x) <<  std::endl
    #define LOOP_START(x)            std::cout << "========================== " << "Loop " << (x) << " ==========================\n" << std::endl
    #define UPDATE_BEST(x)           std::cout << "[Update Best Floorplan] Cost = " << (x) << std::endl
    #define SA_START(c, ar, as, t)   std::cout << "[Start SA]\n Init Cost = " << (c) << "\n Init Area = " << (ar) << "\n Init Aspect Ratio = " << (as) << "\n Init Temperature = " << t << std::endl
    #define SA_LOOP_END(c, t, r, rt) std::cout << "[End One Loop]\n Best Cost = " << (c) << "\n Temperature = " << (t) << "\n Reject Rate = " << (r) << "\n Run Time = " << (rt) << std::endl
    #define DDASH_LINE               std::cout << "==============================================================" << std::endl
    #define DASH_LINE                std::cout << "--------------------------------------------------------------" << std::endl
    #define MOVE_INFO(x)             std::cout << "Move " << x->m_number << "  (from: " << (x->m_group + 1) << " to: " << ((!x->m_group) + 1) << ")" << std::endl
    #define LOGXY(xy)                std::cout << "[XY] " << "X = " << xy.x << ", Y = " << xy.y << std::endl
    #define LOGBLOCK(b)              std::cout << "[Block" << ((b).number + 1) << "] " << "X = " << (b).x << ", Y = " << (b).y << ", W = " << (b).width << ", H = " << (b).height << std::endl
    #define END_LINE                 std::cout << std::endl
    #define AAA                      std::cout << "AAAAAAAAAAAAAAAAAAAA" << std::endl;
    #define BBB                      std::cout << "BBBBBBBBBBBBBBBBBBBB" << std::endl;
#else
    #define LOG(x) 
    #define LOGKV(x) 
    #define LOOP_START(x) 
    #define UPDATE_BEST(x) 
    #define SA_START(c, ar, as, t)
    #define SA_LOOP_END(c, t, r, rt) 
    #define DDASH_LINE 
    #define DASH_LINE 
    #define MOVE_INFO(x) 
    #define LOGXY(xy) 
    #define LOGBLOCK(b) 
    #define END_LINE 
    #define AAA 
    #define BBB 
#endif
 
#define FIXLOG(x)      std::cout << x << std::endl
#define FIXLOGKV(x, y) std::cout << x << " = " << y <<  std::endl
#define FIX_END_LINE   std::cout << std::endl

#define RANDOM_INT(MAX) (rand() % MAX)
#define RANDOM_BOOL     (static_cast<bool>(rand() % 2))
#define RANDOM_0TO1     (static_cast<double>(rand()) / RAND_MAX)
