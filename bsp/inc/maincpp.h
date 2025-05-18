#ifndef __MAINCPP_H
#define __MAINCPP_H

#ifdef __cplusplus
extern "C" {
#endif

void maincpp(); // 始终暴露为 C 接口声明，保证 C/C++ 都能看到

#ifdef __cplusplus
}
#endif

#endif
