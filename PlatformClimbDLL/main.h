#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>
#include <vector>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT EnterLeaderboardRecord(std::string name, int score);
int DLL_EXPORT GetPersonalBest(std::string name);
int DLL_EXPORT GetHighscore();

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
